#ifdef STARDUST_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <ANGLE/GLES3/gl3.h>

#include "stardust/debug/logging/Logging.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

#ifdef STARDUST_PLATFORM_WINDOWS
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
        namespace
        {
            enum class Error
                : GLenum
            {
                NoError = GL_NO_ERROR,
                InvalidEnum = GL_INVALID_ENUM,
                InvalidValue = GL_INVALID_VALUE,
                InvalidOperation = GL_INVALID_OPERATION,
                InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
                OutOfMemory = GL_OUT_OF_MEMORY,
            };

            [[nodiscard]] inline auto GetCurrentError() noexcept -> Error
            {
                return static_cast<Error>(glGetError());
            }

            [[nodiscard]] auto GetErrorString(const Error error) -> String
            {
                switch (error)
                {
                case Error::NoError:
                    return "No error";

                case Error::InvalidEnum:
                    return "Invalid enum";

                case Error::InvalidValue:
                    return "Invalid value";

                case Error::InvalidOperation:
                    return "Invalid operation";

                case Error::InvalidFramebufferOperation:
                    return "Invalid framebuffer operation";

                case Error::OutOfMemory:
                    return "Out of memory";

                [[unlikely]] default:
                    return "";
                }
            }
        }

        auto CheckErrors() -> void
        {
            if (const Error error = GetCurrentError();
                error != Error::NoError)
            {
                Log::EngineError("OpenGL error: {} [Error code: {}]", GetErrorString(error), static_cast<u32>(error));
            }
        }
    }
}
