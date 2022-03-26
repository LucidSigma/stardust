#pragma once
#ifndef STARDUST_VERTEX_BUFFER_H
#define STARDUST_VERTEX_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        class VertexBuffer final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

        private:
            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            VertexBuffer() = default;
            explicit VertexBuffer(const usize size, const BufferUsage usage = BufferUsage::Static);

            template <typename T = f32>
            explicit VertexBuffer(const List<T>& vertices, const BufferUsage usage = BufferUsage::Static)
            {
                Initialise(vertices, usage);
            }

            VertexBuffer(VertexBuffer&& other) noexcept;
            auto operator =(VertexBuffer&& other) noexcept -> VertexBuffer&;

            ~VertexBuffer() noexcept;

            auto Initialise(const usize size, const BufferUsage usage = BufferUsage::Static) -> void;
            
            template <typename T = f32>
            auto Initialise(const List<T>& vertices, const BufferUsage usage = BufferUsage::Static) -> void
            {
                glGenBuffers(1, &m_id);

                Bind();
                glBufferData(
                    GL_ARRAY_BUFFER,
                    static_cast<GLsizeiptr>(sizeof(T) * vertices.size()),
                    vertices.data(),
                    static_cast<GLenum>(usage)
                );
                Unbind();
            }

            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID; }

            auto Bind() const noexcept -> void;
            auto Unbind() const noexcept -> void;

            template <typename T = f32>
            auto SetSubData(const List<T>& data, const usize count, const uptr offset = 0u) const -> void
            {
                Bind();
                glBufferSubData(
                    GL_ARRAY_BUFFER,
                    static_cast<GLintptr>(offset * sizeof(T)),
                    static_cast<GLsizeiptr>(count * sizeof(T)),
                    data.data()
                );
                Unbind();
            }

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
        };
    }
}

#endif
