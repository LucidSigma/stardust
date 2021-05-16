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
}

#endif