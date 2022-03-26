#include "stardust/graphics/framebuffer/RenderBuffer.h"

#include <utility>

namespace stardust
{
    namespace graphics
    {
        RenderBuffer::RenderBuffer(const UVector2 extent, const StorageFormat storageFormat)
        {
            Initialise(extent, storageFormat);
        }

        RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_size, other.m_size);
            std::swap(m_storageFormat, other.m_storageFormat);
        }

        auto RenderBuffer::operator =(RenderBuffer&& other) noexcept -> RenderBuffer&
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_size, other.m_size);
            std::swap(m_storageFormat, other.m_storageFormat);

            return *this;
        }

        RenderBuffer::~RenderBuffer() noexcept
        {
            Destroy();
        }

        auto RenderBuffer::Initialise(const UVector2 extent, const StorageFormat storageFormat) -> void
        {
            glGenRenderbuffers(1, &m_id);
            Resize(extent, storageFormat);
        }

        auto RenderBuffer::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                glDeleteRenderbuffers(1, &m_id);

                m_id = s_InvalidID;
                m_size = UVector2Zero;
                m_storageFormat = StorageFormat::RGBA8;
            }
        }

        auto RenderBuffer::Resize(const UVector2 extent, const StorageFormat storageFormat) -> void
        {
            Bind();

            glRenderbufferStorage(
                GL_RENDERBUFFER,
                static_cast<GLenum>(storageFormat),
                static_cast<GLsizei>(extent.x),
                static_cast<GLsizei>(extent.y)
            );

            Unbind();
        }

        auto RenderBuffer::Bind() const noexcept -> void
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_id);
        }

        auto RenderBuffer::Unbind() const noexcept -> void
        {
            glBindRenderbuffer(GL_RENDERBUFFER, s_InvalidID);
        }
    }
}
