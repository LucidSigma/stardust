#pragma once
#ifndef STARDUST_VERTEX_ATTRIBUTE_H
#define STARDUST_VERTEX_ATTRIBUTE_H

#include <ANGLE/GLES3/gl3.h>

#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        struct VertexAttribute final
        {
            enum class Type
                : GLenum
            {
                Int8 = GL_BYTE,
                Int16 = GL_SHORT,
                Int32 = GL_INT,
                UnsignedInt8 = GL_UNSIGNED_BYTE,
                UnsignedInt16 = GL_UNSIGNED_SHORT,
                UnsignedInt32 = GL_UNSIGNED_INT,
                Float32 = GL_FLOAT,
            };

            u32 elementCount;
            Type dataType;

            bool isNormalised;
        };
    }
}

#endif
