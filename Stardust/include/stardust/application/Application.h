#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <atomic>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
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
		};

	private:
		bool m_didInitialiseSuccessfully = false;

		Window m_window;
		bool m_hasWindowFocus = true;

		std::atomic_bool m_isRunning = true;

	public:
		Application(const CreateInfo& createInfo);
		~Application() noexcept;

		void Run();

		void ForceQuit() noexcept;

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		inline Window& GetWindow() noexcept { return m_window; }

		[[nodiscard]] String GetPlatformName() const;

		inline bool IsRunning() const noexcept { return m_isRunning; }
		inline bool HasWindowFocus() const noexcept { return m_hasWindowFocus; }

	private:
		void Initialise(const CreateInfo& createInfo);
		Status InitialiseSDL(const CreateInfo&);
		Status InitialiseWindow(const CreateInfo& createInfo);

		void PollEvents(SDL_Event& event);
		void ProcessWindowEvents(const SDL_WindowEvent& windowEvent);
	};
}

#endif