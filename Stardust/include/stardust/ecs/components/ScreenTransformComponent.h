#pragma once
#ifndef STARDUST_SCREEN_TRANSFORM_COMPONENT_H
#define STARDUST_SCREEN_TRANSFORM_COMPONENT_H

#include "stardust/graphics/Graphics.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace components
    {
        struct ScreenTransform final
        {
            IVector2 translation = IVector2Zero;

            graphics::Reflection reflection = graphics::Reflection::None;

            f32 rotation = 0.0f;
            Optional<IVector2> pivot = None;

            Optional<Vector2> shear = None;
        };
    }
}

#endif
