#include "stardust/application/Application.h"

namespace stardust
{
	Application::Application()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
	}

	Application::~Application() noexcept
	{
		SDL_Quit();
	}

	void Application::Run()
	{
		SDL_Event event{ };

		while (m_isRunning)
		{


			PollEvents(event);
		}
	}

	[[nodiscard]] String Application::GetPlatformName() const
	{
		return SDL_GetPlatform();
	}

	void Application::PollEvents(SDL_Event& event)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_isRunning = false;

				break;

			default:
				break;
			}
		}
	}
}