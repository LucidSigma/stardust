#include "stardust/graphics/backend/OpenGL.h"

#include <SDL2/SDL.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/debug/logging/Log.h"

#ifndef WIN32
    #define __cdecl
#endif

#ifdef WIN32
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00'00'00'01u;
    _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00'00'00'01u;
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

        [[nodiscard]] VersionInfo GetVersion()
        {
            GLint openGLMajorVersion = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &openGLMajorVersion);

            GLint openGLMinorVersion = 0;
            glGetIntegerv(GL_MINOR_VERSION, &openGLMinorVersion);

            return VersionInfo{
                .major = static_cast<u32>(openGLMajorVersion),
                .minor = static_cast<u32>(openGLMinorVersion),
                .patch = 0u,
            };
        }
    }
}

#ifndef WIN32
    #undef __cdecl
#endif