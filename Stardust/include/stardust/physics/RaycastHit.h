#pragma once
#ifndef STARDUST_RAYCAST_HIT_H
#define STARDUST_RAYCAST_HIT_H

#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/physics/Physics.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        struct RaycastHit final
        {
            Collider collider;
            EntityHandle entityHandle;

            f32 distance;
            f32 fraction;

            Vector2 normal;
            Vector2 point;
        };
    }
}

#endif
