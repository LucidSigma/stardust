#pragma once
#ifndef STARDUST_PHYSICS_H
#define STARDUST_PHYSICS_H

#include <limits>

#include <box2d/box2d.h>

#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        using Shape = b2Shape;
        using CollisionLayer = u16;

        constexpr CollisionLayer AllLayers = std::numeric_limits<u16>::max();
        constexpr CollisionLayer NoLayers = 0u;

        enum class ShapeType
        {
            Circle = b2Shape::e_circle,
            Edge = b2Shape::e_edge,
            Polygon = b2Shape::e_polygon,
            Chain = b2Shape::e_chain,
        };

        struct MassData final
        {
            f32 mass;
            Vector2 centre;
            f32 momentOfInertia;
        };
    }

    namespace phys = physics;
}

#endif
