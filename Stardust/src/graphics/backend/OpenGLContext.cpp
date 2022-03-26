#include "stardust/graphics/backend/OpenGLContext.h"

#include <utility>

namespace stardust
{
    namespace opengl
    {
        Context::Context(const Window& window)
        {
            Initialise(window);
        }

        Context::Context(Context&& other) noexcept
        {
            Destroy();

            std::swap(m_handle, other.m_handle);
            std::swap(m_window, other.m_window);
        }

        auto Context::operator =(Context&& other) noexcept -> Context&
        {
            Destroy();

            std::swap(m_handle, other.m_handle);
            std::swap(m_window, other.m_window);

            return *this;
        }

        Context::~Context() noexcept
        {
            Destroy();
        }

        auto Context::Initialise(const Window& window) -> void
        {
            m_handle = SDL_GL_CreateContext(window.GetRawHandle());
            m_window = &window;
        }
        
        auto Context::Destroy() noexcept -> void
        {
            if (m_handle != nullptr)
            {
                SDL_GL_DeleteContext(m_handle);

                m_handle = nullptr;
                m_window = nullptr;
            }
        }
        
        [[nodiscard]] auto Context::MakeCurrent() -> Status
        {
            return SDL_GL_MakeCurrent(m_window->GetRawHandle(), m_handle) == 0
                ? Status::Success
                : Status::Fail;
        }
    }
}
