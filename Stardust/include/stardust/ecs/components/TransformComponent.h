#pragma once
#ifndef STARDUST_TRANSFORM_COMPONENT_H
#define STARDUST_TRANSFORM_COMPONENT_H

#include "stardust/graphics/Graphics.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace components
    {
        struct Transform final
        {
            Vector2 translation = Vector2Zero;

            Vector2 scale = Vector2One;
            graphics::Reflection reflection = graphics::Reflection::None;

            f32 rotation = 0.0f;
            Optional<Vector2> pivot = None;

            Optional<Vector2> shear = None;
        };
    }
}

#endif
