#include "stardust/graphics/RenderArea.h"

#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        [[nodiscard]] auto GetRenderAreaFromZone(const Window& window, const RenderZone renderZone) -> RenderArea
        {
            RenderArea renderArea{
                .bottomLeft = IVector2Zero,
                .size = window.GetDrawableSize(),
            };

            switch (renderZone)
            {
            case RenderZone::Whole:
                break;

            case RenderZone::TopHalf:
                renderArea.bottomLeft.y = static_cast<i32>(window.GetSize().y) / 2;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::LeftHalf:
                renderArea.size.x /= 2u;

                break;

            case RenderZone::RightHalf:
                renderArea.bottomLeft.x = static_cast<i32>(window.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomHalf:
                renderArea.size.y /= 2u;

                break;

            case RenderZone::TopLeftQuadrant:
                renderArea.bottomLeft.y = static_cast<i32>(window.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::TopRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(window.GetSize().x) / 2;
                renderArea.bottomLeft.y = static_cast<i32>(window.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomLeftQuadrant:
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;

                break;

            case RenderZone::BottomRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(window.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.x;

                break;
            }

            return renderArea;
        }
        
        [[nodiscard]] auto GetRenderAreaFromZone(const Texture& texture, const RenderZone renderZone) -> RenderArea
        {
            RenderArea renderArea{
                .bottomLeft = IVector2Zero,
                .size = texture.GetSize(),
            };

            switch (renderZone)
            {
            case RenderZone::Whole:
                break;

            case RenderZone::TopHalf:
                renderArea.bottomLeft.y = static_cast<i32>(texture.GetSize().y) / 2;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::LeftHalf:
                renderArea.size.x /= 2u;

                break;

            case RenderZone::RightHalf:
                renderArea.bottomLeft.x = static_cast<i32>(texture.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomHalf:
                renderArea.size.y /= 2u;

                break;

            case RenderZone::TopLeftQuadrant:
                renderArea.bottomLeft.y = static_cast<i32>(texture.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::TopRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(texture.GetSize().x) / 2;
                renderArea.bottomLeft.y = static_cast<i32>(texture.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomLeftQuadrant:
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;

                break;

            case RenderZone::BottomRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(texture.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.x;

                break;
            }

            return renderArea;
        }

        [[nodiscard]] auto GetRenderAreaFromZone(const RenderBuffer& renderBuffer, const RenderZone renderZone) -> RenderArea
        {
            RenderArea renderArea{
                .bottomLeft = IVector2Zero,
                .size = renderBuffer.GetSize(),
            };

            switch (renderZone)
            {
            case RenderZone::Whole:
                break;

            case RenderZone::TopHalf:
                renderArea.bottomLeft.y = static_cast<i32>(renderBuffer.GetSize().y) / 2;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::LeftHalf:
                renderArea.size.x /= 2u;

                break;

            case RenderZone::RightHalf:
                renderArea.bottomLeft.x = static_cast<i32>(renderBuffer.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomHalf:
                renderArea.size.y /= 2u;

                break;

            case RenderZone::TopLeftQuadrant:
                renderArea.bottomLeft.y = static_cast<i32>(renderBuffer.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;

                break;

            case RenderZone::TopRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(renderBuffer.GetSize().x) / 2;
                renderArea.bottomLeft.y = static_cast<i32>(renderBuffer.GetSize().y) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.y;
                ++renderArea.size.x;

                break;

            case RenderZone::BottomLeftQuadrant:
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;

                break;

            case RenderZone::BottomRightQuadrant:
                renderArea.bottomLeft.x = static_cast<i32>(renderBuffer.GetSize().x) / 2;
                renderArea.size.x /= 2u;
                renderArea.size.y /= 2u;
                ++renderArea.size.x;

                break;
            }

            return renderArea;
        }
    }
}
