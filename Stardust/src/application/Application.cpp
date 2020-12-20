#include "stardust/application/Application.h"

#include <functional>
#include <string_view>
#include <utility>

#include "stardust/data/MathTypes.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/vfs/VFS.h"

namespace stardust
{
	Application::Application(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Application::~Application() noexcept
	{
		m_window.Destroy();

		vfs::Quit();

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

		if (m_onExit.has_value())
		{
			m_onExit.value()(*this);
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

	Any& Application::GetFromGlobalSceneData(const String& dataName)
	{
		return m_globalSceneData[dataName];
	}

	void Application::RemoveFromGlobalSceneData(const String& dataName)
	{
		m_globalSceneData.erase(dataName);
	}

	void Application::Initialise(const CreateInfo& createInfo)
	{
	#ifndef NDEBUG
		Log::Initialise(createInfo.filepaths.logFilepath);
	#endif

		Log::EngineInfo("Logger initialised.");
		Log::EngineDebug("Platform detected: \"{}\".", GetPlatformName().c_str());

		static const Vector<std::function<Status(Application* const, const CreateInfo&)>> initialisationFunctions{
			&Application::InitialiseVFS,
			&Application::InitialiseConfig,
			&Application::InitialiseLocale,
			//&Application::InitialiseSoundSystem,
			&Application::InitialiseSDL,
			&Application::InitialiseWindow,
			&Application::InitialiseOpenGL,
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

		m_onInitialise = createInfo.initialiseCallback;
		m_onExit = createInfo.exitCallback;

		if (m_onInitialise.has_value())
		{
			if (m_onInitialise.value()(*this) != Status::Success)
			{
				return;
			}
		}

		m_fixedTimestep = createInfo.fixedTimestep;
		m_ticksCount = SDL_GetPerformanceCounter();

		m_didInitialiseSuccessfully = true;
	}

	Status Application::InitialiseVFS(const CreateInfo& createInfo)
	{
		if (!vfs::Initialise(createInfo.filesystem.argv0))
		{
			using namespace eastl::literals::string_view_literals;

			message_box::Show("Filesystem Error"sv, "Virtual filesystem failed to initialise."sv, message_box::Type::Error);
			Log::EngineError("Failed to initialise virtual filesystem.");

			return Status::Fail;
		}

		vfs::AddToSearchPath({ createInfo.filesystem.assetsArchive, createInfo.filesystem.localesArchive });
		Log::EngineInfo("Virtual filesystem initialised.");

		return Status::Success;
	}

	Status Application::InitialiseConfig(const CreateInfo& createInfo)
	{
		if (m_config.Initialise(createInfo.filepaths.configFilepath) == Status::Fail)
		{
			using namespace eastl::literals::string_view_literals;

			message_box::Show("Config Error"sv, "Config file is invalid."sv, message_box::Type::Error);
			Log::EngineError("Failed to load config file at {}.", createInfo.filepaths.configFilepath.data());

			return Status::Fail;
		}

		Log::EngineInfo("Config loaded.");

		return Status::Success;
	}

	Status Application::InitialiseLocale(const CreateInfo&)
	{
		m_locale.Initialise("locales");

		if (m_locale.SetLocale(std::string(m_config["locale"]).c_str()) == Status::Fail)
		{
			using namespace eastl::literals::string_view_literals;

			message_box::Show("Locale Error"sv, "Failed to load initial locale files."sv, message_box::Type::Error);
			Log::EngineError("Failed to load locale files for initial locale {}.", m_config["locale"]);

			return Status::Fail;
		}

		Log::EngineInfo("Locale \"{}\" loaded.", m_locale.GetCurrentLocaleName().c_str());

		return Status::Success;
	}

	Status Application::InitialiseSDL(const CreateInfo&)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["initialise"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["initialise-sdl"]), message_box::Type::Error);
			Log::EngineCritical("Failed to initialise SDL: {}.", SDL_GetError());

			return Status::Fail;
		}

		Log::EngineInfo("SDL initialised.");

		return Status::Success;
	}

	Status Application::InitialiseWindow(const CreateInfo& createInfo)
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8); // Use config.
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	#if defined(__APPLE__) && !defined(NDEBUG)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	#elif defined(__APPLE__)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	#elif !defined(NDEBUG)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	#endif

		Window::SetMinimiseOnFullscreenFocusLoss(m_config["graphics"]["enable-fullscreen-minimise"]);

		Vector<Window::CreateFlag> windowCreateFlags{ Window::CreateFlag::AllowHighDPI, Window::CreateFlag::OpenGL, Window::CreateFlag::Shown };

		if (m_config["window"]["fullscreen"])
		{
			windowCreateFlags.push_back(Window::CreateFlag::HardFullscreen);

			if (m_config["window"]["borderless"])
			{
				windowCreateFlags.push_back(Window::CreateFlag::Borderless);
			}
		}

		m_window.Initialise(Window::CreateInfo{
			.title = createInfo.title.data(),
			.x = Window::Position::Centred,
			.y = Window::Position::Centred,
			.size = UVec2{ m_config["window"]["size"]["width"], m_config["window"]["size"]["height"] },
			.flags = std::move(windowCreateFlags),
		});

		if (!m_window.IsValid())
		{
			message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["window"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["window"]), message_box::Type::Error);
			Log::EngineCritical("Failed to create window: {}.", SDL_GetError());

			return Status::Fail;
		}

		m_window.SetIcon(createInfo.filepaths.windowIconFile, m_locale);
		Log::EngineInfo("Window created.");

		return Status::Success;
	}

	Status Application::InitialiseOpenGL(const CreateInfo&)
	{
		m_openGLContext.Initialise(m_window);

		if (!m_openGLContext.IsValid())
		{
			message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["opengl"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["opengl-context"]), message_box::Type::Error);
			Log::EngineCritical("Failed to create OpenGL context: {}.", SDL_GetError());

			return Status::Fail;
		}

		if (m_openGLContext.MakeCurrent() != Status::Success)
		{
			message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["opengl"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["opengl-current"]), message_box::Type::Error);
			Log::EngineCritical("Failed to set current OpenGL context: {}.", SDL_GetError());

			return Status::Fail;
		}

		if (OpenGLContext::InitialiseLoader() != Status::Success)
		{
			message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["opengl"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["opengl-load"]), message_box::Type::Error);
			Log::EngineCritical("Failed to load OpenGL functions.");

			return Status::Fail;
		}

	#ifndef NDEBUG
		OpenGLContext::InitialiseDebugCallback();
	#endif

		Log::EngineInfo("OpenGL set up successfully.");

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
					message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["scene"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["initial-scene"]), message_box::Type::Error);
					Log::EngineError("Failed to load initial scene {}.", m_sceneManager.CurrentScene()->GetName().c_str());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName().c_str());
				}
			}
			else [[unlikely]]
			{
				message_box::Show(std::string_view(m_locale["engine"]["warnings"]["titles"]["scene"]), std::string_view(m_locale["engine"]["warnings"]["bodies"]["initial-scene"]), message_box::Type::Warning);
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
		static const bool capFramerate = m_config["frame-rate"]["cap-fps"];
		static const f32 fpsLimit = m_config["frame-rate"]["fps-limit"];

		const u64 newTicks = SDL_GetPerformanceCounter();
		const u64 frameTicks = newTicks - m_ticksCount;
		m_deltaTime = static_cast<float>(frameTicks) / static_cast<float>(SDL_GetPerformanceFrequency());

		if (capFramerate)
		{
			static const f32 timeToWait = 1.0f / fpsLimit;
		
			if (m_deltaTime < timeToWait)
			{
				constexpr f32 MillisecondsPerSecond = 1'000.0f;
		
				SDL_Delay(static_cast<u32>((timeToWait - m_deltaTime) * MillisecondsPerSecond));
			}
		
			m_deltaTime = timeToWait;
		}

		m_ticksCount = newTicks;
	}

	void Application::UpdateSceneQueue()
	{
		if (m_isCurrentSceneFinished)
		{
			Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName().c_str());

			m_sceneManager.CurrentScene()->OnUnload();
			m_sceneManager.PopScene();
			// m_entityRegistry.clear();
			// m_soundSystem.GetListener().Reset();

			if (!m_sceneManager.IsEmpty())
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					message_box::Show(std::string_view(m_locale["engine"]["errors"]["titles"]["scene"]), std::string_view(m_locale["engine"]["errors"]["bodies"]["next-scene"]), message_box::Type::Error);
					Log::EngineError("Failed to load scene {}.", m_sceneManager.CurrentScene()->GetName().c_str());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName().c_str());
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