#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <EASTL/atomic.h>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/utility/enums/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
	class Application final
		: private INoncopyable, private INonmovable
	{
	public:
		struct CreateInfo
		{
			String title;
			String logFilepath;

			f32 fixedTimestep;
		};

	private:
		bool m_didInitialiseSuccessfully = false;

		Window m_window;
		bool m_hasWindowFocus = true;

		eastl::atomic<bool> m_isRunning = true;

		f32 m_fixedTimestep = 0.0f;
		f32 m_deltaTime = 0.0f;
		u64 m_ticksCount = 0u;
		f32 m_elapsedTime = 0.0f;

		SceneManager m_sceneManager;
		bool m_isCurrentSceneFinished = false;

	public:
		Application(const CreateInfo& createInfo);
		~Application() noexcept;

		void Run();

		void FinishCurrentScene() noexcept;
		void ForceQuit() noexcept;

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		inline Window& GetWindow() noexcept { return m_window; }
		inline SceneManager& GetSceneManager() noexcept { return m_sceneManager; }

		[[nodiscard]] String GetPlatformName() const;

		inline bool IsRunning() const noexcept { return m_isRunning; }
		inline u64 GetTicksCount() const noexcept { return m_ticksCount; }
		inline f32 GetElapsedTime() const noexcept { return m_elapsedTime; }
		inline bool HasWindowFocus() const noexcept { return m_hasWindowFocus; }

	private:
		void Initialise(const CreateInfo& createInfo);
		Status InitialiseSDL(const CreateInfo&);
		Status InitialiseWindow(const CreateInfo& createInfo);
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