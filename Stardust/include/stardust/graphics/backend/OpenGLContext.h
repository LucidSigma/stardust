#pragma once
#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Pointers.h"
#include "stardust/utility/enums/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
	class OpenGLContext
		: private INoncopyable
	{
	private:
		SDL_GLContext m_handle = nullptr;
		ObserverPtr<const Window> m_window = nullptr;

	public:
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
	};
}

#endif