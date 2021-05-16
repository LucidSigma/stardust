#include "stardust/graphics/backend/OpenGL.h"

#include <SDL2/SDL.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "stardust/data/Containers.h"
#include "stardust/debug/logging/Log.h"

#ifndef WIN32
#define __cdecl
#endif


#ifdef WIN32
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
		[[nodiscard]] Status InitialiseLoader()
		{
			return gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)) != 0
				? Status::Success
				: Status::Fail;
		}

		void InitialiseDebugCallback()
		{
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
				Log::EngineWarn("[OPENGL WARNING]: {}", message);

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

#ifndef WIN32
#undef __cdecl
#endif