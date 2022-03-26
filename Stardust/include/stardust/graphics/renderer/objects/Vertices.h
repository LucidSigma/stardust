#pragma once
#ifndef STARDUST_VERTICES_H
#define STARDUST_VERTICES_H

#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        struct LineVertex final
        {
            Vector2 position;
            Vector4 colour;
        };

        struct QuadVertex final
        {
            Vector2 position;
            Vector4 colour;
            Vector2 textureCoordinates;
        };

        constexpr f32 ViewProjectionType = 0.0f;
        constexpr f32 ScreenProjectionType = 1.0f;

        struct BatchLineVertex final
        {
            Vector2 position;
            Vector4 colour;
            f32 projectionType;
        };

        struct BatchQuadVertex final
        {
            Vector2 position;
            Vector4 colour;
            Vector2 textureCoordinates;
            f32 textureIndex;
            f32 projectionType;
        };
    }
}

#endif
