#include "stardust/ui/UI.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace ui
    {
        [[nodiscard]] auto GetRelativePositionFromAnchor(const Anchor anchor, const UVector2 parentSize, const UVector2 componentSize, const IVector2 anchorOffset) -> IVector2
        {
            switch (anchor)
            {
            case Anchor::TopLeft:
                return IVector2Zero + anchorOffset;

            case Anchor::TopCentre:
                return IVector2{
                    static_cast<i32>(parentSize.x / 2 - componentSize.x / 2),
                    0,
                } + anchorOffset;

            case Anchor::TopRight:
                return IVector2{
                    static_cast<i32>(parentSize.x - componentSize.x),
                    0,
                } + anchorOffset;

            case Anchor::CentreLeft:
                return IVector2{
                    0,
                    static_cast<i32>(parentSize.y / 2 - componentSize.y / 2),
                } + anchorOffset;

            case Anchor::Centre:
                return IVector2{
                    static_cast<i32>(parentSize.x / 2 - componentSize.x / 2),
                    static_cast<i32>(parentSize.y / 2 - componentSize.y / 2),
                } + anchorOffset;

            case Anchor::CentreRight:
                return IVector2{
                    static_cast<i32>(parentSize.x - componentSize.x),
                    static_cast<i32>(parentSize.y / 2 - componentSize.y / 2),
                } + anchorOffset;

            case Anchor::BottomLeft:
                return IVector2{
                    0,
                    static_cast<i32>(parentSize.y - componentSize.y),
                } + anchorOffset;

            case Anchor::BottomCentre:
                return IVector2{
                    static_cast<i32>(parentSize.x / 2 - componentSize.x / 2),
                    static_cast<i32>(parentSize.y - componentSize.y),
                } + anchorOffset;

            case Anchor::BottomRight:
                return IVector2{
                    static_cast<i32>(parentSize.x - componentSize.x),
                    static_cast<i32>(parentSize.y - componentSize.y),
                } + anchorOffset;

            [[unlikely]] default:
                return IVector2Zero;
            }
        }

        [[nodiscard]] auto GetScreenPositionFromAnchor(const Anchor anchor, const Window& window, const UVector2 componentSize, const IVector2 anchorOffset) -> IVector2
        {
            const UVector2 screenSize = window.GetDrawableSize();

            return GetRelativePositionFromAnchor(anchor, screenSize, componentSize, anchorOffset);
        }

        auto GetVirtualScreenPositionFromAnchor(const Anchor anchor, const Camera2D& camera, const UVector2 componentSize, const IVector2 anchorOffset) -> IVector2
        {
            const UVector2 virtualScreenSize = camera.GetVirtualScreenSize();

            return GetRelativePositionFromAnchor(anchor, virtualScreenSize, componentSize, anchorOffset);
        }
        
        [[nodiscard]] auto DoesRectangleContainPoint(const geometry::ScreenRectangle& rectangle, const geometry::ScreenPoint point) -> bool
        {
            const SDL_Point convertedPoint{
                .x = point.x,
                .y = point.y,
            };

            const SDL_Rect convertedRectangle{
                .x = rectangle.topLeft.x,
                .y = rectangle.topLeft.y,
                .w = static_cast<i32>(rectangle.size.x),
                .h = static_cast<i32>(rectangle.size.y),
            };

            return SDL_PointInRect(&convertedPoint, &convertedRectangle) == SDL_TRUE;
        }

        [[nodiscard]] auto DoesRectanglesHaveIntersection(const geometry::ScreenRectangle& rectangleA, const geometry::ScreenRectangle& rectangleB) -> bool
        {
            const SDL_Rect convertedRectangleA{
                .x = rectangleA.topLeft.x,
                .y = rectangleA.topLeft.y,
                .w = static_cast<i32>(rectangleA.size.x),
                .h = static_cast<i32>(rectangleA.size.y),
            };

            const SDL_Rect convertedRectangleB{
                .x = rectangleB.topLeft.x,
                .y = rectangleB.topLeft.y,
                .w = static_cast<i32>(rectangleB.size.x),
                .h = static_cast<i32>(rectangleB.size.y),
            };

            return SDL_HasIntersection(&convertedRectangleA, &convertedRectangleB) == SDL_TRUE;
        }
    }
}
