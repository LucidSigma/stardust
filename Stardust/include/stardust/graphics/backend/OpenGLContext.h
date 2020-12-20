#pragma once
#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/enums/Status.h"
#include "stardust/window/Window.h"

#ifndef WIN32
#define __cdecl
#endif

namespace stardust
{
	class OpenGLContext
	{
	private:
		inline static bool s_isNvidiaGPU = false;

		SDL_GLContext m_handle = nullptr;
		ObserverPtr<const Window> m_window = nullptr;

	public:
		static Status InitialiseLoader();
		static void InitialiseDebugCallback();

		OpenGLContext();
		OpenGLContext(const Window& window);

		OpenGLContext(OpenGLContext&& other) noexcept;
		OpenGLContext& operator =(OpenGLContext&& other) noexcept;

		~OpenGLContext() noexcept;

		void Initialise(const Window& window);
		void Destroy();

		Status MakeCurrent() const;

		bool IsValid() const noexcept { return m_handle != nullptr; }
		[[nodiscard]] inline SDL_GLContext const GetRawHandle() const noexcept { return m_handle; }

	private:
		static void __cdecl DebugMessageCallback(const u32 source, const u32 type, const u32 id, const u32 severity, const i32 length, const char* message, const void* userParams) noexcept;
	};
}

#endif