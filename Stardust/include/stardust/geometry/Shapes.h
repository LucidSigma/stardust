#pragma once
#ifndef STARDUST_SHAPES_H
#define STARDUST_SHAPES_H

#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    namespace geometry
    {
        using Point = Vector2;
        using ScreenPoint = IVector2;

        struct Line final
        {
            Point pointA = Vector2Zero;
            Point pointB = Vector2Zero;
        };

        struct ScreenLine final
        {
            ScreenPoint pointA = IVector2Zero;
            ScreenPoint pointB = IVector2Zero;
        };

        struct ScreenRectangle final
        {
            ScreenPoint topLeft = IVector2Zero;
            UVector2 size = UVector2Zero;
        };

        struct Rectangle final
        {
            Point centre = Vector2Zero;
            Vector2 size = Vector2One;
        };

        struct Quad final
        {
            Point lowerLeft = Vector2Zero;
            Point upperLeft = Vector2Zero;

            Point upperRight = Vector2Zero;
            Point lowerRight = Vector2Zero;
        };

        struct ScreenQuad final
        {
            ScreenPoint lowerLeft = IVector2Zero;
            ScreenPoint upperLeft = IVector2Zero;

            ScreenPoint upperRight = IVector2Zero;
            ScreenPoint lowerRight = IVector2Zero;
        };
    }

    namespace geom = geometry;
}

#endif
