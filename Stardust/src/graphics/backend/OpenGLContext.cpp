#include "stardust/graphics/backend/OpenGLContext.h"

#include <utility>

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/debug/logging/Log.h"

namespace stardust
{
	Status OpenGLContext::InitialiseLoader()
	{
		return gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)) != 0
			? Status::Success
			: Status::Fail;
	}

	void OpenGLContext::InitialiseDebugCallback()
	{
		String vendorName(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		vendorName.make_lower();

		s_isNvidiaGPU = vendorName.find("nvidia") != String::npos;

		glDebugMessageCallback(DebugMessageCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	OpenGLContext::OpenGLContext()
	{ }

	OpenGLContext::OpenGLContext(const Window& window)
	{
		Initialise(window);
	}

	OpenGLContext::OpenGLContext(OpenGLContext&& other) noexcept
	{
		Destroy();
		std::swap(m_handle, other.m_handle);
		std::swap(m_window, other.m_window);
	}

	OpenGLContext& OpenGLContext::operator=(OpenGLContext&& other) noexcept
	{
		Destroy();
		std::swap(m_handle, other.m_handle);
		std::swap(m_window, other.m_window);

		return *this;
	}

	OpenGLContext::~OpenGLContext() noexcept
	{
		Destroy();
	}

	void OpenGLContext::Initialise(const Window& window)
	{
		m_handle = SDL_GL_CreateContext(window.GetRawHandle());
		m_window = &window;
	}

	void OpenGLContext::Destroy()
	{
		if (m_handle != nullptr)
		{
			SDL_GL_DeleteContext(m_handle);
			m_handle = nullptr;
			m_window = nullptr;
		}
	}

	Status OpenGLContext::MakeCurrent() const
	{
		return SDL_GL_MakeCurrent(m_window->GetRawHandle(), m_handle) == 0
			? Status::Success
			: Status::Fail;
	}

	void __cdecl OpenGLContext::DebugMessageCallback(const u32 source, const u32 type, const u32 id, const u32 severity, const i32 length, const char* message, const void* userParams) noexcept
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			Log::EngineError("[OPENGL ERROR]: {}", message);

			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			// This Nvidia warning is usually always a false positive, so it is best not to log it.
			if (!s_isNvidiaGPU || (s_isNvidiaGPU && id != 131'218u))
			{
				Log::EngineWarn("[OPENGL WARNING]: {}", message);
			}

			break;

		case GL_DEBUG_SEVERITY_LOW:
			Log::EngineDebug("[OPENGL PERFORMANCE]: {}", message);

			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
		default:
			break;
		}
	}
}