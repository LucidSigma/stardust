#include "stardust/graphics/backend/OpenGLContext.h"

#include <utility>

namespace stardust
{
    OpenGLContext::OpenGLContext(const Window& window)
    {
        Initialise(window);
    }

    OpenGLContext::OpenGLContext(OpenGLContext&& other) noexcept
    {
        Destroy();

        std::swap(m_handle, other.m_handle);
        std::swap(m_window, other.m_window);
    }

    OpenGLContext& OpenGLContext::operator =(OpenGLContext&& other) noexcept
    {
        Destroy();
        std::swap(m_handle, other.m_handle);
        std::swap(m_window, other.m_window);

        return *this;
    }

    OpenGLContext::~OpenGLContext() noexcept
    {
        Destroy();
    }

    void OpenGLContext::Initialise(const Window& window)
    {
        m_handle = SDL_GL_CreateContext(window.GetRawHandle());
        m_window = &window;
    }

    void OpenGLContext::Destroy() noexcept
    {
        if (m_handle != nullptr)
        {
            SDL_GL_DeleteContext(m_handle);
            m_handle = nullptr;
            m_window = nullptr;
        }
    }

    Status OpenGLContext::MakeCurrent() const
    {
        return SDL_GL_MakeCurrent(m_window->GetRawHandle(), m_handle) == 0
            ? Status::Success
            : Status::Fail;
    }
}