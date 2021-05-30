#include "stardust/rect/Rect.h"

namespace stardust
{
    [[nodiscard]] IVec2 Rect::GetTopLeft() const noexcept
    {
        return IVec2{
            m_rect.x,
            m_rect.y,
        };
    }

    [[nodiscard]] IVec2 Rect::GetBottomRight() const noexcept
    {
        return IVec2{
            m_rect.x + m_rect.w,
            m_rect.y + m_rect.h,
        };
    }
}