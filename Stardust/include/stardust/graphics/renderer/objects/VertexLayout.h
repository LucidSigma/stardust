#pragma once
#ifndef STARDUST_VERTEX_LAYOUT_H
#define STARDUST_VERTEX_LAYOUT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        enum class DrawMode
            : GLenum
        {
            Points = GL_POINTS,
            Lines = GL_LINES,
            LineLoop = GL_LINE_LOOP,
            LineStrip = GL_LINE_STRIP,
            Triangles = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
        };

        class VertexLayout final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

        private:
            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;

        public:
            friend class VertexLayoutBuilder;

            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            VertexLayout() = default;

            VertexLayout(VertexLayout&& other) noexcept;
            auto operator =(VertexLayout&& other) noexcept -> VertexLayout&;

            ~VertexLayout() noexcept;

            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID; }

            auto Bind() const noexcept -> void;
            auto Unbind() const noexcept -> void;

            auto Draw(const u32 vertexCount, const u32 offset = 0u, const DrawMode drawMode = DrawMode::Triangles) const -> void;
            auto DrawInstanced(const u32 vertexCount, const u32 instanceCount, const u32 offset = 0u, const DrawMode drawMode = DrawMode::Triangles) const -> void;

            auto DrawIndexed(const IndexBuffer& indexBuffer, const bool bindIndexBuffer = true, const DrawMode drawMode = DrawMode::Triangles) const -> void;
            auto DrawIndexed(const IndexBuffer& indexBuffer, const u32 indexCount, const bool bindIndexBuffer = true, const DrawMode drawMode = DrawMode::Triangles) const -> void;
            auto DrawIndexedInstanced(const IndexBuffer& indexBuffer, const u32 instanceCount, const bool bindIndexBuffer = true, const DrawMode drawMode = DrawMode::Triangles) const -> void;
            auto DrawIndexedInstanced(const IndexBuffer& indexBuffer, const u32 indexCount, const u32 instanceCount, const bool bindIndexBuffer = true, const DrawMode drawMode = DrawMode::Triangles) const -> void;

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
        };
    }
}

#endif
