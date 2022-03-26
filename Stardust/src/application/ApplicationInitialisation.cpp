#include "stardust/application/Application.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <limits>

#include <magic_enum/magic_enum.hpp>
#include <physfs/physfs.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "stardust/debug/assert/Assert.h"
#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/physics/world/World.h"
#include "stardust/types/MathTypes.h"
#include "stardust/utility/message_box/MessageBox.h"

namespace stardust
{
    auto Application::Initialise(const CreateInfo& createInfo) -> void
    {
        if (filesystem::InitialiseApplicationBaseDirectory() != Status::Success)
        {
            message_box::Show(
                "Filesystem Error",
                "Failed to get base directory of application.",
                message_box::Type::Error
            );

            return;
        }

        if (filesystem::InitialiseApplicationPreferenceDirectory(createInfo.applicationInfo.organisationName, createInfo.applicationInfo.applicationName) != Status::Success)
        {
            message_box::Show("Filesystem Error", "Failed to get user preference directory.", message_box::Type::Error);

            return;
        }

    #ifndef NDEBUG
        {
            const String logFilepath = filesystem::GetApplicationBaseDirectory() + createInfo.filesystemInfo.logFileRelativeFilepath;
            Log::Initialise(createInfo.applicationInfo.applicationName, logFilepath);
        }

        debug::InitialiseAssertionCallback();
    #endif

        Log::EngineInfo("Logger initialised.");
        Log::EngineDebug("Base directory: {}", filesystem::GetApplicationBaseDirectory());
        Log::EngineDebug("Preference directory: {}", filesystem::GetApplicationPreferenceDirectory());

        static const List<std::function<auto(Application* const, const CreateInfo&) -> Status>> initialisationFunctions{
            &Application::InitialiseFilesystem,
            &Application::InitialiseUserPrefs,
            &Application::InitialiseLocale,
            &Application::InitialiseSDL,
            &Application::InitialiseWindow,
            &Application::InitialiseGraphics,
            &Application::InitialiseRenderer,
            &Application::InitialiseAudio,
            &Application::InitialiseScriptEngine,
            &Application::InitialisePhysics,
            &Application::InitialiseInput,
        };

        for (const auto& initialisationFunction : initialisationFunctions)
        {
            if (initialisationFunction(this, createInfo) != Status::Success)
            {
                return;
            }
        }

        stbi_set_flip_vertically_on_load(static_cast<i32>(true));
        stbi_flip_vertically_on_write(static_cast<i32>(true));

        if (!m_didUserPrefsFileExist)
        {
            if (m_userPrefs.Save() != Status::Success)
            {
                message_box::ShowComplex(
                    m_locale["engine"]["warnings"]["titles"]["preferences"],
                    m_locale["engine"]["warnings"]["bodies"]["save-preferences"],
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

                Log::EngineWarn("Failed to save user preferences.");
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

        m_didInitialiseSuccessfully = true;
    }

    [[nodiscard]] auto Application::InitialiseFilesystem(const CreateInfo& createInfo) -> Status
    {
        if (vfs::Initialise(createInfo.argv0) != Status::Success)
        {
            const String physFSError = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

            message_box::Show("Filesystem Error", std::format("Failed to initialise virtual filesystem. Error: {}.", physFSError), message_box::Type::Error);
            Log::EngineError("Failed to initialise virtual filesystem: {}.", physFSError);

            return Status::Fail;
        }

        if (vfs::AddToSearchPath(filesystem::GetApplicationBaseDirectory() + createInfo.filesystemInfo.assetsArchiveRelativeFilepath) != Status::Success)
        {
            const String physFSError = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

            message_box::Show("Filesystem Error", std::format("Failed to add assets to virtual filesystem. Error: {}.", physFSError), message_box::Type::Error);
            Log::EngineError("Failed to add assets to virtual filesystem: {}.", physFSError);

            return Status::Fail;
        }

        if (vfs::AddToSearchPath(filesystem::GetApplicationBaseDirectory() + createInfo.filesystemInfo.localesArchiveRelativeFilepath) != Status::Success)
        {
            const String physFSError = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

            message_box::Show("Filesystem Error", std::format("Failed to add locales to virtual filesystem. Error: {}.", physFSError), message_box::Type::Error);
            Log::EngineError("Failed to add locales to virtual filesystem: {}.", physFSError);

            return Status::Fail;
        }

        if (vfs::AddToSearchPath(filesystem::GetApplicationBaseDirectory() + createInfo.filesystemInfo.scriptsArchiveRelativeFilepath) != Status::Success)
        {
            const String physFSError = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

            message_box::Show("Filesystem Error", std::format("Failed to add scripts to virtual filesystem. Error: {}.", physFSError), message_box::Type::Error);
            Log::EngineError("Failed to add scripts to virtual filesystem: {}.", physFSError);

            return Status::Fail;
        }

        Log::EngineInfo("Virtual filesystem initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseUserPrefs(const CreateInfo& createInfo) -> Status
    {
        const String preferencesDirectory = filesystem::GetApplicationPreferenceDirectory() + createInfo.preferenceInfo.preferencesRelativeDirectory;
        
        if (!filesystem::DoesPathExist(preferencesDirectory))
        {
            if (filesystem::CreateDirectory(preferencesDirectory) != Status::Success)
            {
                Log::EngineWarn("Failed to create user preference directory.");
            }
        }

        if (m_userPrefs.Load(preferencesDirectory + filesystem::GetDirectorySeparator() + createInfo.preferenceInfo.userPrefsFilename) != Status::Success)
        {
            Log::EngineWarn("No user preference file located; using defaults...");
            m_didUserPrefsFileExist = false;
        }
        else
        {
            Log::EngineInfo("User preferences loaded from file.");
        }

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseLocale(const CreateInfo&) -> Status
    {
        constexpr const char* DefaultLocale = "english_uk";

        m_locale.Initialise("locales");
        const String userLocale = m_userPrefs.GetOrDefault<String>("locale", "language", DefaultLocale);

        if (m_locale.SetLocale(userLocale, false) != Status::Success)
        {
            message_box::Show("Locale Error", std::format("Failed to load initial locale files for locale {}.", userLocale), message_box::Type::Error);
            Log::EngineError("Failed to load locale files for initial locale {}.", userLocale);

            return Status::Fail;
        }

        Log::EngineInfo("Locale \"{}\" loaded.", m_locale.GetCurrentLocaleName());

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseSDL(const CreateInfo& createInfo) -> Status
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["sdl"],
                m_locale({ "engine", "errors", "bodies", "initialise-sdl" }, {
                    { "SDL2_ERROR", SDL_GetError() },
                }),
                message_box::Type::Error
            );

            Log::EngineCritical("Failed to initialise SDL: {}.", SDL_GetError());

            return Status::Fail;
        }

        const List<Pair<String, String>> sdlVideoHints{
            { SDL_HINT_OPENGL_ES_DRIVER, "1" },
            { SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none" },
        };

        for (const auto& [hintName, hintValue] : sdlVideoHints)
        {
            SDL_SetHintWithPriority(hintName.c_str(), hintValue.c_str(), SDL_HINT_OVERRIDE);
        }

        if (const char* const openGLESDriverHint = SDL_GetHint(SDL_HINT_OPENGL_ES_DRIVER);
            openGLESDriverHint == nullptr || std::strcmp(openGLESDriverHint, "1") != 0)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["sdl"],
                m_locale["engine"]["errors"]["bodies"]["gles-driver-hint"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to set OpenGL ES driver hint.");

            return Status::Fail;
        }

    #ifdef STARDUST_PLATFORM_WINDOWS
        if (const char* const direct3DCompilerHint = SDL_GetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER);
            direct3DCompilerHint == nullptr || std::strcmp(direct3DCompilerHint, "none") != 0)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["sdl"],
                m_locale["engine"]["errors"]["bodies"]["direct3d-compiler-hint"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to set Direct3D compiler hint.");

            return Status::Fail;
        }
    #endif

        SDL_SetHintWithPriority(SDL_HINT_APP_NAME, createInfo.applicationInfo.applicationName.c_str(), SDL_HINT_OVERRIDE);

        const u32 customEventID = SDL_RegisterEvents(1);

        if (customEventID == std::numeric_limits<u32>::max())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["sdl"],
                m_locale({ "engine", "errors", "bodies", "custom-event-allocation" }, {
                    { "SDL2_ERROR", SDL_GetError() },
                }),
                message_box::Type::Error
            );

            Log::EngineWarn("Failed to allocate custom SDL2 events. Error: {}.", SDL_GetError());
        }

        Locale::SetLocaleChangeEventID(customEventID);

        Log::EngineInfo("SDL2 initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseWindow(const CreateInfo& createInfo) -> Status
    {
        const List<Pair<SDL_GLattr, i32>> openGLWindowAttributes{
            { SDL_GL_CONTEXT_EGL, SDL_TRUE },
            { SDL_GL_CONTEXT_MAJOR_VERSION, 3 },
            { SDL_GL_CONTEXT_MINOR_VERSION, 0 },
            { SDL_GL_DOUBLEBUFFER, SDL_TRUE },

            { SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES },
            { SDL_GL_ACCELERATED_VISUAL, SDL_TRUE },

            { SDL_GL_RED_SIZE, 8 },
            { SDL_GL_GREEN_SIZE, 8 },
            { SDL_GL_BLUE_SIZE, 8 },
            { SDL_GL_ALPHA_SIZE, 8 },
            { SDL_GL_DEPTH_SIZE, 24 },
            { SDL_GL_STENCIL_SIZE, 8 },
        };

        for (const auto [attribute, value] : openGLWindowAttributes)
        {
            if (SDL_GL_SetAttribute(attribute, value) != 0)
            {
                message_box::ShowComplex(
                    m_locale["engine"]["warnings"]["titles"]["sdl"],
                    m_locale({ "engine", "warnings", "bodies", "opengl-attribute" }, {
                        { "OPENGL_ATTRIBUTE", magic_enum::enum_name(attribute).data() },
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
                Log::EngineWarn("Failed to set OpenGL attribute {}: {}.", magic_enum::enum_name(attribute).data(), SDL_GetError());
            }
        }

        Window::SetMinimiseOnFullscreenFocusLoss(
            m_userPrefs.GetOrDefault(
                "window",
                "minimise_on_fullscreen_focus_loss",
                false
            )
        );

        List<Window::CreateFlag> windowCreateFlags{
            Window::CreateFlag::OpenGL,
            Window::CreateFlag::Shown,
        };

        if (createInfo.windowInfo.isResizable)
        {
            windowCreateFlags.push_back(Window::CreateFlag::Resizable);
        }

        if (m_userPrefs.GetOrDefault("window", "is_fullscreen", false))
        {
            windowCreateFlags.push_back(Window::CreateFlag::HardFullscreen);
        }

        if (m_userPrefs.GetOrDefault("window", "is_borderless", false))
        {
            windowCreateFlags.push_back(Window::CreateFlag::Borderless);
        }

        constexpr UVector2 DefaultWindowSize{ 1'280u, 720u };

        m_window.Initialise(Window::CreateInfo{
            .title = createInfo.windowInfo.title,
            .x = Window::Position::Centred,
            .y = Window::Position::Centred,
            .size = UVector2{ 
                m_userPrefs.GetOrDefault("window", "width", DefaultWindowSize.x),
                m_userPrefs.GetOrDefault("window", "height", DefaultWindowSize.y),
            },
            .flags = std::move(windowCreateFlags),
        });

        if (!m_window.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["window"],
                m_locale({ "engine", "errors", "bodies", "window" }, {
                    { "SDL2_ERROR", SDL_GetError() },
                }),
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to create window: {}.", SDL_GetError());

            return Status::Fail;
        }

        if (m_window.SetIcon(createInfo.virtualFilesystemInfo.windowIconPath) != Status::Success)
        {
            message_box::ShowComplex(
                m_locale["engine"]["warnings"]["titles"]["window"],
                m_locale["engine"]["warnings"]["bodies"]["window-icon"],
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
            Log::EngineWarn("Could not set window icon (virtual filepath: {}).", createInfo.virtualFilesystemInfo.windowIconPath);
        }

        Log::EngineInfo("Window created.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseGraphics(const CreateInfo&) -> Status
    {
        m_openGLContext.Initialise(m_window);

        if (!m_openGLContext.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["opengl"],
                m_locale({ "engine", "errors", "bodies", "opengl-context" }, {
                    { "SDL2_ERROR", SDL_GetError() },
                }),
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to create OpenGL context: {}.", SDL_GetError());

            return Status::Fail;
        }

        if (m_openGLContext.MakeCurrent() != Status::Success)
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["opengl"],
                m_locale({ "engine", "errors", "bodies", "opengl-make-current" }, {
                    { "SDL2_ERROR", SDL_GetError() },
                }),
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to set current OpenGL context: {}.", SDL_GetError());

            return Status::Fail;
        }

        const bool enableVSync = m_userPrefs.GetOrDefault("graphics", "enable_vsync", false);
        const bool useAdaptiveVSync = m_userPrefs.GetOrDefault("graphics", "use_adaptive_vsync", false);

        if (enableVSync)
        {
            if (useAdaptiveVSync)
            {
                graphics::SetPresentationMode(graphics::PresentationMode::AdaptiveVSync);
            }
            else
            {
                graphics::SetPresentationMode(graphics::PresentationMode::VSync);
            }
        }
        else
        {
            graphics::SetPresentationMode(graphics::PresentationMode::Immediate);
        }

        Log::EngineInfo("Graphics backend initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseRenderer(const CreateInfo& createInfo) -> Status
    {
        m_camera.Initialise(createInfo.graphicsInfo.cameraHalfSize, m_window.GetSize());
        
        m_renderer.Initialise(graphics::Renderer::CreateInfo{
            .window = &m_window,
            .camera = &m_camera,

            .shadersDirectoryPath = createInfo.virtualFilesystemInfo.shadersDirectoryPath,

            .maxShapesPerBatch = createInfo.graphicsInfo.maxShapesPerBatch,
            .maxTextureSlotsPerBatch = createInfo.graphicsInfo.maxTextureSlotsPerBatch,
        });

        if (!m_renderer.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["renderer"],
                m_locale["engine"]["errors"]["bodies"]["renderer"],
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to create renderer.");

            return Status::Fail;
        }

        Log::EngineInfo("Renderer and camera created. Camera pixels per unit is {}.", m_camera.GetPixelsPerUnit());

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseAudio(const CreateInfo&) -> Status
    {
        m_soundSystem.Initialise();

        if (!m_soundSystem.IsValid())
        {
            message_box::Show(
                m_locale["engine"]["errors"]["titles"]["audio"],
                m_locale({ "engine", "errors", "bodies", "sound-system" }, {
                    { "SOLOUD_ERROR", m_soundSystem.GetErrorString() },
                }),
                message_box::Type::Error
            );
            Log::EngineCritical("Failed to initialise sound system: {}.", m_soundSystem.GetErrorString());

            return Status::Fail;
        }

        m_volumeManager.SetVolume(
            audio::VolumeManager::GetMasterVolumeName(),
            std::clamp(m_userPrefs.GetOrDefault("audio", "master_volume", 1.0f), 0.0f, 1.0f)
        );

        Log::EngineInfo("Sound system initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseScriptEngine(const CreateInfo&) -> Status
    {
        m_scriptEngine.Initialise(*this);
        Log::EngineInfo("Lua script engine initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialisePhysics(const CreateInfo& createInfo) -> Status
    {
        m_timestepController.SetFixedTimestep(createInfo.physicsInfo.fixedTimestep);

        physics::World::SetPositionIterations(createInfo.physicsInfo.positionIterations);
        physics::World::SetVelocityIterations(createInfo.physicsInfo.velocityIterations);

        Log::EngineInfo("Physics subsystem initialised.");

        return Status::Success;
    }

    [[nodiscard]] auto Application::InitialiseInput(const CreateInfo& createInfo) -> Status
    {
        m_inputManager.Initialise(m_inputController);

        constexpr f32 DefaultGameControllerDeadzone = 0.1875f;
        m_inputController.SetGameControllerDeadzone(m_userPrefs.GetOrDefault("input", "game_controller_deadzone", DefaultGameControllerDeadzone));

        constexpr f32 DefaultJoystickDeadzone = 0.1875f;
        m_inputController.SetJoystickDeadzone(m_userPrefs.GetOrDefault("input", "joystick_deadzone", DefaultJoystickDeadzone));

        if (InputController::LoadGameControllerDatabase(createInfo.virtualFilesystemInfo.gameControllerDatabasePath) != Status::Success)
        {
            message_box::ShowComplex(
                m_locale["engine"]["warnings"]["titles"]["game-controller"],
                m_locale["engine"]["warnings"]["bodies"]["game-controller-database"],
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
            Log::EngineWarn("Failed to load game controller database; some game controllers may not work properly.");
        }

        const String controlPrefsFileLocation =
            filesystem::GetApplicationPreferenceDirectory() +
            createInfo.preferenceInfo.preferencesRelativeDirectory +
            filesystem::GetDirectorySeparator() +
            createInfo.preferenceInfo.controlPrefsFilename;

        m_controlPrefs.Initialise(controlPrefsFileLocation);

        if (m_controlPrefs.MergeControlsFromJSONFile(createInfo.virtualFilesystemInfo.defaultControlsPath) != Status::Success)
        {
            message_box::ShowComplex(
                m_locale["engine"]["warnings"]["titles"]["controls"],
                m_locale["engine"]["warnings"]["bodies"]["default-controls"],
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
            Log::EngineError("Failed to load default controls at path {}.", createInfo.virtualFilesystemInfo.defaultControlsPath);
        }

        if (filesystem::DoesPathExist(controlPrefsFileLocation))
        {
            if (m_controlPrefs.MergeControlsFromMessagePackFile(controlPrefsFileLocation, false) != Status::Success)
            {
                message_box::ShowComplex(
                    m_locale["engine"]["warnings"]["titles"]["controls"],
                    m_locale["engine"]["warnings"]["bodies"]["user-controls"],
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
                Log::EngineWarn("Failed to load user controls at path {}.", controlPrefsFileLocation);
            }
        }

        if (m_controlPrefs.Save() != Status::Success)
        {
            Log::EngineWarn("Failed to save control preferences.");
        }

        Log::EngineInfo("Input subsystem initialised.");

        return Status::Success;
    }

    auto Application::InitialiseScenes() -> void
    {
        if (m_didInitialiseSuccessfully)
        {
            m_isRunning.store(!m_sceneManager.IsEmpty(), std::memory_order::relaxed);

            if (m_isRunning.load(std::memory_order::relaxed))
            {
                if (m_sceneManager.CurrentScene()->OnLoad() != Status::Success)
                {
                    message_box::Show(
                        m_locale["engine"]["errors"]["titles"]["scene"],
                        m_locale({ "engine", "errors", "bodies", "initial-scene" }, {
                            { "SCENE_NAME", m_sceneManager.CurrentScene()->GetName() },
                        }),
                        message_box::Type::Error
                    );

                    Log::EngineError("Failed to load initial scene {}.", m_sceneManager.CurrentScene()->GetName());
                    m_isRunning.store(false, std::memory_order::relaxed);
                }
                else
                {
                    Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
                }
            }
        }
        else
        {
            m_isRunning.store(false, std::memory_order::relaxed);
        }
    }
}
