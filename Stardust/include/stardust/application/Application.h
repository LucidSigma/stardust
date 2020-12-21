#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <atomic>
#include <functional>

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "stardust/config/Config.h"
#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/backend/OpenGLContext.h"
#include "stardust/locale/Locale.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/utility/enums/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
	class Application final
		: private INoncopyable, private INonmovable
	{
	public:
		using InitialiseCallback = std::function<Status(Application&)>;
		using ExitCallback = std::function<void(Application&)>;

		struct FilesystemInfo
		{
			const char* argv0;

			StringView assetsArchive;
			StringView localesArchive;
		};

		struct FilepathsInfo
		{
			StringView configFilepath;
			StringView logFilepath;
			StringView screenshotDirectory;
			StringView windowIconFile;
		};

		struct CreateInfo
		{
			StringView applicationName;
			StringView windowTitle;

			FilesystemInfo filesystem;
			FilepathsInfo filepaths;

			f32 fixedTimestep;

			Optional<InitialiseCallback> initialiseCallback;
			Optional<ExitCallback> exitCallback;
		};

	private:
		bool m_didInitialiseSuccessfully = false;

		Config m_config;
		Locale m_locale;

		Window m_window;
		OpenGLContext m_openGLContext;
		bool m_hasWindowFocus = true;

		std::atomic_bool m_isRunning = true;

		f32 m_fixedTimestep = 0.0f;
		f32 m_deltaTime = 0.0f;
		u64 m_ticksCount = 0u;
		f32 m_elapsedTime = 0.0f;

		SceneManager m_sceneManager;
		bool m_isCurrentSceneFinished = false;

		entt::registry m_entityRegistry{ };
		HashMap<String, Any> m_globalSceneData{ };

		String m_screenshotDirectory;

		Optional<ExitCallback> m_onExit = NullOpt;
		Optional<InitialiseCallback> m_onInitialise = NullOpt;

	public:
		Application(const CreateInfo& createInfo);
		~Application() noexcept;

		void Run();

		void FinishCurrentScene() noexcept;
		void ForceQuit() noexcept;

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		inline Config& GetConfig() noexcept { return m_config; }
		inline Locale& GetLocale() noexcept { return m_locale; }

		inline Window& GetWindow() noexcept { return m_window; }
		inline SceneManager& GetSceneManager() noexcept { return m_sceneManager; }
		inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }

		[[nodiscard]] String GetPlatformName() const;

		inline bool IsRunning() const noexcept { return m_isRunning; }
		inline u64 GetTicksCount() const noexcept { return m_ticksCount; }
		inline f32 GetElapsedTime() const noexcept { return m_elapsedTime; }
		inline bool HasWindowFocus() const noexcept { return m_hasWindowFocus; }

		void CaptureScreenshot() const;

		template <typename T>
		void SetGlobalSceneData(const String& dataName, const T& data)
		{
			m_globalSceneData[dataName] = data;
		}

		Any& GetFromGlobalSceneData(const String& dataName);
		void RemoveFromGlobalSceneData(const String& dataName);

		inline HashMap<String, Any>& GetGlobalSceneData() noexcept { return m_globalSceneData; }

	private:
		void Initialise(const CreateInfo& createInfo);
		Status InitialiseVFS(const CreateInfo& createInfo);
		Status InitialiseConfig(const CreateInfo& createInfo);
		Status InitialiseLocale(const CreateInfo&);
		Status InitialiseSDL(const CreateInfo&);
		Status InitialiseWindow(const CreateInfo& createInfo);
		Status InitialiseOpenGL(const CreateInfo&);
		void InitialiseScenes();

		void FixedUpdate();
		void ProcessInput();
		void Update();
		void LateUpdate();
		void Render() const;

		void PollEvents(SDL_Event& event);
		void ProcessWindowEvents(const SDL_WindowEvent& windowEvent);

		void CalculateDeltaTime();
		void UpdateSceneQueue();
	};
}

#endif