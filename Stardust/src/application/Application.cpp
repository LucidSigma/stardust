#include "stardust/application/Application.h"

#include <functional>

#include "stardust/data/MathTypes.h"
#include "stardust/debug/logging/Log.h"

namespace stardust
{
	Application::Application(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Application::~Application() noexcept
	{
		m_window.Destroy();

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

	void Application::ForceQuit() noexcept
	{
		m_isRunning = false;
	}

	[[nodiscard]] String Application::GetPlatformName() const
	{
		return SDL_GetPlatform();
	}

	void Application::Initialise(const CreateInfo& createInfo)
	{
	#ifndef NDEBUG
		Log::Initialise(createInfo.logFilepath);
	#endif

		Log::EngineInfo("Logger initialised.");
		Log::EngineDebug("Platform detected: \"{}\".", GetPlatformName().cpp_str());

		static const Vector<std::function<Status(Application* const, const CreateInfo&)>> initialisationFunctions{
			//&Application::InitialiseVFS,
			//&Application::InitialiseConfig,
			//&Application::InitialiseLocale,
			//&Application::InitialiseSoundSystem,
			&Application::InitialiseSDL,
			&Application::InitialiseWindow,
			//&Application::InitialiseRenderer,
			//&Application::InitialiseTextSystem,
		};

		for (const auto& initialisationFunction : initialisationFunctions)
		{
			if (initialisationFunction(this, createInfo) != Status::Success)
			{
				return;
			}
		}

		m_didInitialiseSuccessfully = true;
	}

	Status Application::InitialiseSDL(const CreateInfo&)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			// message box
			Log::EngineCritical("Failed to initialise SDL: {}.", SDL_GetError());

			return Status::Fail;
		}

		Log::EngineInfo("SDL initialised.");

		return Status::Success;
	}

	Status Application::InitialiseWindow(const CreateInfo& createInfo)
	{
		// config here

		m_window.Initialise(Window::CreateInfo{
			.title = createInfo.title.data(),
			.x = Window::Position::Centred,
			.y = Window::Position::Centred,
			.size = UVec2{ 1280u, 720u }, //UVec2{ m_config["window"]["size"]["width"], m_config["window"]["size"]["height"] },
			.flags = { Window::CreateFlag::OpenGL } // std::move(windowCreateFlags),
		});

		if (!m_window.IsValid())
		{
			Log::EngineCritical("Failed to create window: {}.", SDL_GetError());

			return Status::Fail;
		}

		// Set window icon.
		Log::EngineInfo("Window created.");

		return Status::Success;
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

	void Application::ProcessWindowEvents(const SDL_WindowEvent& windowEvent)
	{
		switch (windowEvent.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_window.ProcessResize(glm::uvec2{ windowEvent.data1, windowEvent.data2 });

			break;

		case SDL_WINDOWEVENT_MINIMIZED:
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_hasWindowFocus = false;

			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_hasWindowFocus = true;

			// if (Input::IsMouseInRelativeMode())
			// {
			// 	Input::ClearRelativeMouseState();
			// }

			break;

		default:
			break;
		}
	}
}