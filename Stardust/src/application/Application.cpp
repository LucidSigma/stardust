#include "stardust/application/Application.h"

#include <limits>
#include <string>

#include "stardust/debug/assert/Assert.h"
#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/graphics/backend/OpenGL.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/keyboard/VirtualKeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/input/touch/TouchDevice.h"
#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/utility/message_box/MessageBox.h"
#include "stardust/window/cursor/Cursor.h"

namespace stardust
{
    Application::Application(const CreateInfo& createInfo)
    {
        Initialise(createInfo);
    }

    Application::~Application() noexcept
    {
        if (m_onExit.has_value())
        {
            m_onExit.value()(*this);
        }

        m_globalSceneResources.Clear();
        m_entityRegistry.ClearAllEntities();
        m_inputController.GetGameControllerLobby().RemoveAllGameControllers();
        m_inputController.GetJoystickLobby().RemoveAllJoysticks();

        if (m_soundSystem.IsValid())
        {
            m_soundSystem.StopAllSounds();
            m_soundSystem.Destroy();
        }

        if (m_renderer.IsValid())
        {
            m_renderer.Destroy();
        }

        if (m_openGLContext.IsValid())
        {
            m_openGLContext.Destroy();
        }

        if (m_window.IsValid())
        {
            m_window.Destroy();
        }

        cursor::Reset();

    #ifndef NDEBUG
        debug::ResetAssertionCallback();
        Log::Shutdown();
    #endif

        vfs::Quit();

        if (SDL_WasInit(SDL_INIT_EVERYTHING) != 0u)
        {
            SDL_Quit();
        }
    }

    auto Application::Run() -> void
    {
        InitialiseScenes();

        m_timestepController.Start();
        SDL_Event event{ };

        while (m_isRunning.load(std::memory_order::relaxed))
        {
            m_timestepController.Update(m_elapsedTime);

            while (m_timestepController.HasFixedTimeAccumulated())
            {
                FixedUpdate();
                m_timestepController.UpdateFixedTime();
            }

            m_timestepController.UpdateFixedTimeInterpolation();

            ProcessInput();

            PreUpdate();
            Update();
            PostUpdate();

            if (!m_timestepController.IsSynchronisingToTargetFrameRate() && !m_timestepController.SkipNextFrame())
            {
                Render();
            }

            PollEvents(event);
            UpdateSceneQueue();
        }

        if (!m_sceneManager.IsEmpty())
        {
            m_sceneManager.CurrentScene()->GetLayerStack().RemoveAllLayers();
            m_sceneManager.CurrentScene()->OnUnload();
        }
    }

    auto Application::ForceQuit() noexcept -> void
    {
        m_isRunning.store(false, std::memory_order::relaxed);
    }

    auto Application::PushUserEvent(const events::UserEvent& userEvent) -> void
    {
        m_userEvents.push(userEvent);
    }

    auto Application::FixedUpdate() -> void
    {
        m_sceneManager.CurrentScene()->FixedUpdate(static_cast<f32>(m_timestepController.GetFixedTimestep()));
    }

    auto Application::ProcessInput() -> void
    {
        m_inputController.Update();

        m_sceneManager.CurrentScene()->ProcessInput(m_inputController, m_inputManager);
    }

    auto Application::PreUpdate() -> void
    {
        m_sceneManager.CurrentScene()->PreUpdate(static_cast<f32>(m_timestepController.GetDeltaTime()));
    }

    auto Application::Update() -> void
    {
        m_sceneManager.CurrentScene()->Update(static_cast<f32>(m_timestepController.GetDeltaTime()));
    }

    auto Application::PostUpdate() -> void
    {
        m_sceneManager.CurrentScene()->PostUpdate(static_cast<f32>(m_timestepController.GetDeltaTime()));
        m_soundSystem.Update();
    }

    auto Application::Render() -> void
    {
        m_sceneManager.CurrentScene()->Render(m_renderer);

        m_window.Present();

    #ifndef NDEBUG
        opengl::CheckErrors();
    #endif
    }

    auto Application::PollEvents(SDL_Event& event) -> void
    {
        m_inputController.GetMouseState().ResetScrollState();

        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                ForceQuit();

                break;

            case SDL_KEYDOWN:
            {
                const auto keyDownEvent = events::KeyDown{
                    .keyCode = static_cast<KeyCode>(event.key.keysym.scancode),
                    .virtualKeyCode = static_cast<VirtualKeyCode>(event.key.keysym.sym),
                    .modState = static_cast<u32>(event.key.keysym.mod),
                    .isRepeat = event.key.repeat != 0u,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnKeyDown(keyDownEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnKeyDown(*this, keyDownEvent);
                }

                break;
            }

            case SDL_KEYUP:
            {
                const auto keyUpEvent = events::KeyUp{
                    .keyCode = static_cast<KeyCode>(event.key.keysym.scancode),
                    .virtualKeyCode = static_cast<VirtualKeyCode>(event.key.keysym.sym),
                    .modState = static_cast<u32>(event.key.keysym.mod),
                    .isRepeat = event.key.repeat != 0u,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnKeyUp(keyUpEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnKeyUp(*this, keyUpEvent);
                }

                break;
            }

            case SDL_TEXTINPUT:
            {
                const auto textInputEvent = events::TextInput{
                    .text = event.text.text,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnTextInput(textInputEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnTextInput(*this, textInputEvent);
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                const auto mouseButtonDownEvent = events::MouseButtonDown{
                    .mouseButton = static_cast<MouseButton>(event.button.button),
                    .coordinates = IVector2{ event.button.x, event.button.y },
                    .clickCount = static_cast<u32>(event.button.clicks),
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnMouseButtonDown(mouseButtonDownEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnMouseButtonDown(*this, mouseButtonDownEvent);
                }

                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                const auto mouseButtonUpEvent = events::MouseButtonUp{
                    .mouseButton = static_cast<MouseButton>(event.button.button),
                    .coordinates = IVector2{ event.button.x, event.button.y },
                    .clickCount = static_cast<u32>(event.button.clicks),
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnMouseButtonUp(mouseButtonUpEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnMouseButtonUp(*this, mouseButtonUpEvent);
                }

                break;
            }

            case SDL_MOUSEMOTION:
            {
                const auto mouseMotionEvent = events::MouseMotion{
                    .coordinates = IVector2{ event.motion.x, event.motion.y },
                    .relativeCoordinates = IVector2{ event.motion.xrel, event.motion.yrel },
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnMouseMotion(mouseMotionEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnMouseMotion(*this, mouseMotionEvent);
                }

                break;
            }

            case SDL_MOUSEWHEEL:
            {
                m_inputController.GetMouseState().UpdateScrollState(event.wheel.y, event.wheel.preciseY);

                const auto mouseScrollEvent = events::MouseScroll{
                    .scrollAmount = event.wheel.y,
                    .preciseScrollAmount = event.wheel.preciseY,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnMouseScroll(mouseScrollEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnMouseScroll(*this, mouseScrollEvent);
                }

                break;
            }

            case SDL_CONTROLLERDEVICEADDED:
                if (const ObserverPointer<GameController> gameController = m_inputController.GetGameControllerLobby().AddGameController(event.cdevice.which);
                    gameController != nullptr)
                {
                    Log::EngineInfo("Game controller \"{}\" added (ID: {}; GUID: {}).", gameController->GetName(), gameController->GetID(), gameController->GetGUID());

                    const auto gameControllerAddedEvent = events::GameControllerAdded{
                        .gameController = gameController,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerAdded(gameControllerAddedEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerAdded(*this, gameControllerAddedEvent);
                    }
                }
                else
                {
                    message_box::ShowComplex(
                        m_locale["engine"]["warnings"]["titles"]["game-controller"],
                        m_locale({ "engine", "warnings", "bodies", "game-controller" }, {
                            { "GAME_CONTROLLER_ID", std::to_string(event.cdevice.which) },
                        }),
                        message_box::Type::Warning,
                        List<message_box::ButtonData>{
                            message_box::ButtonData{
                                .id = 0,
                                .text = m_locale["engine"]["buttons"]["continue"],
                                .flags = {
                                    message_box::ButtonFlag::ReturnKeyDefault,
                                    message_box::ButtonFlag::EscapeKeyDefault,
                                },
                            },
                        }
                    );

                    Log::EngineWarn("Failed to add game controller (ID: {}).", event.cdevice.which);
                }

                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                if (const ObserverPointer<const GameController> gameController = m_inputController.GetGameControllerLobby().GetGameController(event.cdevice.which);
                    gameController != nullptr)
                {
                    Log::EngineInfo("Game controller {} removed.", gameController->GetID());

                    const auto gameControllerRemovedEvent = events::GameControllerRemoved{
                        .gameController = gameController,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerRemoved(gameControllerRemovedEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerRemoved(*this, gameControllerRemovedEvent);
                    }
                }

                m_inputController.GetGameControllerLobby().RemoveGameController(event.cdevice.which);

                break;

            case SDL_CONTROLLERBUTTONDOWN:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.cbutton.which))
                {
                    const auto gameControllerButtonDownEvent = events::GameControllerButtonDown{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.cbutton.which),
                        .button = static_cast<GameControllerButton>(static_cast<SDL_GameControllerButton>(event.cbutton.button)),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerButtonDown(gameControllerButtonDownEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerButtonDown(*this, gameControllerButtonDownEvent);
                    }
                }

                break;

            case SDL_CONTROLLERBUTTONUP:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.cbutton.which))
                {
                    const auto gameControllerButtonUpEvent = events::GameControllerButtonUp{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.cbutton.which),
                        .button = static_cast<GameControllerButton>(static_cast<SDL_GameControllerButton>(event.cbutton.button)),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerButtonUp(gameControllerButtonUpEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerButtonUp(*this, gameControllerButtonUpEvent);
                    }
                }

                break;

            case SDL_CONTROLLERAXISMOTION:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.caxis.which))
                {
                    const auto gameControllerAxisMotionEvent = events::GameControllerAxisMotion{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.caxis.which),
                        .axis = static_cast<GameControllerAxis>(static_cast<SDL_GameControllerAxis>(event.caxis.axis)),
                        .value = glm::clamp(static_cast<f32>(event.caxis.value) / static_cast<f32>(std::numeric_limits<i16>::max()), -1.0f, 1.0f),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerAxisMotion(gameControllerAxisMotionEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerAxisMotion(*this, gameControllerAxisMotionEvent);
                    }
                }

                break;

            case SDL_CONTROLLERTOUCHPADDOWN:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.ctouchpad.which))
                {
                    const auto gameControllerTouchpadFingerDownEvent = events::GameControllerTouchpadFingerDown{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.ctouchpad.which),
                        .fingerIndex = static_cast<usize>(event.ctouchpad.finger),
                        .position = Vector2{ event.ctouchpad.x, event.ctouchpad.y },
                        .pressure = event.ctouchpad.pressure,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerTouchpadFingerDown(gameControllerTouchpadFingerDownEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerTouchpadFingerDown(*this, gameControllerTouchpadFingerDownEvent);
                    }
                }

                break;

            case SDL_CONTROLLERTOUCHPADUP:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.ctouchpad.which))
                {
                    const auto gameControllerTouchpadFingerUpEvent = events::GameControllerTouchpadFingerUp{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.ctouchpad.which),
                        .fingerIndex = static_cast<usize>(event.ctouchpad.finger),
                        .position = Vector2{ event.ctouchpad.x, event.ctouchpad.y },
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerTouchpadFingerUp(gameControllerTouchpadFingerUpEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerTouchpadFingerUp(*this, gameControllerTouchpadFingerUpEvent);
                    }
                }

                break;

            case SDL_CONTROLLERTOUCHPADMOTION:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.ctouchpad.which))
                {
                    const auto gameControllerTouchpadFingerMotionEvent = events::GameControllerTouchpadFingerMotion{
                        .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.ctouchpad.which),
                        .fingerIndex = static_cast<usize>(event.ctouchpad.finger),
                        .position = Vector2{ event.ctouchpad.x, event.ctouchpad.y },
                        .pressure = event.ctouchpad.pressure,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerTouchpadFingerMotion(gameControllerTouchpadFingerMotionEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnGameControllerTouchpadFingerMotion(*this, gameControllerTouchpadFingerMotionEvent);
                    }
                }

                break;

            case SDL_CONTROLLERSENSORUPDATE:
                if (m_inputController.GetGameControllerLobby().DoesGameControllerExist(event.csensor.which))
                {
                    switch (event.csensor.sensor)
                    {
                    case SDL_SENSOR_ACCEL:
                    {
                        const auto gameControllerAccelerometerUpdateEvent = events::GameControllerAccelerometerUpdate{
                            .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.csensor.which),
                            .acceleration = Vector3{ event.csensor.data[0], event.csensor.data[1], event.csensor.data[2] },
                        };

                        const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerAccelerometerUpdate(gameControllerAccelerometerUpdateEvent);

                        if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                        {
                            m_globalEventHandler->OnGameControllerAccelerometerUpdate(*this, gameControllerAccelerometerUpdateEvent);
                        }

                        break;
                    }

                    case SDL_SENSOR_GYRO:
                    {
                        const auto gameControllerGyroscopeUpdateEvent = events::GameControllerGyroscopeUpdate{
                            .gameController = m_inputController.GetGameControllerLobby().GetGameController(event.csensor.which),
                            .rotation = Vector3{ event.csensor.data[0], event.csensor.data[1], event.csensor.data[2] },
                        };

                        const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnGameControllerGyroscopeUpdate(gameControllerGyroscopeUpdateEvent);

                        if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                        {
                            m_globalEventHandler->OnGameControllerGyroscopeUpdate(*this, gameControllerGyroscopeUpdateEvent);
                        }

                        break;
                    }
                    }
                }

                break;

            case SDL_JOYDEVICEADDED:
                if (SDL_IsGameController(event.jdevice.which))
                {
                    break;
                }

                if (const ObserverPointer<Joystick> joystick = m_inputController.GetJoystickLobby().AddJoystick(event.jdevice.which);
                    joystick != nullptr)
                {
                    Log::EngineInfo("Joystick \"{}\" added (ID: {}; GUID: {}).", joystick->GetName(), joystick->GetID(), joystick->GetGUID());

                    const auto joystickAddedEvent = events::JoystickAdded{
                        .joystick = joystick,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickAdded(joystickAddedEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickAdded(*this, joystickAddedEvent);
                    }
                }
                else
                {
                    message_box::ShowComplex(
                        m_locale["engine"]["warnings"]["titles"]["joystick"],
                        m_locale({ "engine", "warnings", "bodies", "joystick" }, {
                            { "JOYSTICK_ID", std::to_string(event.jdevice.which) },
                        }),
                        message_box::Type::Warning,
                        List<message_box::ButtonData>{
                            message_box::ButtonData{
                                .id = 0,
                                .text = m_locale["engine"]["buttons"]["continue"],
                                .flags = {
                                    message_box::ButtonFlag::ReturnKeyDefault,
                                    message_box::ButtonFlag::EscapeKeyDefault,
                                },
                            },
                        }
                    );

                    Log::EngineWarn("Failed to add joystick (ID: {}).", event.jdevice.which);
                }

                break;

            case SDL_JOYDEVICEREMOVED:
                if (SDL_IsGameController(event.jdevice.which))
                {
                    break;
                }

                if (const ObserverPointer<const Joystick> joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jdevice.which);
                    joystick != nullptr)
                {
                    Log::EngineInfo("Joystick {} removed.", joystick->GetID());

                    const auto joystickRemovedEvent = events::JoystickRemoved{
                        .joystick = joystick,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickRemoved(joystickRemovedEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickRemoved(*this, joystickRemovedEvent);
                    }
                }

                m_inputController.GetJoystickLobby().RemoveJoystick(event.jdevice.which);

                break;

            case SDL_JOYBUTTONDOWN:
                if (m_inputController.GetJoystickLobby().DoesJoystickExist(event.jbutton.which))
                {
                    const auto joystickButtonDownEvent = events::JoystickButtonDown{
                        .joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jbutton.which),
                        .button = static_cast<Joystick::ButtonID>(event.jbutton.button),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickButtonDown(joystickButtonDownEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickButtonDown(*this, joystickButtonDownEvent);
                    }
                }

                break;

            case SDL_JOYBUTTONUP:
                if (m_inputController.GetJoystickLobby().DoesJoystickExist(event.jbutton.which))
                {
                    const auto joystickButtonUpEvent = events::JoystickButtonUp{
                        .joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jbutton.which),
                        .button = static_cast<Joystick::ButtonID>(event.jbutton.button),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickButtonUp(joystickButtonUpEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickButtonUp(*this, joystickButtonUpEvent);
                    }
                }

                break;

            case SDL_JOYAXISMOTION:
                if (m_inputController.GetJoystickLobby().DoesJoystickExist(event.jaxis.which))
                {
                    const auto joystickAxisMotionEvent = events::JoystickAxisMotion{
                        .joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jaxis.which),
                        .axis = static_cast<Joystick::AxisID>(event.jaxis.axis),
                        .value = glm::clamp(static_cast<f32>(event.jaxis.value) / static_cast<f32>(std::numeric_limits<i16>::max()), -1.0f, 1.0f),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickAxisMotion(joystickAxisMotionEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickAxisMotion(*this, joystickAxisMotionEvent);
                    }
                }

                break;

            case SDL_JOYHATMOTION:
                if (m_inputController.GetJoystickLobby().DoesJoystickExist(event.jhat.which))
                {
                    const auto joystickHatSwitchMotionEvent = events::JoystickHatSwitchMotion{
                        .joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jhat.which),
                        .hatSwitch = static_cast<Joystick::HatSwitchID>(event.jhat.hat),
                        .direction = static_cast<JoystickHatSwitchDirection>(event.jhat.value),
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickHatSwitchMotion(joystickHatSwitchMotionEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickHatSwitchMotion(*this, joystickHatSwitchMotionEvent);
                    }
                }

                break;

            case SDL_JOYBALLMOTION:
                if (m_inputController.GetJoystickLobby().DoesJoystickExist(event.jball.which))
                {
                    const auto joystickTrackballMotionEvent = events::JoystickTrackballMotion{
                        .joystick = m_inputController.GetJoystickLobby().GetJoystick(event.jball.which),
                        .trackball = static_cast<Joystick::TrackballID>(event.jball.ball),
                        .relativeMotion = Vector2{
                            static_cast<f32>(event.jball.xrel),
                            static_cast<f32>(event.jball.yrel),
                        },
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnJoystickTrackballMotion(joystickTrackballMotionEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnJoystickTrackballMotion(*this, joystickTrackballMotionEvent);
                    }
                }

                break;

            case SDL_FINGERDOWN:
            {
                const auto touchFingerDownEvent = events::TouchFingerDown{
                    .touchDevice = TouchDevice(event.tfinger.touchId),
                    .fingerID = event.tfinger.fingerId,
                    .position = Vector2{ event.tfinger.x, event.tfinger.y },
                    .pressure = event.tfinger.pressure,
                    .isOnGameWindow = static_cast<Window::ID>(event.tfinger.windowID) == m_window.GetID(),
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnTouchFingerDown(touchFingerDownEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnTouchFingerDown(*this, touchFingerDownEvent);
                }

                break;
            }

            case SDL_FINGERUP:
            {
                const auto touchFingerUpEvent = events::TouchFingerUp{
                    .touchDevice = TouchDevice(event.tfinger.touchId),
                    .fingerID = event.tfinger.fingerId,
                    .position = Vector2{ event.tfinger.x, event.tfinger.y },
                    .isOnGameWindow = static_cast<Window::ID>(event.tfinger.windowID) == m_window.GetID(),
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnTouchFingerUp(touchFingerUpEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnTouchFingerUp(*this, touchFingerUpEvent);
                }

                break;
            }

            case SDL_FINGERMOTION:
            {
                const auto touchFingerMotionEvent = events::TouchFingerMotion{
                    .touchDevice = TouchDevice(event.tfinger.touchId),
                    .fingerID = event.tfinger.fingerId,
                    .position = Vector2{ event.tfinger.x, event.tfinger.y },
                    .deltaPosition = Vector2{ event.tfinger.dx, event.tfinger.dy },
                    .pressure = event.tfinger.pressure,
                    .isOnGameWindow = static_cast<Window::ID>(event.tfinger.windowID) == m_window.GetID(),
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnTouchFingerMotion(touchFingerMotionEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnTouchFingerMotion(*this, touchFingerMotionEvent);
                }

                break;
            }

            case SDL_MULTIGESTURE:
            {
                const auto multiTouchFingerGestureEvent = events::MultiTouchFingerGesture{
                    .touchDevice = TouchDevice(event.mgesture.touchId),
                    .fingerCount = static_cast<u32>(event.mgesture.numFingers),
                    .normalisedCentre = Vector2{ event.mgesture.x, event.mgesture.y },
                    .pinchAmount = event.mgesture.dDist,
                    .rotationAmount = event.mgesture.dTheta,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnMultiTouchFingerGesture(multiTouchFingerGestureEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnMultiTouchFingerGesture(*this, multiTouchFingerGestureEvent);
                }

                break;
            }

            case SDL_DOLLARGESTURE:
            {
                const auto dollarGesturePerformedEvent = events::DollarGesturePerformed{
                    .touchDevice = TouchDevice(event.dgesture.touchId),
                    .gestureID = event.dgesture.gestureId,
                    .fingerCount = static_cast<u32>(event.dgesture.numFingers),
                    .normalisedCentre = Vector2{ event.dgesture.x, event.dgesture.y },
                    .errorAmount = event.dgesture.error,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnDollarGesturePerformed(dollarGesturePerformedEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnDollarGesturePerformed(*this, dollarGesturePerformedEvent);
                }

                break;
            }

            case SDL_DOLLARRECORD:
            {
                const auto dollarGestureRecordedEvent = events::DollarGestureRecorded{
                    .touchDevice = TouchDevice(event.dgesture.touchId),
                    .gestureID = event.dgesture.gestureId,
                };

                const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnDollarGestureRecorded(dollarGestureRecordedEvent);

                if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                {
                    m_globalEventHandler->OnDollarGestureRecorded(*this, dollarGestureRecordedEvent);
                }

                break;
            }

            case SDL_WINDOWEVENT:
                ProcessWindowEvent(event.window);

                break;

            case SDL_USEREVENT:
                switch (event.user.code)
                {
                case Locale::ChangeEventCode:
                {
                    const String newLocale = static_cast<const char*>(event.user.data1);
                    Log::EngineInfo("Locale changed to {}.", newLocale);

                    const auto localeChangedEvent = events::LocaleChanged{
                        .newLocaleName = newLocale,
                    };

                    const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnLocaleChanged(localeChangedEvent);

                    if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
                    {
                        m_globalEventHandler->OnLocaleChanged(*this, localeChangedEvent);
                    }

                    break;
                }
                }

                break;
            }
        }

        while (!m_userEvents.empty())
        {
            const events::UserEvent& userEvent = m_userEvents.front();
            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnUserEvent(userEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnUserEvent(*this, userEvent);
            }

            m_userEvents.pop();
        }
    }

    auto Application::ProcessWindowEvent(const SDL_WindowEvent& windowEvent) -> void
    {
        switch (windowEvent.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
        {
            const auto windowShownEvent = events::WindowShown{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowShown(windowShownEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowShown(*this, windowShownEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_HIDDEN:
        {
            const auto windowHiddenEvent = events::WindowHidden{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowHidden(windowHiddenEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowHidden(*this, windowHiddenEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_MOVED:
        {
            const auto windowMovedEvent = events::WindowMoved{
                .window = &m_window,
                .newPosition = IVector2{ windowEvent.data1, windowEvent.data2 },
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowMoved(windowMovedEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowMoved(*this, windowMovedEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
            m_window.ProcessResize(UVector2{
                windowEvent.data1,
                windowEvent.data2,
                });

            m_camera.ResetVirtualScreenSize(UVector2{
                windowEvent.data1,
                windowEvent.data2,
                });

            m_renderer.ProcessResize();

            const auto windowResizedEvent = events::WindowResized{
                .window = &m_window,
                .newSize = m_window.GetSize(),
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowResized(windowResizedEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowResized(*this, windowResizedEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_MINIMIZED:
        {
            m_hasWindowFocus = false;

            const auto windowMinimisedEvent = events::WindowMinimised{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowMinimised(windowMinimisedEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowMinimised(*this, windowMinimisedEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_MAXIMIZED:
        {
            const auto windowMaximisedEvent = events::WindowMaximised{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowMaximised(windowMaximisedEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowMaximised(*this, windowMaximisedEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_RESTORED:
        {
            m_hasWindowFocus = true;

            const auto windowRestoredEvent = events::WindowRestored{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowRestored(windowRestoredEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowRestored(*this, windowRestoredEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_ENTER:
        {
            const auto windowMouseEnterEvent = events::WindowMouseEnter{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowMouseEnter(windowMouseEnterEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowMouseEnter(*this, windowMouseEnterEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_LEAVE:
        {
            const auto windowMouseExitEvent = events::WindowMouseExit{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowMouseExit(windowMouseExitEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowMouseExit(*this, windowMouseExitEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
            m_hasWindowFocus = true;

            const auto windowFocusGainedEvent = events::WindowFocusGained{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowFocusGained(windowFocusGainedEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowFocusGained(*this, windowFocusGainedEvent);
            }

            break;
        }

        case SDL_WINDOWEVENT_FOCUS_LOST:
        {
            m_hasWindowFocus = false;

            const auto windowFocusLostEvent = events::WindowFocusLost{
                .window = &m_window,
            };

            const EventStatus eventStatus = m_sceneManager.CurrentScene()->OnWindowFocusLost(windowFocusLostEvent);

            if (eventStatus == EventStatus::NotHandled && m_globalEventHandler != nullptr)
            {
                m_globalEventHandler->OnWindowFocusLost(*this, windowFocusLostEvent);
            }

            break;
        }
        }
    }

    auto Application::UpdateSceneQueue() -> void
    {
        if (m_sceneManager.CurrentScene()->IsFinished())
        {
            m_sceneManager.CurrentScene()->GetLayerStack().RemoveAllLayers();
            m_sceneManager.CurrentScene()->OnUnload();

            Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName());

            m_sceneManager.PopScene();

            m_camera.ResetTransform();
            m_soundSystem.GetListener().Reset();

            if (!m_sceneManager.IsEmpty())
            {
                if (m_sceneManager.CurrentScene()->OnLoad() != Status::Success)
                {
                    message_box::Show(
                        m_locale["engine"]["errors"]["titles"]["scene"],
                        m_locale({ "engine", "errors", "bodies", "next-scene" }, {
                            { "SCENE_NAME", m_sceneManager.CurrentScene()->GetName() },
                        }),
                        message_box::Type::Error
                    );
                    Log::EngineError("Failed to load scene {}.", m_sceneManager.CurrentScene()->GetName());

                    m_isRunning.store(false, std::memory_order::relaxed);
                }
                else
                {
                    Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
                }
            }
        }

        if (m_sceneManager.IsEmpty())
        {
            m_isRunning.store(false, std::memory_order::relaxed);
        }
    }
}
