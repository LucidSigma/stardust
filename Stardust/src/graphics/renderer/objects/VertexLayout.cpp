#include "stardust/graphics/renderer/objects/VertexLayout.h"

#include <utility>

namespace stardust
{
    namespace graphics
    {
        VertexLayout::VertexLayout(VertexLayout&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
        }

        auto VertexLayout::operator =(VertexLayout&& other) noexcept -> VertexLayout&
        {
            Destroy();
            std::swap(m_id, other.m_id);

            return *this;
        }

        VertexLayout::~VertexLayout() noexcept
        {
            Destroy();
        }

        auto VertexLayout::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Unbind();

                glDeleteVertexArrays(1, &m_id);
                m_id = s_InvalidID;
            }
        }

        auto VertexLayout::Bind() const noexcept -> void
        {
            glBindVertexArray(m_id);
        }

        auto VertexLayout::Unbind() const noexcept -> void
        {
            glBindVertexArray(s_InvalidID);
        }

        auto VertexLayout::Draw(const u32 vertexCount, const u32 offset, const DrawMode drawMode) const -> void
        {
            glDrawArrays(static_cast<GLenum>(drawMode), static_cast<GLint>(offset), static_cast<GLsizei>(vertexCount));
        }

        auto VertexLayout::DrawInstanced(const u32 vertexCount, const u32 instanceCount, const u32 offset, const DrawMode drawMode) const -> void
        {
            glDrawArraysInstanced(
                static_cast<GLenum>(drawMode),
                static_cast<GLint>(offset),
                static_cast<GLsizei>(vertexCount),
                static_cast<GLsizei>(instanceCount)
            );
        }

        auto VertexLayout::DrawIndexed(const IndexBuffer& indexBuffer, const bool bindIndexBuffer, const DrawMode drawMode) const -> void
        {
            if (bindIndexBuffer)
            {
                indexBuffer.Bind();
            }

            glDrawElements(
                static_cast<GLenum>(drawMode),
                static_cast<GLsizei>(indexBuffer.GetIndexCount()),
                static_cast<GLenum>(indexBuffer.GetDataType()),
                nullptr
            );

            if (bindIndexBuffer)
            {
                indexBuffer.Unbind();
            }
        }

        auto VertexLayout::DrawIndexed(const IndexBuffer& indexBuffer, const u32 indexCount, const bool bindIndexBuffer, const DrawMode drawMode) const -> void
        {
            if (bindIndexBuffer)
            {
                indexBuffer.Bind();
            }

            glDrawElements(
                static_cast<GLenum>(drawMode),
                static_cast<GLsizei>(indexCount),
                static_cast<GLenum>(indexBuffer.GetDataType()),
                nullptr
            );

            if (bindIndexBuffer)
            {
                indexBuffer.Unbind();
            }
        }

        auto VertexLayout::DrawIndexedInstanced(const IndexBuffer& indexBuffer, const u32 instanceCount, const bool bindIndexBuffer, const DrawMode drawMode) const -> void
        {
            if (bindIndexBuffer)
            {
                indexBuffer.Bind();
            }

            glDrawElementsInstanced(
                static_cast<GLenum>(drawMode),
                static_cast<GLsizei>(indexBuffer.GetIndexCount()),
                static_cast<GLenum>(indexBuffer.GetDataType()),
                nullptr,
                static_cast<GLsizei>(instanceCount)
            );

            if (bindIndexBuffer)
            {
                indexBuffer.Unbind();
            }
        }

        auto VertexLayout::DrawIndexedInstanced(const IndexBuffer& indexBuffer, const u32 indexCount, const u32 instanceCount, const bool bindIndexBuffer, const DrawMode drawMode) const -> void
        {
            if (bindIndexBuffer)
            {
                indexBuffer.Bind();
            }

            glDrawElementsInstanced(
                static_cast<GLenum>(drawMode),
                static_cast<GLsizei>(indexCount),
                static_cast<GLenum>(indexBuffer.GetDataType()),
                nullptr,
                static_cast<GLsizei>(instanceCount)
            );

            if (bindIndexBuffer)
            {
                indexBuffer.Unbind();
            }
        }
    }
}
