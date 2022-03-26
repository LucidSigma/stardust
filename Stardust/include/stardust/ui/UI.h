#pragma once
#ifndef STARDUST_UI_H
#define STARDUST_UI_H

#include "stardust/camera/Camera2D.h"
#include "stardust/geometry/Shapes.h"
#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/window/Window.h"

namespace stardust
{
    namespace ui
    {
        enum class Anchor
        {
            TopLeft,
            TopCentre,
            TopRight,
            CentreLeft,
            Centre,
            CentreRight,
            BottomLeft,
            BottomCentre,
            BottomRight,
        };

        [[nodiscard]] extern auto GetRelativePositionFromAnchor(const Anchor anchor, const UVector2 parentSize, const UVector2 componentSize, const IVector2 anchorOffset = IVector2Zero) -> IVector2;
        [[nodiscard]] extern auto GetScreenPositionFromAnchor(const Anchor anchor, const Window& window, const UVector2 componentSize, const IVector2 anchorOffset = IVector2Zero) -> IVector2;
        [[nodiscard]] extern auto GetVirtualScreenPositionFromAnchor(const Anchor anchor, const Camera2D& camera, const UVector2 componentSize, const IVector2 anchorOffset = IVector2Zero) -> IVector2;

        [[nodiscard]] extern auto DoesRectangleContainPoint(const geometry::ScreenRectangle& rectangle, const geometry::ScreenPoint point) -> bool;
        [[nodiscard]] extern auto DoesRectanglesHaveIntersection(const geometry::ScreenRectangle& rectangleA, const geometry::ScreenRectangle& rectangleB) -> bool;
    }
}

#endif
