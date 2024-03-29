#pragma once
#ifndef STARDUST_INDEX_BUFFER_H
#define STARDUST_INDEX_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <concepts>
#include <type_traits>

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace graphics
    {
        class IndexBuffer final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

            enum class IndexType
                : GLenum
            {
                UnsignedInt8 = GL_UNSIGNED_BYTE,
                UnsignedInt16 = GL_UNSIGNED_SHORT,
                UnsignedInt32 = GL_UNSIGNED_INT,
            };

        private:
            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;
            IndexType m_indexType = IndexType::UnsignedInt32;

            u32 m_indexCount = 0u;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            IndexBuffer() = default;

            template <std::unsigned_integral T = u32>
            explicit IndexBuffer(const List<T>& indices, const BufferUsage usage = BufferUsage::Static)
            {
                Initialise(indices, usage);
            }

            IndexBuffer(IndexBuffer&& other) noexcept;
            auto operator =(IndexBuffer&& other) noexcept -> IndexBuffer&;

            ~IndexBuffer() noexcept;

            template <std::unsigned_integral T = u32>
            auto Initialise(const List<T>& indices, const BufferUsage usage = BufferUsage::Static) -> void
            {
                glGenBuffers(1, &m_id);

                Bind();
                glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(T) * indices.size(),
                    indices.data(),
                    static_cast<GLenum>(usage)
                );
                Unbind();

                m_indexType = GetOpenGLIndexType<T>();
                m_indexCount = static_cast<u32>(indices.size());
            }

            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID; }

            auto Bind() const noexcept -> void;
            auto Unbind() const noexcept -> void;

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
            [[nodiscard]] inline auto GetDataType() const noexcept -> IndexType { return m_indexType; }

            [[nodiscard]] inline auto GetIndexCount() const noexcept -> u32 { return m_indexCount; }

        private:
            template <std::unsigned_integral T>
            [[nodiscard]] static consteval auto GetOpenGLIndexType() noexcept -> IndexType
            {
                static_assert(sizeof(u8) == sizeof(GLubyte));
                static_assert(sizeof(u16) == sizeof(GLushort));
                static_assert(sizeof(u32) == sizeof(GLuint));

                if constexpr (std::is_same_v<T, u32>)
                {
                    return IndexType::UnsignedInt32;
                }
                else if constexpr (std::is_same_v<T, u16>)
                {
                    return IndexType::UnsignedInt16;
                }
                else if constexpr (std::is_same_v<T, u8>)
                {
                    return IndexType::UnsignedInt8;
                }
                else
                {
                    static_assert(utility::DependentFalse<T>, "Unimplemented index buffer type.");
                }
            }
        };
    }
}

#endif
