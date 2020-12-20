#ifndef OPENGL_H
#define OPENGL_H

#include <glad/glad.h>

#include "stardust/utility/enums/Status.h"

#ifndef WIN32
#define __cdecl
#endif

namespace stardust
{
	namespace opengl
	{
		[[nodiscard]] extern Status InitialiseLoader();
		extern void InitialiseDebugCallback();

		extern void __cdecl DebugMessageCallback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParams) noexcept;
	}
}

#endif