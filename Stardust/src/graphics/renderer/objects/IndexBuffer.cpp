#include "stardust/graphics/renderer/objects/IndexBuffer.h"

#include <utility>

namespace stardust
{
    namespace graphics
    {
        IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_indexType, other.m_indexType);
            std::swap(m_indexCount, other.m_indexCount);
        }

        auto IndexBuffer::operator =(IndexBuffer&& other) noexcept -> IndexBuffer&
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_indexType, other.m_indexType);
            std::swap(m_indexCount, other.m_indexCount);

            return *this;
        }
        
        IndexBuffer::~IndexBuffer() noexcept
        {
            Destroy();
        }

        auto IndexBuffer::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Unbind();

                glDeleteBuffers(1, &m_id);

                m_id = s_InvalidID;
                m_indexType = IndexType::UnsignedInt32;
                m_indexCount = 0u;
            }
        }
        
        auto IndexBuffer::Bind() const noexcept -> void
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        }
        
        auto IndexBuffer::Unbind() const noexcept -> void
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_InvalidID);
        }
    }
}
