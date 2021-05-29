#pragma once
#ifndef STARDUST_QUAD_H
#define STARDUST_QUAD_H

#include "stardust/data/MathTypes.h"

namespace stardust
{
    // For best results, centre the quad around (0.0, 0.0).
    struct Quad
    {
        Vec2 lowerLeft;
        Vec2 upperLeft;

        Vec2 upperRight;
        Vec2 lowerRight;
    };

    [[nodiscard]] inline Quad CreateQuad(const Vec2& lowerLeft, const Vec2& upperLeft, const Vec2& upperRight, const Vec2& lowerRight)
    {
        return Quad{
            .lowerLeft = lowerLeft,
            .upperLeft = upperLeft,
            .upperRight = upperRight,
            .lowerRight = lowerRight,
        };
    }
}

#endif