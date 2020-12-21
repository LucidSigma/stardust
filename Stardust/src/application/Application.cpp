#include "stardust/application/Application.h"

#include <chrono>
#include <utility>

#include <glad/glad.h>
#include <stb_image/stb_image_write.h>

#include "stardust/data/MathTypes.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/graphics/backend/OpenGL.h"
#include "stardust/vfs/VFS.h"

namespace stardust
{
	Application::Application(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Application::~Application() noexcept
	{
		m_entityRegistry.clear();

		m_openGLContext.Destroy();
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

	void Application::CaptureScreenshot() const
	{
		constexpr usize PixelChannelCount = 3u;
		const UVec2 screenshotSize = m_window.GetDrawableSize();

		Vector<ubyte> imageData(PixelChannelCount * screenshotSize.x * screenshotSize.y);

		glReadPixels(
			0, 0,
			static_cast<i32>(screenshotSize.x), static_cast<i32>(screenshotSize.y),
			GL_RGB, GL_UNSIGNED_BYTE,
			imageData.data()
		);

		const u64 currentTime = static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count());
		const String screenshotFilename = m_screenshotDirectory + "/screenshot_" + std::to_string(currentTime) + ".png";

		const i32 screenshotWriteResult = stbi_write_png(
			screenshotFilename.c_str(),
			static_cast<i32>(screenshotSize.x), static_cast<i32>(screenshotSize.y), static_cast<i32>(PixelChannelCount),
			imageData.data(),
			static_cast<i32>(screenshotSize.x * PixelChannelCount)
		);

		if (screenshotWriteResult == 0)
		{
			message_box::Show(
				std::string_view(m_locale["engine"]["warnings"]["titles"]["screenshot"]),
				std::string_view(m_locale["engine"]["warnings"]["bodies"]["screenshot"]),
				message_box::Type::Warning
			);
			Log::EngineWarn("Failed to take screenshot.");
		}
		else [[likely]]
		{
			Log::EngineTrace("Screenshot captured at {}.", screenshotFilename);
		}
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
		Log::Initialise(createInfo.applicationName, createInfo.filepaths.logFilepath);
	#endif

		Log::EngineInfo("Logger initialised.");
		Log::EngineDebug("Platform detected: \"{}\".", GetPlatformName());
		Log::EngineInfo("ECS initialised.");

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
		m_screenshotDirectory = createInfo.filepaths.screenshotDirectory;

		m_didInitialiseSuccessfully = true;
	}

	Status Application::InitialiseVFS(const CreateInfo& createInfo)
	{
		if (!vfs::Initialise(createInfo.filesystem.argv0))
		{
			message_box::Show("Filesystem Error", "Virtual filesystem failed to initialise.", message_box::Type::Error);
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
			message_box::Show("Config Error", "Config file is invalid.", message_box::Type::Error);
			Log::EngineError("Failed to load config file at {}.", createInfo.filepaths.configFilepath);

			return Status::Fail;
		}

		Log::EngineInfo("Config loaded.");

		return Status::Success;
	}

	Status Application::InitialiseLocale(const CreateInfo&)
	{
		m_locale.Initialise("locales");

		if (m_locale.SetLocale(m_config["locale"]) == Status::Fail)
		{
			message_box::Show("Locale Error", "Failed to load initial locale files.", message_box::Type::Error);
			Log::EngineError("Failed to load locale files for initial locale {}.", m_config["locale"]);

			return Status::Fail;
		}

		Log::EngineInfo("Locale \"{}\" loaded.", m_locale.GetCurrentLocaleName());

		return Status::Success;
	}

	Status Application::InitialiseSDL(const CreateInfo&)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["initialise"],
				m_locale["engine"]["errors"]["bodies"]["initialise-sdl"],
				message_box::Type::Error
			);
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
		SDL_GL_SetAttribute(
			SDL_GL_MULTISAMPLESAMPLES,
			m_config["graphics"]["multisampling"]["enabled"] 
				? static_cast<i32>(m_config["graphics"]["multisampling"]["sample-count"]) 
				: 1
		);
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
			.title = createInfo.windowTitle,
			.x = Window::Position::Centred,
			.y = Window::Position::Centred,
			.size = UVec2{ m_config["window"]["size"]["width"], m_config["window"]["size"]["height"] },
			.flags = std::move(windowCreateFlags),
		});

		if (!m_window.IsValid())
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["window"],
				m_locale["engine"]["errors"]["bodies"]["window"],
				message_box::Type::Error
			);
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
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["opengl"],
				m_locale["engine"]["errors"]["bodies"]["opengl-context"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to create OpenGL context: {}.", SDL_GetError());
		
			return Status::Fail;
		}
		
		if (m_openGLContext.MakeCurrent() != Status::Success)
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["opengl"],
				m_locale["engine"]["errors"]["bodies"]["opengl-current"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to set current OpenGL context: {}.", SDL_GetError());
		
			return Status::Fail;
		}

		if (m_config["frame-rate"]["vsync"]["enabled"])
		{
			const bool useAdaptiveVSync = m_config["frame-rate"]["vsync"]["adaptive"];
			bool didAdaptiveVSyncFail = false;

			if (useAdaptiveVSync)
			{
				if (SDL_GL_SetSwapInterval(-1) != 0)
				{
					Log::EngineWarn("Failed to set adaptive VSync; defaulting to regular VSync.");
					didAdaptiveVSyncFail = true;
				}
			}

			if (!useAdaptiveVSync || didAdaptiveVSyncFail)
			{
				if (SDL_GL_SetSwapInterval(1) != 0)
				{
					Log::EngineWarn("Failed to set VSync.");
					SDL_GL_SetSwapInterval(0);
				}
			}
		}
		else
		{
			SDL_GL_SetSwapInterval(0);
		}

		if (opengl::InitialiseLoader() != Status::Success)
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["opengl"],
				m_locale["engine"]["errors"]["bodies"]["opengl-load"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to load OpenGL functions.");

			return Status::Fail;
		}

	#ifndef NDEBUG
		opengl::InitialiseDebugCallback();
	#endif

		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		if (m_config["graphics"]["multisampling"]["enabled"])
		{
			glEnable(GL_MULTISAMPLE);
		}
		
		if (m_config["graphics"]["srgb-gamma"])
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
					message_box::Show(
						m_locale["engine"]["errors"]["titles"]["scene"],
						m_locale["engine"]["errors"]["bodies"]["initial-scene"],
						message_box::Type::Error
					);
					Log::EngineError("Failed to load initial scene {}.", m_sceneManager.CurrentScene()->GetName());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
				}
			}
			else [[unlikely]]
			{
				message_box::Show(
					m_locale["engine"]["warnings"]["titles"]["scene"],
					m_locale["engine"]["warnings"]["bodies"]["initial-scene"],
					message_box::Type::Warning
				);
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
	
		m_window.Present();
	}

	void Application::PollEvents(SDL_Event& event)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_F2:
					CaptureScreenshot();

					break;

				default:
					break;
				}

				break;

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
			m_window.ProcessResize(UVec2{ windowEvent.data1, windowEvent.data2 });

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
		m_deltaTime = static_cast<f32>(frameTicks) / static_cast<f32>(SDL_GetPerformanceFrequency());

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
			Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName());

			m_sceneManager.CurrentScene()->OnUnload();
			m_sceneManager.PopScene();
			m_entityRegistry.clear();
			// m_soundSystem.GetListener().Reset();

			if (!m_sceneManager.IsEmpty())
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					message_box::Show(
						m_locale["engine"]["errors"]["titles"]["scene"],
						m_locale["engine"]["errors"]["bodies"]["next-scene"],
						message_box::Type::Error
					);
					Log::EngineError("Failed to load scene {}.", m_sceneManager.CurrentScene()->GetName());
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
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