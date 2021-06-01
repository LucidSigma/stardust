#include "stardust/graphics/renderer/objects/IndexBuffer.h"

#include <utility>

namespace stardust
{
    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_indexCount, other.m_indexCount);
        std::swap(m_dataType, other.m_dataType);
    }

    IndexBuffer& IndexBuffer::operator =(IndexBuffer&& other) noexcept
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_indexCount, other.m_indexCount);
        std::swap(m_dataType, other.m_dataType);

        return *this;
    }

    IndexBuffer::~IndexBuffer() noexcept
    {
        Destroy();
    }

    void IndexBuffer::Destroy() noexcept
    {
        if (m_id != 0u)
        {
            Unbind();

            glDeleteBuffers(1, &m_id);

            m_id = 0u;
            m_indexCount = 0u;
            m_dataType = GL_UNSIGNED_INT;
        }
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
    }
}