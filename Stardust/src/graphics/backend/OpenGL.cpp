#include "stardust/graphics/backend/OpenGL.h"

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/debug/logging/Log.h"

#ifdef WIN32
#include <windows.h>

extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001u;
	_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001u;
}
#endif

namespace stardust
{
	namespace opengl
	{
		static bool isNvidiaGPU = false;

		[[nodiscard]] Status InitialiseLoader()
		{
			return gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)) != 0
				? Status::Success
				: Status::Fail;
		}

		void InitialiseDebugCallback()
		{
			String vendorName(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			vendorName.make_lower();

			isNvidiaGPU = vendorName.find("nvidia") != String::npos;

			glDebugMessageCallback(DebugMessageCallback, nullptr);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		void __cdecl DebugMessageCallback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParams) noexcept
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				Log::EngineError("[OPENGL ERROR]: {}", message);

				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				// The following Nvidia warnings are usually always false positives in this engine, so it is best not to log them.
				// 131218 is to do with shaders matching the state when they're bound and used for rendering.
				// 131154 is to do with not using separate hardware transfer queues. Right now it's a false positive when taking screenshots.
				if (!isNvidiaGPU || (isNvidiaGPU && (id != 131'218u && id != 131'154u)))
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
}