#pragma once
#ifndef STARDUST_RENDER_AREA_H
#define STARDUST_RENDER_AREA_H

#include "stardust/graphics/framebuffer/RenderBuffer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/window/Window.h"

namespace stardust
{
    namespace graphics
    {
        struct RenderArea final
        {
            IVector2 bottomLeft = IVector2Zero;
            UVector2 size = UVector2Zero;
        };

        enum class RenderZone
        {
            Whole,
            LeftHalf,
            RightHalf,
            TopHalf,
            BottomHalf,
            TopLeftQuadrant,
            TopRightQuadrant,
            BottomLeftQuadrant,
            BottomRightQuadrant,
        };

        [[nodiscard]] extern auto GetRenderAreaFromZone(const Window& window, const RenderZone renderZone) -> RenderArea;
        [[nodiscard]] extern auto GetRenderAreaFromZone(const Texture& texture, const RenderZone renderZone) -> RenderArea;
        [[nodiscard]] extern auto GetRenderAreaFromZone(const RenderBuffer& renderBuffer, const RenderZone renderZone) -> RenderArea;
    }
}

#endif
