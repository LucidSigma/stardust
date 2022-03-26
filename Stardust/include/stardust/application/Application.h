#pragma once
#ifndef STARDUST_APPLICATION_H
#define STARDUST_APPLICATION_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <atomic>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include <SDL2/SDL.h>

#include "stardust/application/events/Events.h"
#include "stardust/application/events/GlobalEventHandler.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/audio/volume/VolumeManager.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/registry/EntityRegistry.h"
#include "stardust/graphics/backend/OpenGLContext.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/InputController.h"
#include "stardust/input/InputManager.h"
#include "stardust/locale/Locale.h"
#include "stardust/preferences/ControlPrefs.h"
#include "stardust/preferences/UserPrefs.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/scene/resources/GlobalResources.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/time/timestep/TimestepController.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    class Application final
        : private INoncopyable, private INonmovable
    {
    public:
        using InitialiseCallback = std::function<auto(Application&) -> Status>;
        using ExitCallback = std::function<auto(Application&) -> void>;

        struct CreateInfo final
        {
            struct ApplicationInfo final
            {
                String applicationName;
                String organisationName;
            } applicationInfo;

            struct WindowInfo final
            {
                String title;
                bool isResizable;
            } windowInfo;

            struct FilesystemInfo final
            {
                String assetsArchiveRelativeFilepath;
                String localesArchiveRelativeFilepath;
                String scriptsArchiveRelativeFilepath;

                String logFileRelativeFilepath;
            } filesystemInfo;

            struct VirtualFilesystemInfo final
            {
                String windowIconPath;

                String gameControllerDatabasePath;
                String defaultControlsPath;

                String shadersDirectoryPath;
            } virtualFilesystemInfo;

            struct PreferenceInfo final
            {
                String preferencesRelativeDirectory;
                String userPrefsFilename;
                String controlPrefsFilename;
            } preferenceInfo;

            struct GraphicsInfo final
            {
                f32 cameraHalfSize;

                usize maxShapesPerBatch;
                usize maxTextureSlotsPerBatch;
            } graphicsInfo;

            struct PhysicsInfo final
            {
                f64 fixedTimestep;

                u32 positionIterations;
                u32 velocityIterations;
            } physicsInfo;

            const char* argv0;

            Optional<InitialiseCallback> initialiseCallback;
            Optional<ExitCallback> exitCallback;
        };

    private:
        bool m_didInitialiseSuccessfully = false;

        UserPrefs m_userPrefs;
        ControlPrefs m_controlPrefs;

        bool m_didUserPrefsFileExist = true;
        Locale m_locale;

        Window m_window;
        bool m_hasWindowFocus = true;
        opengl::Context m_openGLContext;
        graphics::Renderer m_renderer;
        Camera2D m_camera;

        audio::SoundSystem m_soundSystem;
        audio::VolumeManager m_volumeManager;

        std::atomic_bool m_isRunning = true;
        Queue<events::UserEvent> m_userEvents{ };

        TimestepController m_timestepController;
        f64 m_elapsedTime = 0.0;

        InputController m_inputController;
        InputManager m_inputManager;

        SceneManager m_sceneManager;
        EntityRegistry m_entityRegistry;
        GlobalResources m_globalSceneResources{ };
        ScriptEngine m_scriptEngine;

        Optional<InitialiseCallback> m_onInitialise = None;
        Optional<ExitCallback> m_onExit = None;

        UniquePointer<GlobalEventHandler> m_globalEventHandler = nullptr;

    public:
        explicit Application(const CreateInfo& createInfo);
        ~Application() noexcept;

        auto Run() -> void;

        auto ForceQuit() noexcept -> void;
        auto PushUserEvent(const events::UserEvent& userEvent) -> void;

        template <std::derived_from<GlobalEventHandler> G, typename... Args>
            requires std::is_constructible_v<G, Args...>
        auto RegisterGlobalEventHandler(Args&&... args) -> void
        {
            m_globalEventHandler = std::make_unique<G>(std::forward<Args>(args)...);
        }

        inline auto DeregisterGlobalEventHandler() { m_globalEventHandler = nullptr; }

        [[nodiscard]] inline auto DidInitialiseSuccessfully() const noexcept -> bool { return m_didInitialiseSuccessfully; }

        [[nodiscard]] inline auto GetUserPrefs() noexcept -> UserPrefs& { return m_userPrefs; }
        [[nodiscard]] inline auto GetControlPrefs() noexcept -> ControlPrefs& { return m_controlPrefs; }
        [[nodiscard]] inline auto GetControlPrefs() const noexcept -> const ControlPrefs& { return m_controlPrefs; }
        [[nodiscard]] inline auto GetLocale() noexcept -> Locale& { return m_locale; }

        [[nodiscard]] inline auto GetWindow() noexcept -> Window& { return m_window; }
        [[nodiscard]] inline auto GetWindow() const noexcept -> const Window& { return m_window; }
        [[nodiscard]] inline auto HasWindowFocus() const noexcept -> bool { return m_hasWindowFocus; }
        [[nodiscard]] inline auto GetRenderer() noexcept -> graphics::Renderer& { return m_renderer; }
        [[nodiscard]] inline auto GetRenderer() const noexcept -> const graphics::Renderer& { return m_renderer; }
        [[nodiscard]] inline auto GetOpenGLContext() noexcept -> opengl::Context& { return m_openGLContext; }
        [[nodiscard]] inline auto GetOpenGLContext() const noexcept -> const opengl::Context& { return m_openGLContext; }
        [[nodiscard]] inline auto GetCamera() noexcept -> Camera2D& { return m_camera; }
        [[nodiscard]] inline auto GetCamera() const noexcept -> const Camera2D& { return m_camera; }

        [[nodiscard]] inline auto GetSoundSystem() noexcept -> audio::SoundSystem& { return m_soundSystem; }
        [[nodiscard]] inline auto GetSoundSystem() const noexcept -> const audio::SoundSystem& { return m_soundSystem; }
        [[nodiscard]] inline auto GetVolumeManager() noexcept -> audio::VolumeManager& { return m_volumeManager; }
        [[nodiscard]] inline auto GetVolumeManager() const noexcept -> const audio::VolumeManager& { return m_volumeManager; }

        [[nodiscard]] inline auto GetInputController() const noexcept -> const InputController& { return m_inputController; }
        [[nodiscard]] inline auto GetInputManager() noexcept -> InputManager& { return m_inputManager; }
        [[nodiscard]] inline auto GetInputManager() const noexcept -> const InputManager& { return m_inputManager; }

        [[nodiscard]] inline auto GetSceneManager() noexcept -> SceneManager& { return m_sceneManager; }
        [[nodiscard]] inline auto GetSceneManager() const noexcept -> const SceneManager& { return m_sceneManager; }
        [[nodiscard]] inline auto GetEntityRegistry() noexcept -> EntityRegistry& { return m_entityRegistry; }
        [[nodiscard]] inline auto GetEntityRegistry() const noexcept -> const EntityRegistry& { return m_entityRegistry; }
        [[nodiscard]] inline auto GetGlobalSceneResources() noexcept -> GlobalResources& { return m_globalSceneResources; }
        [[nodiscard]] inline auto GetGlobalSceneResources() const noexcept -> const GlobalResources& { return m_globalSceneResources; }
        [[nodiscard]] inline auto GetScriptEngine() noexcept -> ScriptEngine& { return m_scriptEngine; }
        [[nodiscard]] inline auto GetScriptEngine() const noexcept -> const ScriptEngine& { return m_scriptEngine; }

        [[nodiscard]] inline auto GetTimestepController() noexcept -> TimestepController& { return m_timestepController; }
        [[nodiscard]] inline auto GetTimestepController() const noexcept -> const TimestepController& { return m_timestepController; }
        [[nodiscard]] inline auto GetElapsedTime() const noexcept -> f64 { return m_elapsedTime; }

        [[nodiscard]] inline auto IsRunning() const noexcept -> bool { return m_isRunning.load(std::memory_order::relaxed); }

    private:
        auto Initialise(const CreateInfo& createInfo) -> void;
        [[nodiscard]] auto InitialiseFilesystem(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseUserPrefs(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseLocale(const CreateInfo&) -> Status;
        [[nodiscard]] auto InitialiseSDL(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseWindow(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseGraphics(const CreateInfo&) -> Status;
        [[nodiscard]] auto InitialiseRenderer(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseAudio(const CreateInfo&) -> Status;
        [[nodiscard]] auto InitialiseScriptEngine(const CreateInfo&) -> Status;
        [[nodiscard]] auto InitialisePhysics(const CreateInfo& createInfo) -> Status;
        [[nodiscard]] auto InitialiseInput(const CreateInfo& createInfo) -> Status;
        auto InitialiseScenes() -> void;

        auto FixedUpdate() -> void;
        auto ProcessInput() -> void;
        auto PreUpdate() -> void;
        auto Update() -> void;
        auto PostUpdate() -> void;
        auto Render() -> void;

        auto PollEvents(SDL_Event& event) -> void;
        auto ProcessWindowEvent(const SDL_WindowEvent& windowEvent) -> void;

        auto UpdateSceneQueue() -> void;
    };
}

#endif
