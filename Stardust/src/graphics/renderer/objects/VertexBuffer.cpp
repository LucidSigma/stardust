#include "stardust/graphics/renderer/objects/VertexBuffer.h"

#include <utility>

namespace stardust
{
    namespace graphics
    {
        VertexBuffer::VertexBuffer(const usize size, const BufferUsage usage)
        {
            Initialise(size, usage);
        }

        VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
        }

        auto VertexBuffer::operator =(VertexBuffer&& other) noexcept -> VertexBuffer&
        {
            Destroy();
            std::swap(m_id, other.m_id);

            return *this;
        }

        VertexBuffer::~VertexBuffer() noexcept
        {
            Destroy();
        }

        auto VertexBuffer::Initialise(const usize size, const BufferUsage usage) -> void
        {
            glGenBuffers(1, &m_id);

            Bind();
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(size),
                nullptr,
                static_cast<GLenum>(usage)
            );
            Unbind();
        }

        auto VertexBuffer::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Unbind();

                glDeleteBuffers(1, &m_id);
                m_id = s_InvalidID;
            }
        }
        
        auto VertexBuffer::Bind() const noexcept -> void
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id);
        }
        
        auto VertexBuffer::Unbind() const noexcept -> void
        {
            glBindBuffer(GL_ARRAY_BUFFER, s_InvalidID);
        }
    }
}
