#include "stardust/graphics/renderer/objects/VertexBuffer.h"

#include <utility>

namespace stardust
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

    VertexBuffer& VertexBuffer::operator =(VertexBuffer&& other) noexcept
    {
        Destroy();
        std::swap(m_id, other.m_id);

        return *this;
    }

    VertexBuffer::~VertexBuffer() noexcept
    {
        Destroy();
    }

    void VertexBuffer::Initialise(const usize size, const BufferUsage usage)
    {
        glGenBuffers(1, &m_id);
        Bind();

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, static_cast<GLenum>(usage));

        Unbind();
    }

    void VertexBuffer::Destroy() noexcept
    {
        Unbind();

        if (m_id != 0u)
        {
            glDeleteBuffers(1, &m_id);
            m_id = 0u;
        }
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0u);
    }
}