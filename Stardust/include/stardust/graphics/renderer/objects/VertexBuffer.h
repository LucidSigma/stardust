#pragma once
#ifndef STARDUST_VERTEX_BUFFER_H
#define STARDUST_VERTEX_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"

namespace stardust
{
    class VertexBuffer
        : private INoncopyable
    {
    private:
        GLuint m_id = 0u;

    public:
        VertexBuffer() = default;

        template <typename T = f32>
        explicit VertexBuffer(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
        {
            Initialise(vertices, usage);
        }

        explicit VertexBuffer(const usize size, const BufferUsage usage = BufferUsage::Static);

        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator =(VertexBuffer&& other) noexcept;

        ~VertexBuffer() noexcept;

        template <typename T = f32>
        void Initialise(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
        {
            glGenBuffers(1, &m_id);
            Bind();

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(T) * vertices.size()), vertices.data(), static_cast<GLenum>(usage));
            
            Unbind();
        }

        void Initialise(const usize size, const BufferUsage usage = BufferUsage::Static);
        void Destroy() noexcept;

        inline bool IsValid() const noexcept { return m_id != 0u; }

        void Bind() const;
        void Unbind() const;

        template <typename T = f32>
        void SetSubData(const Vector<T>& data, const usize count, const uptr offset = 0u) const
        {
            Bind();
            glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(count * sizeof(T)), data.data());
            Unbind();
        }

        inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }
    };
}

#endif