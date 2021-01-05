#include "stardust/application/Application.h"

#include <chrono>
#include <string>
#include <utility>

#include <glad/glad.h>
#include <SDL2/SDL_ttf.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "stardust/data/MathTypes.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/backend/OpenGL.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/Input.h"

namespace stardust
{
	Application::Application(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Application::~Application() noexcept
	{
		m_entityRegistry.clear();
		m_globalSceneData.clear();

		Input::RemoveAllGameControllers();

		m_renderer.Destroy();
		m_openGLContext.Destroy();
		m_window.Destroy();

		vfs::Quit();

		TTF_Quit();
		SDL_Quit();
	}

	void Application::Run()
	{
		InitialiseScenes();

		SDL_Event event{ };
		f64 timeAccumulator = 0.0f;

		while (m_isRunning)
		{
			UpdateTime(timeAccumulator);
			m_soundSystem.Update();

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
		const Renderer::PixelReadData pixelReadData = m_renderer.ReadPixels();

		const u64 currentTime = static_cast<u64>(std::chrono::system_clock::now().time_since_epoch().count());
		const String screenshotFilename = m_screenshotDirectory + "/screenshot_" + std::to_string(currentTime) + ".png";

		const i32 screenshotWriteResult = stbi_write_png(
			screenshotFilename.c_str(),
			static_cast<i32>(pixelReadData.extent.x), static_cast<i32>(pixelReadData.extent.y), static_cast<i32>(pixelReadData.channelCount),
			pixelReadData.pixels.data(),
			static_cast<i32>(pixelReadData.extent.x * pixelReadData.channelCount)
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
		if (char* baseDirectory = SDL_GetBasePath();
			baseDirectory != nullptr)
		{
			m_baseDirectory = baseDirectory;

			SDL_free(baseDirectory);
			baseDirectory = nullptr;
		}

	#ifndef NDEBUG
		{
			const String logFilepath = m_baseDirectory + "log.txt";
			Log::Initialise(createInfo.applicationName, logFilepath);
		}
	#endif

		Log::EngineInfo("Logger initialised.");
		Log::EngineDebug("Platform detected: \"{}\".", GetPlatformName());
		Log::EngineDebug("Base directory: \"{}\"", m_baseDirectory);
		Log::EngineInfo("ECS initialised.");

		static const Vector<std::function<Status(Application* const, const CreateInfo&)>> initialisationFunctions{
			&Application::InitialiseFilesystem,
			&Application::InitialiseConfig,
			&Application::InitialiseLocale,
			&Application::InitialiseSoundSystem,
			&Application::InitialiseSDL,
			&Application::InitialiseWindow,
			&Application::InitialiseOpenGL,
			&Application::InitialiseRenderer,
			&Application::InitialiseTextSystem,
			&Application::InitialiseScriptEngine,
		};

		for (const auto& initialisationFunction : initialisationFunctions)
		{
			if (initialisationFunction(this, createInfo) != Status::Success)
			{
				return;
			}
		}

		Input::SetGameControllerDeadzone(m_config["input"]["controller-deadzone"]);

		m_onInitialise = createInfo.initialiseCallback;
		m_onExit = createInfo.exitCallback;

		if (m_onInitialise.has_value())
		{
			if (m_onInitialise.value()(*this) != Status::Success)
			{
				return;
			}
		}

		stbi_set_flip_vertically_on_load(true);
		stbi_flip_vertically_on_write(true);
		
		m_fixedTimestep = createInfo.fixedTimestep;
		m_ticksCount = SDL_GetPerformanceCounter();

		m_didInitialiseSuccessfully = true;
	}

	[[nodiscard]] Status Application::InitialiseFilesystem(const CreateInfo& createInfo)
	{
		if (m_baseDirectory.empty())
		{
			Log::EngineWarn("Failed to get base directory.");
		}

		if (char* preferenceDirectory = SDL_GetPrefPath(createInfo.organisationName.data(), createInfo.applicationName.data());
			preferenceDirectory != nullptr)
		{
			m_preferenceDirectory = preferenceDirectory;

			SDL_free(preferenceDirectory);
			preferenceDirectory = nullptr;
		}
		else
		{
			message_box::Show("Filesystem Error", "Failed to get preference directory.", message_box::Type::Error);
			Log::EngineError("Failed to get preference directory.");

			return Status::Fail;
		}

		m_screenshotDirectory = m_preferenceDirectory + "screenshots";

		if (!filesystem::IsDirectory(m_screenshotDirectory))
		{
			if (filesystem::CreateDirectory(m_screenshotDirectory) == Status::Fail)
			{
				Log::EngineWarn("Failed to create screenshot directory.");
			}
		}

		Log::EngineInfo("Filesystem initialised.");

		if (!vfs::Initialise(createInfo.filesystem.argv0))
		{
			message_box::Show("Filesystem Error", "Virtual filesystem failed to initialise.", message_box::Type::Error);
			Log::EngineError("Failed to initialise virtual filesystem.");

			return Status::Fail;
		}

		const String assetsFilepath = m_baseDirectory + String(createInfo.filesystem.assetsArchive);
		const String localesFilepath = m_baseDirectory + String(createInfo.filesystem.localesArchive);

		vfs::AddToSearchPath({
			assetsFilepath,
			localesFilepath,
		});

		Log::EngineInfo("Virtual filesystem initialised.");

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseConfig(const CreateInfo& createInfo)
	{
		if (m_config.Initialise(m_preferenceDirectory, createInfo.filepaths.defaultConfigFilepath) == Status::Fail)
		{
			message_box::Show("Config Error", "Failed to load config file.", message_box::Type::Error);
			Log::EngineError("Failed to load config file.");

			return Status::Fail;
		}

		Log::EngineInfo("Config loaded.");

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseLocale(const CreateInfo&)
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

	[[nodiscard]] Status Application::InitialiseSoundSystem(const CreateInfo&)
	{
		if (!m_soundSystem.DidInitialiseSuccessfully())
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["sound"],
				m_locale["engine"]["errors"]["bodies"]["sound"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to initialise sound system.");

			return Status::Fail;
		}

		m_volumeManager.AddVolume(VolumeManager::GetMasterVolumeName(), m_config["audio"]["volumes"]["master"]);
		Log::EngineInfo("Sound system initialised.");

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseSDL(const CreateInfo&)
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

	[[nodiscard]] Status Application::InitialiseWindow(const CreateInfo& createInfo)
	{
		const Vector<Pair<SDL_GLattr, i32>> glWindowAttributes{
			{ SDL_GL_DOUBLEBUFFER, SDL_TRUE },
			{ SDL_GL_CONTEXT_MAJOR_VERSION, 4 },
			{ SDL_GL_CONTEXT_MINOR_VERSION, 6 },
			{ SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE },
			{ SDL_GL_ACCELERATED_VISUAL, SDL_TRUE },

			{ SDL_GL_RED_SIZE, 8 },
			{ SDL_GL_GREEN_SIZE, 8 },
			{ SDL_GL_BLUE_SIZE, 8 },
			{ SDL_GL_ALPHA_SIZE, 8 },
			{ SDL_GL_DEPTH_SIZE, 24 },
			{ SDL_GL_STENCIL_SIZE, 8 },

			{ SDL_GL_MULTISAMPLEBUFFERS, 1 },
			{ SDL_GL_MULTISAMPLESAMPLES, 4 },

		#if defined(__APPLE__) && !defined(NDEBUG)
			{ SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
		#elif defined(__APPLE__)
			{ SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
		#elif !defined(NDEBUG)
			{ SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG },
		#endif
		};

		for (const auto [attribute, value] : glWindowAttributes)
		{
			SDL_GL_SetAttribute(attribute, value);
		}

		Window::SetMinimiseOnFullscreenFocusLoss(m_config["graphics"]["window"]["enable-fullscreen-minimise"]);

		Vector<Window::CreateFlag> windowCreateFlags{ 
			Window::CreateFlag::AllowHighDPI,
			Window::CreateFlag::OpenGL,
			Window::CreateFlag::Shown,
			Window::CreateFlag::Resizable,
		};

		if (m_config["graphics"]["window"]["fullscreen"])
		{
			windowCreateFlags.push_back(Window::CreateFlag::HardFullscreen);
		}

		if (m_config["graphics"]["window"]["borderless"])
		{
			windowCreateFlags.push_back(Window::CreateFlag::Borderless);
		}

		m_window.Initialise(Window::CreateInfo{
			.title = createInfo.windowTitle,
			.x = Window::Position::Centred,
			.y = Window::Position::Centred,
			.size = UVec2{ m_config["graphics"]["window"]["size"]["width"], m_config["graphics"]["window"]["size"]["height"] },
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

	[[nodiscard]] Status Application::InitialiseOpenGL(const CreateInfo&)
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
				if (Window::SetVSync(VSyncType::Adaptive) != Status::Success)
				{
					Log::EngineWarn("Failed to set adaptive VSync; defaulting to regular VSync.");
					didAdaptiveVSyncFail = true;
				}
			}

			if (!useAdaptiveVSync || didAdaptiveVSyncFail)
			{
				if (Window::SetVSync(VSyncType::Regular) != Status::Success)
				{
					Log::EngineWarn("Failed to set VSync.");
					Window::SetVSync(VSyncType::None);
				}
			}
		}
		else
		{
			Window::SetVSync(VSyncType::None);
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

		Log::EngineInfo("OpenGL set up successfully.");

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseRenderer(const CreateInfo&)
	{
		m_renderer.Initialise(Renderer::CreateInfo{
			.window = &m_window,
			.virtualSize = UVec2{
				m_config["graphics"]["resolution"]["width"],
				m_config["graphics"]["resolution"]["height"],
			},
		});

		if (!m_renderer.IsValid())
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["renderer"],
				m_locale["engine"]["errors"]["bodies"]["renderer"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to initialise renderer.");

			return Status::Fail;
		}

		m_renderer.SetAntiAliasing(m_config["graphics"]["anti-aliasing"]);
		m_renderer.SetClearColour(colours::Black);

		m_camera.Initialise(8.0f, m_renderer);
		Log::EngineInfo("Renderer and camera created. Camera pixels per unit is {}.", m_camera.GetPixelsPerUnit());

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseTextSystem(const CreateInfo&)
	{
		if (TTF_Init() != 0)
		{
			message_box::Show(
				m_locale["engine"]["errors"]["titles"]["ttf"],
				m_locale["engine"]["errors"]["bodies"]["ttf"],
				message_box::Type::Error
			);
			Log::EngineCritical("Failed to initialise SDL_TTF: {}.", TTF_GetError());

			return Status::Fail;
		}

		Log::EngineInfo("Text subsystem initialised.");

		return Status::Success;
	}

	[[nodiscard]] Status Application::InitialiseScriptEngine(const CreateInfo&)
	{
		m_scriptEngine.Initialise(*this);
		Log::EngineInfo("Lua script engine initialised.");

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
		m_sceneManager.CurrentScene()->FixedUpdate(static_cast<f32>(m_fixedTimestep));
	}

	void Application::ProcessInput()
	{
		Input::UpdateKeyboardState();
		Input::UpdateMouseState();
		Input::UpdateGameControllers();

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

	void Application::Render()
	{
		m_renderer.Clear();
		m_renderer.BeginFrame();

		m_sceneManager.CurrentScene()->Render(m_renderer);

		m_renderer.EndFrame(m_camera);
		m_window.Present();
	}

	void Application::PollEvents(SDL_Event& event)
	{
		Input::ResetScrollState();

		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				ProcessWindowEvents(event.window);

				break;

			case SDL_MOUSEWHEEL:
				Input::UpdateScrollState(event.wheel.y);

				break;

			case SDL_CONTROLLERDEVICEADDED:
				if (GameController* const gameController = Input::AddGameController(event.cdevice.which, m_locale);
					gameController != nullptr)
				{
					m_sceneManager.CurrentScene()->OnGameControllerAdded(*gameController);
				}
				
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				if (GameController* const gameController = Input::GetGameController(event.cdevice.which);
					gameController != nullptr)
				{
					m_sceneManager.CurrentScene()->OnGameControllerRemoved(*gameController);
				}

				Input::RemoveGameController(event.cdevice.which);

				break;

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
			m_renderer.ProcessResize();
			m_camera.Refresh();

			break;

		case SDL_WINDOWEVENT_MINIMIZED:
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_hasWindowFocus = false;

			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_hasWindowFocus = true;

			if (Input::IsMouseInRelativeMode())
			{
				Input::ClearRelativeMouseState();
			}

			break;

		default:
			break;
		}
	}

	void Application::UpdateTime(f64& timeAccumulator)
	{
		static const bool capFramerate = m_config["frame-rate"]["cap-fps"];

		const u64 newTicks = SDL_GetPerformanceCounter();
		const u64 frameTicks = newTicks - m_ticksCount;
		const f64 preciseDeltaTime = static_cast<f64>(frameTicks) / static_cast<f64>(SDL_GetPerformanceFrequency());
		m_deltaTime = static_cast<f32>(preciseDeltaTime);

		if (capFramerate)
		{
			static const f32 fpsLimit = m_config["frame-rate"]["fps-limit"];
			static const f32 timeToWait = 1.0f / fpsLimit;
		
			if (m_deltaTime < timeToWait)
			{
				constexpr f32 MillisecondsPerSecond = 1'000.0f;
		
				SDL_Delay(static_cast<u32>((timeToWait - m_deltaTime) * MillisecondsPerSecond));
			}
		
			m_deltaTime = timeToWait;
		}

		m_ticksCount = newTicks;
		m_elapsedTime += preciseDeltaTime;
		timeAccumulator += preciseDeltaTime;
	}

	void Application::UpdateSceneQueue()
	{
		if (m_isCurrentSceneFinished)
		{
			Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName());

			m_sceneManager.CurrentScene()->OnUnload();
			m_sceneManager.PopScene();

			m_renderer.SetClearColour(colours::Black);
			m_camera.ResetTransform();
			m_entityRegistry.clear();
			m_soundSystem.GetListener().Reset();

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