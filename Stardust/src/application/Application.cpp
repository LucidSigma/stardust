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
		InitialiseScenes();

		SDL_Event event{ };
		f32 timeAccumulator = 0.0f;

		while (m_isRunning)
		{
			CalculateDeltaTime();
			m_elapsedTime += m_deltaTime;
			timeAccumulator += m_deltaTime;

			// m_soundSystem.Update();

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

		m_fixedTimestep = createInfo.fixedTimestep;

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

	void Application::InitialiseScenes()
	{
		if (m_didInitialiseSuccessfully)
		{
			m_isRunning = !m_sceneManager.IsEmpty();

			if (m_isRunning)
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					// message_box::Show(m_locale["errors"]["titles"]["scene"], m_locale["errors"]["bodies"]["initial-scene"], message_box::Type::Error);
					Log::EngineError("Failed to load initial scene {}.", m_sceneManager.CurrentScene()->GetName().cpp_str());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName().cpp_str());
				}
			}
			else [[unlikely]]
			{
				// message_box::Show(m_locale["warnings"]["titles"]["scene"], m_locale["warnings"]["bodies"]["initial-scene"], message_box::Type::Warning);
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
		m_sceneManager.CurrentScene()->FixedUpdate(m_fixedTimestep);
	}

	void Application::ProcessInput()
	{
		if (m_hasWindowFocus)
		{
			// Input::UpdateKeyboardState();
			// Input::UpdateMouseState();
			// Input::UpdateGameControllers();
		}

		m_sceneManager.CurrentScene()->ProcessInput();
	}

	void Application::Update()
	{
		m_sceneManager.CurrentScene()->Update(m_deltaTime);
	}

	void Application::LateUpdate()
	{
		m_sceneManager.CurrentScene()->LateUpdate(m_deltaTime);
	}

	void Application::Render() const
	{
		// m_renderer.Clear(colours::Black);
		// m_sceneManager.CurrentScene()->Render(m_renderer);
		// m_renderer.Present();
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

			m_sceneManager.CurrentScene()->PollEvent(event);
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

	void Application::CalculateDeltaTime()
	{
		// static const bool capFramerate = m_config["frame-rate"]["cap-fps"];
		// static const f32 fpsLimit = m_config["frame-rate"]["fps-limit"];

		const u64 newTicks = SDL_GetPerformanceCounter();
		const u64 frameTicks = newTicks - m_ticksCount;
		m_deltaTime = static_cast<float>(frameTicks) / static_cast<float>(SDL_GetPerformanceFrequency());

		//if (capFramerate)
		//{
		//	static const f32 timeToWait = 1.0f / fpsLimit;
		//
		//	if (m_deltaTime < timeToWait)
		//	{
		//		constexpr f32 MillisecondsPerSecond = 1'000.0f;
		//
		//		SDL_Delay(static_cast<u32>((timeToWait - m_deltaTime) * MillisecondsPerSecond));
		//	}
		//
		//	m_deltaTime = timeToWait;
		//}

		m_ticksCount = newTicks;
	}

	void Application::UpdateSceneQueue()
	{
		if (m_isCurrentSceneFinished)
		{
			Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName().cpp_str());
			m_sceneManager.CurrentScene()->OnUnload();
			m_sceneManager.PopScene();
			// m_entityRegistry.clear();
			// m_soundSystem.GetListener().Reset();

			if (!m_sceneManager.IsEmpty())
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					// message_box::Show(m_locale["errors"]["titles"]["scene"], m_locale["errors"]["bodies"]["next-scene"], message_box::Type::Error);
					Log::EngineError("Failed to load scene {}.", m_sceneManager.CurrentScene()->GetName().cpp_str());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName().cpp_str());
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