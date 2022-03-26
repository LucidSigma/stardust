#pragma once
#ifndef STARDUST_OPENGL_CONTEXT_H
#define STARDUST_OPENGL_CONTEXT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/types/Pointers.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    namespace opengl
    {
        class Context final
            : private INoncopyable
        {
        private:
            SDL_GLContext m_handle = nullptr;
            ObserverPointer<const Window> m_window = nullptr;

        public:
            Context() = default;
            explicit Context(const Window& window);

            Context(Context&& other) noexcept;
            auto operator =(Context&& other) noexcept -> Context&;

            ~Context() noexcept;

            auto Initialise(const Window& window) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

            [[nodiscard]] auto MakeCurrent() -> Status;

            [[nodiscard]] inline auto GetRawHandle() const noexcept -> SDL_GLContext { return m_handle; }
        };
    }
}

#endif
