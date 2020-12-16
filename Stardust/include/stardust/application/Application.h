#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <atomic>

#include <SDL2/SDL.h>

#include "stardust/data/Types.h"

namespace stardust
{
	class Application final
	{
	private:
		bool m_didInitialiseSuccessfully = false;

		std::atomic_bool m_isRunning = true;

	public:
		Application();
		~Application() noexcept;

		void Run();

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		[[nodiscard]] String GetPlatformName() const;

		inline bool IsRunning() const noexcept { return m_isRunning; }

	private:
		void PollEvents(SDL_Event& event);
	};
}

#endif