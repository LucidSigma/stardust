#ifndef OPENGL_H
#define OPENGL_H

#include "stardust/data/Types.h"
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

		extern void __cdecl DebugMessageCallback(const u32 source, const u32 type, const u32 id, const u32 severity, const i32 length, const char* message, const void* userParams) noexcept;
	}
}

#endif