#pragma once
#ifndef STARDUST_VERTEX_LAYOUT_BUILDER_H
#define STARDUST_VERTEX_LAYOUT_BUILDER_H

#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        class VertexLayout;

        class VertexLayoutBuilder final
        {
        private:
            struct AttributeState final
            {
                VertexAttribute attribute;
                usize offset;
            };

            Queue<AttributeState> m_attributes{ };
            List<ReferenceWrapper<const VertexBuffer>> m_vertexBuffers{ };

            usize m_vertexSize = 0u;

        public:
            auto AddAttribute(const VertexAttribute& attribute) -> VertexLayoutBuilder&;
            auto AddVertexBuffer(const VertexBuffer& vertexBuffer) -> VertexLayoutBuilder&;

            [[nodiscard]] auto Build()->VertexLayout;

        private:
            [[nodiscard]] static constexpr auto GetAttributeTypeSize(const VertexAttribute::Type attributeType) -> usize
            {
                static_assert(sizeof(i8) == sizeof(GLbyte));
                static_assert(sizeof(i16) == sizeof(GLshort));
                static_assert(sizeof(i32) == sizeof(GLint));
                static_assert(sizeof(u8) == sizeof(GLubyte));
                static_assert(sizeof(u16) == sizeof(GLushort));
                static_assert(sizeof(u32) == sizeof(GLuint));
                static_assert(sizeof(f32) == sizeof(GLfloat));

                switch (attributeType)
                {
                case VertexAttribute::Type::Int8:
                    return sizeof(i8);

                case VertexAttribute::Type::Int16:
                    return sizeof(i16);

                case VertexAttribute::Type::Int32:
                    return sizeof(i32);

                case VertexAttribute::Type::UnsignedInt8:
                    return sizeof(u8);

                case VertexAttribute::Type::UnsignedInt16:
                    return sizeof(u16);

                case VertexAttribute::Type::UnsignedInt32:
                    return sizeof(u32);

                case VertexAttribute::Type::Float32:
                default:
                    return sizeof(f32);
                }
            }
        };
    }
}

#endif
