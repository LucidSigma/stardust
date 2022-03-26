#pragma once
#ifndef STARDUST_SAMPLER_H
#define STARDUST_SAMPLER_H

#include <ANGLE/GLES3/gl3.h>

namespace stardust
{
    namespace graphics
    {
        enum class TextureWrap
            : GLint
        {
            Repeat = GL_REPEAT,
            MirroredRepeat = GL_MIRRORED_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
        };

        enum class TextureMinFilter
            : GLint
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,
            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
            LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
        };

        enum class TextureMagFilter
            : GLint
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,
        };

        struct Sampler final
        {
            TextureWrap horizontalWrap = TextureWrap::Repeat;
            TextureWrap verticalWrap = TextureWrap::Repeat;

            TextureMinFilter minFilter = TextureMinFilter::Nearest;
            TextureMagFilter magFilter = TextureMagFilter::Nearest;

            bool generateMipmaps = false;
        };

        constexpr Sampler DefaultSampler{ };
    }
}

#endif
