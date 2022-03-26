#pragma once
#ifndef STARDUST_BUFFER_USAGE_H
#define STARDUST_BUFFER_USAGE_H

#include <ANGLE/GLES3/gl3.h>

namespace stardust
{
    namespace graphics
    {
        enum class BufferUsage
            : GLenum
        {
            Stream = GL_STREAM_DRAW,
            Static = GL_STATIC_DRAW,
            Dynamic = GL_DYNAMIC_DRAW,
        };
    }
}

#endif
