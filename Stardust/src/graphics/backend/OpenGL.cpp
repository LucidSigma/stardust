#include "stardust/graphics/backend/OpenGL.h"

#include <glad/glad.h>
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

		void __cdecl DebugMessageCallback(const u32 source, const u32 type, const u32 id, const u32 severity, const i32 length, const char* message, const void* userParams) noexcept
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				Log::EngineError("[OPENGL ERROR]: {}", message);

				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				// This Nvidia warning is usually always a false positive, so it is best not to log it.
				if (!isNvidiaGPU || (isNvidiaGPU && id != 131'218u))
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