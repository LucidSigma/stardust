#include "stardust/application/Application.h"

#include <string>
#include <utility>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stb/stb_image.h>
#include <toml++/toml.hpp>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/debug/assert/Assert.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/backend/OpenGL.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/Input.h"
#include "stardust/physics/world/World.h"
#include "stardust/system/System.h"
#include "stardust/ui/UI.h"
#include "stardust/utility/version/Version.h"
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

        m_entityRegistry.clear();
        m_globalSceneData.clear();

        Input::RemoveAllGameControllers();
        ui::Shutdown();

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

        ResetCursor();
        
        if (m_soundSystem.DidInitialiseSuccessfully())
        {
            m_soundSystem.StopAllSounds();
        }

    #ifndef NDEBUG
        debug::ResetAssertionCallback();
        Log::Shutdown();
    #endif

        vfs::Quit();

        TTF_Quit();
        SDL_Quit();
    }

    void Application::Run()
    {
        InitialiseScenes();

        SDL_Event event{ };
        f64 timeAccumulator = 0.0;
        m_ticksCount = std::chrono::high_resolution_clock::now();

        while (m_isRunning)
        {
            UpdateTime(timeAccumulator);
            m_soundSystem.Update();

            while (timeAccumulator >= m_fixedTimestep)
            {
                FixedUpdate();
                timeAccumulator -= m_fixedTimestep;
            }

            ProcessInput();
            Update();
            LateUpdate();

            Render();

            PollEvents(event);
            UpdateSceneQueue();
        }
    }

    void Application::FinishCurrentScene() noexcept
    {
        m_isCurrentSceneFinished = true;
    }

    void Application::ForceQuit() noexcept
    {
        m_isRunning = false;
    }

    void Application::RemoveFromGlobalSceneData(const String& dataName)
    {
        m_globalSceneData.erase(dataName);
    }

    [[nodiscard]] Status Application::CreateApplicationConfig(const String& appTOMLFilepath, const char* argv0, AppConfig& out_appConfig)
    {
        toml::table tomlData{ };
        
        if (filesystem::ReadTOML(appTOMLFilepath, tomlData) == Status::Fail)
        {
            return Status::Fail;
        }
        
        try
        {
            out_appConfig.applicationName = tomlData["application"]["application_name"].value<String>().value();
            out_appConfig.organisationName = tomlData["application"]["organisation_name"].value<String>().value();

            out_appConfig.allowResizableWindow = tomlData["window"]["can_resize"].value<bool>().value();
            out_appConfig.windowTitle = tomlData["window"]["title"].value<String>().value();

            out_appConfig.assetsArchiveRelativeFilepath = tomlData["filesystem"]["assets_archive_relative_path"].value<String>().value();
            out_appConfig.localesArchiveRelativeFilepath = tomlData["filesystem"]["locales_archive_relative_path"].value<String>().value();
            out_appConfig.logFileRelativeFilepath = tomlData["filesystem"]["log_file_relative_path"].value<String>().value();

            out_appConfig.defaultConfigFilepath = tomlData["filesystem"]["virtual_filepaths"]["default_config_path"].value<String>().value();
            out_appConfig.gameControllerDatabaseFilepath = tomlData["filesystem"]["virtual_filepaths"]["game_controller_database_path"].value<String>().value();
            out_appConfig.windowIconFilepath = tomlData["filesystem"]["virtual_filepaths"]["window_icon_path"].value<String>();

            out_appConfig.fixedTimestep = tomlData["physics"]["fixed_timestep"].value<f64>().value();
            out_appConfig.positionIterations = tomlData["physics"]["position_iterations"].value<u32>().value();
            out_appConfig.velocityIterations = tomlData["physics"]["velocity_iterations"].value<u32>().value();

            out_appConfig.argv0 = argv0;
        }
        catch (const std::bad_optional_access& error)
        {
            return Status::Fail;
        }

        return Status::Success;
    }

    void Application::Initialise(const CreateInfo& createInfo)
    {
        if (filesystem::InitialiseApplicationBaseDirectory() == Status::Fail)
        {
            message_box::Show("Filesystem Error", "Failed to get base directory of application.", message_box::Type::Error);

            return;
        }

        AppConfig appConfig{ };

        if (CreateApplicationConfig(filesystem::GetApplicationBaseDirectory() + createInfo.appTOMLRelativeFilepath, createInfo.argv0, appConfig) == Status::Fail)
        {
            message_box::Show("app.toml Error", "Could not find app.toml at " + createInfo.appTOMLRelativeFilepath, message_box::Type::Error);

            return;
        }

        if (filesystem::InitialiseApplicationPreferenceDirectory(appConfig.organisationName, appConfig.applicationName) == Status::Fail)
        {
            message_box::Show("Filesystem Error", "Failed to get preference directory of application.", message_box::Type::Error);

            return;
        }

    #ifndef NDEBUG
        {
            const String logFilepath = filesystem::GetApplicationBaseDirectory() + appConfig.logFileRelativeFilepath;
            Log::Initialise(appConfig.applicationName, logFilepath);
            debug::InitialiseAssertionCallback();
        }
    #endif

        Log::EngineInfo("Logger initialised [Stardust Version {}].", Version.ToString());
        Log::EngineDebug("Platform detected: \"{}\".", system::GetPlatformName());
        Log::EngineDebug("Base directory: \"{}\"", filesystem::GetApplicationBaseDirectory());
        Log::EngineInfo("ECS initialised.");

        static const Vector<std::function<Status(Application* const, const AppConfig&)>> initialisationFunctions{
            &Application::InitialiseFilesystem,
            &Application::InitialiseConfig,
            &Application::InitialiseLocale,
            &Application::InitialiseSoundSystem,
            &Application::InitialiseSDL,
            &Application::InitialiseWindow,
            &Application::InitialiseOpenGL,
            &Application::InitialiseRenderer,
            &Application::InitialiseTextSystem,
            &Application::InitialiseUI,
            &Application::InitialiseScriptEngine,
            &Application::InitialisePhysics,
            &Application::InitialiseInput,
        };

        for (const auto& initialisationFunction : initialisationFunctions)
        {
            if (initialisationFunction(this, appConfig) != Status::Success)
            {
                return;
            }
        }

        m_onInitialise = createInfo.initialiseCallback;
        m_onExit = createInfo.exitCallback;

        if (m_onInitialise.has_value())
        {
            if (m_onInitialise.value()(*this) != Status::Success)
            {
                return;
            }
        }

        stbi_set_flip_vertically_on_load(true);

        m_didInitialiseSuccessfully = true;
    }

    [[nodiscard]] Status Application::InitialiseFilesystem(const AppConfig& appConfig)
    {
        Log::EngineInfo("Filesystem initialised.");

        if (!vfs::Initialise(appConfig.argv0))
        {
            message_box::Show("Filesystem Error", "Virtual filesystem failed to initialise.", message_box::Type::Error);
            Log::EngineError("Failed to initialise virtual filesystem.");

            return Status::Fail;
        }

        const String assetsFilepath = filesystem::GetApplicationBaseDirectory() + appConfig.assetsArchiveRelativeFilepath;
        const String localesFilepath = filesystem::GetApplicationBaseDirectory() + appConfig.localesArchiveRelativeFilepath;

        vfs::AddToSearchPath({
            assetsFilepath,
            localesFilepath,
        });

        Log::EngineInfo("Virtual filesystem initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseConfig(const AppConfig& appConfig)
    {
        if (m_config.Initialise(filesystem::GetApplicationPreferenceDirectory(), appConfig.defaultConfigFilepath) == Status::Fail)
        {
            message_box::Show("Config Error", "Failed to load config file.", message_box::Type::Error);
            Log::EngineError("Failed to load config file.");

            return Status::Fail;
        }

        Log::EngineInfo("Config loaded.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseLocale(const AppConfig&)
    {
        m_locale.Initialise("locales");

        if (m_locale.SetLocale(m_config["locale"]) == Status::Fail)
        {
            message_box::Show("Locale Error", "Failed to load initial locale files.", message_box::Type::Error);
            Log::EngineError("Failed to load locale files for initial locale {}.", m_config["locale"]);

            return Status::Fail;
        }

        Log::EngineInfo("Locale \"{}\" loaded.", m_locale.GetCurrentLocaleName());

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseSoundSystem(const AppConfig&)
    {
        if (!m_soundSystem.DidInitialiseSuccessfully())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["sound"],
                m_locale["engine"]["errors"]["bodies"]["sound"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to initialise sound system.");

            return Status::Fail;
        }

        m_volumeManager.AddVolume(audio::VolumeManager::GetMasterVolumeName(), m_config["audio"]["volumes"]["master"]);
        Log::EngineInfo("Sound system initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseSDL(const AppConfig&)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["initialise"],
                m_locale["engine"]["errors"]["bodies"]["initialise-sdl"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to initialise SDL: {}.", SDL_GetError());

            return Status::Fail;
        }

        Log::EngineInfo("SDL initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseWindow(const AppConfig& appConfig)
    {
        const Vector<Pair<SDL_GLattr, i32>> glWindowAttributes{
            { SDL_GL_DOUBLEBUFFER, SDL_TRUE },
            { SDL_GL_CONTEXT_MAJOR_VERSION, 4 },
            { SDL_GL_CONTEXT_MINOR_VERSION, 6 },
            { SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE },
            { SDL_GL_ACCELERATED_VISUAL, SDL_TRUE },

            { SDL_GL_RED_SIZE, 8 },
            { SDL_GL_GREEN_SIZE, 8 },
            { SDL_GL_BLUE_SIZE, 8 },
            { SDL_GL_ALPHA_SIZE, 8 },
            { SDL_GL_DEPTH_SIZE, 24 },
            { SDL_GL_STENCIL_SIZE, 8 },

            { SDL_GL_MULTISAMPLEBUFFERS, 1 },
            { SDL_GL_MULTISAMPLESAMPLES, 4 },

        #if defined(__APPLE__) && !defined(NDEBUG)
            { SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
        #elif defined(__APPLE__)
            { SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
        #elif !defined(NDEBUG)
            { SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG },
        #endif
        };

        for (auto&& [attribute, value] : glWindowAttributes)
        {
            SDL_GL_SetAttribute(attribute, value);
        }

        Window::SetMinimiseOnFullscreenFocusLoss(m_config["graphics"]["window"]["enable-fullscreen-minimise"]);

        Vector<Window::CreateFlag> windowCreateFlags{ 
            Window::CreateFlag::AllowHighDPI,
            Window::CreateFlag::OpenGL,
            Window::CreateFlag::Shown,
        };

        if (appConfig.allowResizableWindow)
        {
            windowCreateFlags.push_back(Window::CreateFlag::Resizable);
        }

        if (m_config["graphics"]["window"]["fullscreen"])
        {
            windowCreateFlags.push_back(Window::CreateFlag::HardFullscreen);
        }

        if (m_config["graphics"]["window"]["borderless"])
        {
            windowCreateFlags.push_back(Window::CreateFlag::Borderless);
        }

        m_window.Initialise(Window::CreateInfo{
            .title = appConfig.windowTitle,
            .x = Window::Position::Centred,
            .y = Window::Position::Centred,
            .size = UVec2{ m_config["graphics"]["window"]["size"]["width"], m_config["graphics"]["window"]["size"]["height"] },
            .flags = std::move(windowCreateFlags),
        });

        if (!m_window.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["window"],
                m_locale["engine"]["errors"]["bodies"]["window"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to create window: {}.", SDL_GetError());

            return Status::Fail;
        }

        if (appConfig.windowIconFilepath.has_value() && !appConfig.windowIconFilepath.value().empty())
        {
            m_window.SetIcon(appConfig.windowIconFilepath.value(), m_locale);
        }

        Log::EngineInfo("Window created.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseOpenGL(const AppConfig&)
    {
        m_openGLContext.Initialise(m_window);
        
        if (!m_openGLContext.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["opengl"],
                m_locale["engine"]["errors"]["bodies"]["opengl-context"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to create OpenGL context: {}.", SDL_GetError());
        
            return Status::Fail;
        }
        
        if (m_openGLContext.MakeCurrent() != Status::Success)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["opengl"],
                m_locale["engine"]["errors"]["bodies"]["opengl-current"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to set current OpenGL context: {}.", SDL_GetError());
        
            return Status::Fail;
        }

        if (m_config["graphics"]["vsync"]["enabled"])
        {
            const bool useAdaptiveVSync = m_config["graphics"]["vsync"]["adaptive"];
            bool didAdaptiveVSyncFail = false;

            if (useAdaptiveVSync)
            {
                if (Window::SetVSync(VSyncType::Adaptive) != Status::Success)
                {
                    Log::EngineWarn("Failed to set adaptive VSync; defaulting to regular VSync.");
                    didAdaptiveVSyncFail = true;
                }
            }

            if (!useAdaptiveVSync || didAdaptiveVSyncFail)
            {
                if (Window::SetVSync(VSyncType::Regular) != Status::Success)
                {
                    Log::EngineWarn("Failed to set VSync.");
                    Window::SetVSync(VSyncType::None);
                }
            }
        }
        else
        {
            Window::SetVSync(VSyncType::None);
        }

        if (opengl::InitialiseLoader() != Status::Success)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["opengl"],
                m_locale["engine"]["errors"]["bodies"]["opengl-load"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to load OpenGL functions.");

            return Status::Fail;
        }

    #ifndef NDEBUG
        opengl::InitialiseDebugCallback();
    #endif

        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Log::EngineInfo("OpenGL set up successfully.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseRenderer(const AppConfig&)
    {
        m_renderer.Initialise(Renderer::CreateInfo{
            .window = &m_window,
            .virtualSize = UVec2{
                m_config["graphics"]["resolution"]["width"],
                m_config["graphics"]["resolution"]["height"],
            },
        });

        if (!m_renderer.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["renderer"],
                m_locale["engine"]["errors"]["bodies"]["renderer"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to initialise renderer.");

            return Status::Fail;
        }

        m_renderer.SetAntiAliasing(m_config["graphics"]["anti-aliasing"]);
        m_renderer.SetClearColour(colours::Black);

        m_camera.Initialise(8.0f, m_renderer);
        Log::EngineInfo("Renderer and camera created. Camera pixels per unit is {}.", m_camera.GetPixelsPerUnit());

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseTextSystem(const AppConfig&)
    {
        if (TTF_Init() != 0)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["ttf"],
                m_locale["engine"]["errors"]["bodies"]["ttf"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to initialise SDL_TTF: {}.", TTF_GetError());

            return Status::Fail;
        }

        Log::EngineInfo("Text subsystem initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseUI(const AppConfig&)
    {
        return ui::Initialise(*this);
    }

    [[nodiscard]] Status Application::InitialiseScriptEngine(const AppConfig&)
    {
        m_scriptEngine.Initialise(*this);
        Log::EngineInfo("Lua script engine initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialisePhysics(const AppConfig& appConfig)
    {
        m_fixedTimestep = appConfig.fixedTimestep;
        physics::World::SetVelocityIterations(appConfig.velocityIterations);
        physics::World::SetPositionIterations(appConfig.positionIterations);
        Log::EngineInfo("Physics subsystem initialised.");

        return Status::Success;
    }

    [[nodiscard]] Status Application::InitialiseInput(const AppConfig& appConfig)
    {
        Input::SetGameControllerDeadzone(m_config["input"]["controller-deadzone"]);

        if (Input::InitialiseGameControllerDatabase(appConfig.gameControllerDatabaseFilepath) != Status::Success)
        {
            Log::EngineWarn("Game controller database not loaded correctly - some controllers might not work properly.");
        }

        Log::EngineInfo("Input subsystem initialised.");

        return Status::Success;
    }

    void Application::InitialiseScenes()
    {
        if (m_didInitialiseSuccessfully)
        {
            m_isRunning = !m_sceneManager.IsEmpty();

            if (m_isRunning)
            {
                if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
                {
                    message_box::Show(
                        m_locale["engine"]["errors"]["titles"]["scene"],
                        m_locale["engine"]["errors"]["bodies"]["initial-scene"],
                        message_box::Type::Error
                    );
                    Log::EngineError("Failed to load initial scene {}.", m_sceneManager.CurrentScene()->GetName());
                    m_isRunning = false;
                }
                else
                {
                    Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
                }
            }
            else [[unlikely]]
            {
                message_box::Show(
                    m_locale["engine"]["warnings"]["titles"]["scene"],
                    m_locale["engine"]["warnings"]["bodies"]["initial-scene"],
                    message_box::Type::Warning
                );
                Log::EngineWarn("No initial scene loaded.");
            }
        }
        else
        {
            m_isRunning = false;
        }
    }

    void Application::FixedUpdate()
    {
        m_sceneManager.CurrentScene()->FixedUpdate(static_cast<f32>(m_fixedTimestep));
    }

    void Application::ProcessInput()
    {
        Input::UpdateKeyboardState();
        Input::UpdateMouseState();
        Input::UpdateGameControllers();

        m_sceneManager.CurrentScene()->ProcessInput(m_inputManager);
    }

    void Application::Update()
    {
        m_sceneManager.CurrentScene()->Update(m_deltaTime);
    }

    void Application::LateUpdate()
    {
        m_sceneManager.CurrentScene()->LateUpdate(m_deltaTime);
    }

    void Application::Render()
    {
        m_renderer.Clear();
        m_renderer.BeginFrame();

        m_sceneManager.CurrentScene()->Render(m_renderer);

        m_renderer.EndFrame(m_camera);
        m_window.Present();
    }

    void Application::PollEvents(Event& event)
    {
        Input::ResetScrollState();

        while (SDL_PollEvent(&event) != 0)
        {
            switch (GetEventType(event))
            {
            case EventType::Window:
                ProcessWindowEvents(event.window);

                break;

            case EventType::MouseScroll:
                Input::UpdateScrollState(event.wheel.y);

                break;

            case EventType::GameControllerDeviceAdd:
                if (const ObserverPtr<GameController> gameController = Input::AddGameController(event.cdevice.which, m_locale);
                    gameController != nullptr)
                {
                    m_sceneManager.CurrentScene()->OnGameControllerAdded(*gameController);
                }
                
                break;

            case EventType::GameControllerDeviceRemove:
                if (const ObserverPtr<const GameController> gameController = Input::GetGameController(event.cdevice.which);
                    gameController != nullptr)
                {
                    m_sceneManager.CurrentScene()->OnGameControllerRemoved(*gameController);
                }

                Input::RemoveGameController(event.cdevice.which);

                break;

            case EventType::Quit:
                m_isRunning = false;

                break;

            default:
                break;
            }

            m_sceneManager.CurrentScene()->PollEvent(event);
        }
    }

    void Application::ProcessWindowEvents(const WindowEvent& windowEvent)
    {
        switch (GetWindowEventType(windowEvent))
        {
        case WindowEventType::SizeChange:
            m_window.ProcessResize(UVec2{ windowEvent.data1, windowEvent.data2 });
            m_renderer.ProcessResize();
            m_camera.Refresh();

            break;

        case WindowEventType::Minimise:
        case WindowEventType::KeyboardFocusLoss:
            m_hasWindowFocus = false;

            break;

        case WindowEventType::KeyboardFocusGain:
            m_hasWindowFocus = true;

            if (Input::IsMouseInRelativeMode())
            {
                Input::ClearRelativeMouseState();
            }

            break;

        default:
            break;
        }
    }

    void Application::UpdateTime(f64& timeAccumulator)
    {
        const auto newTicks = std::chrono::high_resolution_clock::now();
        const auto frameTicks = newTicks - m_ticksCount;
        const f64 preciseDeltaTime = static_cast<f64>(frameTicks.count()) / static_cast<f64>(std::chrono::high_resolution_clock::period::den);

        m_deltaTime = static_cast<f32>(preciseDeltaTime);
        m_ticksCount = newTicks;
        m_elapsedTime += preciseDeltaTime;
        timeAccumulator += preciseDeltaTime;
    }

    void Application::UpdateSceneQueue()
    {
        if (m_isCurrentSceneFinished)
        {
            Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName());

            m_sceneManager.CurrentScene()->OnUnload();
            m_sceneManager.PopScene();

            m_renderer.SetClearColour(colours::Black);
            m_camera.ResetTransform();
            m_entityRegistry.clear();
            m_soundSystem.GetListener().Reset();

            if (!m_sceneManager.IsEmpty())
            {
                if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
                {
                    message_box::Show(
                        m_locale["engine"]["errors"]["titles"]["scene"],
                        m_locale["engine"]["errors"]["bodies"]["next-scene"],
                        message_box::Type::Error
                    );
                    Log::EngineError("Failed to load scene {}.", m_sceneManager.CurrentScene()->GetName());

                    m_isRunning = false;
                }
                else
                {
                    Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
                }
            }

            m_isCurrentSceneFinished = false;
        }

        if (m_sceneManager.IsEmpty())
        {
            m_isRunning = false;
        }
    }
}