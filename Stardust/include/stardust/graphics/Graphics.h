#pragma once
#ifndef STARDUST_GRAPHICS_H
#define STARDUST_GRAPHICS_H

#include <ANGLE/GLES3/gl3.h>

#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        struct PixelData final
        {
            List<ubyte> pixels{ };

            UVector2 extent = UVector2Zero;
            u32 channelCount = 0u;
        };

        enum class DepthFunction
            : GLenum
        {
            Equal = GL_EQUAL,
            NotEqual = GL_NOTEQUAL,
            LessThan = GL_LESS,
            LessThanOrEqual = GL_LEQUAL,
            GreaterThan = GL_GREATER,
            GreaterThanOrEqual = GL_GEQUAL,
            Always = GL_ALWAYS, 
            Never = GL_NEVER,
        };

        enum class StencilFunction
            : GLenum
        {
            Equal = GL_EQUAL,
            NotEqual = GL_NOTEQUAL,
            LessThan = GL_LESS,
            LessThanOrEqual = GL_LEQUAL,
            GreaterThan = GL_GREATER,
            GreaterThanOrEqual = GL_GEQUAL,
            Always = GL_ALWAYS,
            Never = GL_NEVER,
        };

        enum class StencilOperation
            : GLenum
        {
            Keep = GL_KEEP,
            SetToZero = GL_ZERO,
            ReplaceWithReference = GL_REPLACE,
            Increment = GL_INCR,
            WrappedIncrement = GL_INCR_WRAP,
            Decrement = GL_DECR,
            WrappedDecrement = GL_DECR_WRAP,
            Invert = GL_INVERT,
        };

        struct StencilOperations final
        {
            StencilOperation onStencilFail;
            StencilOperation onStencilPassAndDepthFail;
            StencilOperation onStencilPassAndDepthPass;
        };

        struct StencilParameters final
        {
            StencilFunction function = StencilFunction::Equal;

            ubyte reference = 0xFFu;
            ubyte mask = 0xFFu;
        };

        enum class Reflection
        {
            None,
            Horizontal,
            Vertical,
            Both,
        };

        enum class PresentationMode
            : i32
        {
            AdaptiveVSync = -1,
            Immediate = 0,
            VSync = 1,
        };

        extern auto SetPresentationMode(const PresentationMode presentationMode) -> void;
    }

    namespace gfx = graphics;
}

#endif
