#pragma once
#ifndef STARDUST_APPLICATION_H
#define STARDUST_APPLICATION_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <atomic>
#include <chrono>
#include <functional>

#include <entt/entt.hpp>

#include "stardust/application/events/Events.h"
#include "stardust/application/AppConfig.h"
#include "stardust/audio/volume/VolumeManager.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/config/Config.h"
#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/backend/OpenGLContext.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/InputManager.h"
#include "stardust/locale/Locale.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/utility/status/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    class Application final
        : private INoncopyable, private INonmovable
    {
    public:
        using InitialiseCallback = std::function<Status(Application&)>;
        using ExitCallback = std::function<void(Application&)>;

        struct CreateInfo
        {
            String appTOMLRelativeFilepath;
            const char* argv0;

            Optional<InitialiseCallback> initialiseCallback;
            Optional<ExitCallback> exitCallback;
        };

    private:
        bool m_didInitialiseSuccessfully = false;

        Config m_config;
        Locale m_locale;

        Window m_window;
        OpenGLContext m_openGLContext;
        Renderer m_renderer;
        bool m_hasWindowFocus = true;

        std::atomic_bool m_isRunning = true;

        Camera2D m_camera;

        f64 m_fixedTimestep = 0.0;
        f32 m_deltaTime = 0.0f;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_ticksCount;
        f64 m_elapsedTime = 0.0;

        InputManager m_inputManager;
        SceneManager m_sceneManager;
        bool m_isCurrentSceneFinished = false;

        entt::registry m_entityRegistry;
        ScriptEngine m_scriptEngine;
        audio::SoundSystem m_soundSystem;
        audio::VolumeManager m_volumeManager;

        HashMap<String, Any> m_globalSceneData{ };

        Optional<ExitCallback> m_onExit = NullOpt;
        Optional<InitialiseCallback> m_onInitialise = NullOpt;

    public:
        explicit Application(const CreateInfo& createInfo);
        ~Application() noexcept;

        void Run();

        void FinishCurrentScene() noexcept;
        void ForceQuit() noexcept;

        [[nodiscard]] inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

        [[nodiscard]] inline Config& GetConfig() noexcept { return m_config; }
        [[nodiscard]] inline Locale& GetLocale() noexcept { return m_locale; }

        [[nodiscard]] inline Window& GetWindow() noexcept { return m_window; }
        [[nodiscard]] inline const Window& GetWindow() const noexcept { return m_window; }
        [[nodiscard]] inline Renderer& GetRenderer() noexcept { return m_renderer; }
        [[nodiscard]] inline const Renderer& GetRenderer() const noexcept { return m_renderer; }
        [[nodiscard]] inline Camera2D& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] inline const Camera2D& GetCamera() const noexcept { return m_camera; }
        [[nodiscard]] inline InputManager& GetInputManager() noexcept { return m_inputManager; }
        [[nodiscard]] inline SceneManager& GetSceneManager() noexcept { return m_sceneManager; }

        [[nodiscard]] inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }
        [[nodiscard]] inline ScriptEngine& GetScriptEngine() noexcept { return m_scriptEngine; }
        [[nodiscard]] inline audio::SoundSystem& GetSoundSystem() noexcept { return m_soundSystem; }
        [[nodiscard]] inline audio::VolumeManager& GetVolumeManager() noexcept { return m_volumeManager; }

        [[nodiscard]] inline bool IsRunning() const noexcept { return m_isRunning; }
        [[nodiscard]] inline f64 GetElapsedTime() const noexcept { return m_elapsedTime; }
        [[nodiscard]] inline bool HasWindowFocus() const noexcept { return m_hasWindowFocus; }

        template <typename T>
        void SetGlobalSceneData(const String& dataName, const T& data)
        {
            m_globalSceneData[dataName] = data;
        }

        template <typename T>
        [[nodiscard]] T GetFromGlobalSceneData(const String& dataName)
        {
            return std::any_cast<T>(m_globalSceneData[dataName]);
        }

        void RemoveFromGlobalSceneData(const String& dataName);

        [[nodiscard]] inline HashMap<String, Any>& GetGlobalSceneData() noexcept { return m_globalSceneData; }

    private:
        [[nodiscard]] Status CreateApplicationConfig(const String& appTOMLFilepath, const char* argv0, AppConfig& out_appConfig);

        void Initialise(const CreateInfo& createInfo);
        [[nodiscard]] Status InitialiseFilesystem(const AppConfig& appConfig);
        [[nodiscard]] Status InitialiseConfig(const AppConfig& appConfig);
        [[nodiscard]] Status InitialiseLocale(const AppConfig&);
        [[nodiscard]] Status InitialiseSoundSystem(const AppConfig&);
        [[nodiscard]] Status InitialiseSDL(const AppConfig&);
        [[nodiscard]] Status InitialiseWindow(const AppConfig& appConfig);
        [[nodiscard]] Status InitialiseOpenGL(const AppConfig&);
        [[nodiscard]] Status InitialiseRenderer(const AppConfig&);
        [[nodiscard]] Status InitialiseTextSystem(const AppConfig&);
        [[nodiscard]] Status InitialiseUI(const AppConfig&);
        [[nodiscard]] Status InitialiseScriptEngine(const AppConfig&);
        [[nodiscard]] Status InitialisePhysics(const AppConfig& appConfig);
        [[nodiscard]] Status InitialiseInput(const AppConfig& appConfig);
        void InitialiseScenes();

        void FixedUpdate();
        void ProcessInput();
        void Update();
        void LateUpdate();
        void Render();

        void PollEvents(Event& event);
        void ProcessWindowEvents(const WindowEvent& windowEvent);

        void UpdateTime(f64& timeAccumulator);
        void UpdateSceneQueue();
    };
}

#endif