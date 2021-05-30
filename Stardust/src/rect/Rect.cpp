#include "stardust/rect/Rect.h"

#include "stardust/math/Math.h"

namespace stardust
{
    [[nodiscard]] Rect Rect::GetMinimumEnslosingRect(const Vector<IVec2>& points)
    {
        Rect minimumEnslosingRect;
        SDL_EnclosePoints(reinterpret_cast<const SDL_Point*>(points.data()), static_cast<i32>(points.size()), nullptr, &minimumEnslosingRect.m_rect);

        return minimumEnslosingRect;
    }

    [[nodiscard]] Rect Rect::GetMinimumEnslosingRect(const Vector<IVec2>& points, const Rect& clippingRect)
    {
        Rect minimumEnslosingRect;
        SDL_EnclosePoints(reinterpret_cast<const SDL_Point*>(points.data()), static_cast<i32>(points.size()), &clippingRect.m_rect, &minimumEnslosingRect.m_rect);

        return minimumEnslosingRect;
    }

    Rect::Rect(const SDL_Rect& rect)
        : m_rect(rect)
    { }

    Rect::Rect(const IVec2& topLeft, const UVec2& size)
        : m_rect(SDL_Rect{
            .x = topLeft.x,
            .y = topLeft.y,
            .w = static_cast<i32>(size.x),
            .h = static_cast<i32>(size.y),
        })
    { }

    Rect::Rect(const i32 x, const i32 y, const u32 width, const u32 height)
        : m_rect(SDL_Rect{
            .x = x,
            .y = y,
            .w = static_cast<i32>(width),
            .h = static_cast<i32>(height),
        })
    { }
    
    [[nodiscard]] u32 Rect::GetArea() const noexcept
    {
        return static_cast<u32>(m_rect.w * m_rect.h);
    }

    [[nodiscard]] u32 Rect::GetPerimeter() const noexcept
    {
        return static_cast<u32>(2 * m_rect.w + 2 * m_rect.h);
    }

    [[nodiscard]] f32 Rect::GetDiagonal() const
    {
        return glm::sqrt(static_cast<f32>(m_rect.w * m_rect.w + m_rect.h + m_rect.h));
    }

    [[nodiscard]] bool Rect::IsEmpty() const noexcept
    {
        return SDL_RectEmpty(&m_rect) == SDL_TRUE;
    }

    [[nodiscard]] bool Rect::IsSquare() const noexcept
    {
        return m_rect.w == m_rect.h;
    }

    [[nodiscard]] bool Rect::IsPointEnclosed(const IVec2& point) const
    {
        const SDL_Point convertedPoint{
            .x = point.x,
            .y = point.y,
        };

        return SDL_PointInRect(&convertedPoint, &m_rect) == SDL_TRUE;
    }

    [[nodiscard]] bool Rect::IsAnyPointEnclosed(const Vector<IVec2>& points) const
    {
        return SDL_EnclosePoints(reinterpret_cast<const SDL_Point*>(points.data()), static_cast<i32>(points.size()), &m_rect, nullptr) == SDL_TRUE;
    }

    [[nodiscard]] bool Rect::HasIntersection(const Rect& other) const
    {
        return SDL_HasIntersection(&m_rect, &other.m_rect) == SDL_TRUE;
    }

    [[nodiscard]] bool Rect::HasIntersection(const Pair<IVec2, IVec2>& line) const
    {
        auto [pointA, pointB] = line;

        return SDL_IntersectRectAndLine(&m_rect, &pointA.x, &pointA.y, &pointB.x, &pointB.y) == SDL_TRUE;
    }

    [[nodiscard]] Optional<Rect> Rect::GetIntersection(const Rect& other) const
    {
        Rect rectIntersection;
        const bool doesIntersectionExist = SDL_IntersectRect(&m_rect, &other.m_rect, &rectIntersection.m_rect) == SDL_TRUE;

        return doesIntersectionExist ? Optional<Rect>(rectIntersection) : NullOpt;
    }

    [[nodiscard]] Rect Rect::GetUnion(const Rect& other) const
    {
        Rect rectUnion;
        SDL_UnionRect(&m_rect, &other.m_rect, &rectUnion.m_rect);

        return rectUnion;
    }

    [[nodiscard]] Pair<IVec2, IVec2> Rect::ClipLineToRect(const Pair<IVec2, IVec2>& line) const
    {
        auto lineCopy = line;
        SDL_IntersectRectAndLine(&m_rect, &lineCopy.first.x, &lineCopy.first.y, &lineCopy.second.x, &lineCopy.second.y);

        return lineCopy;
    }

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

    [[nodiscard]] IVec2 Rect::GetCentre() const noexcept
    {
        return IVec2{
            m_rect.x + static_cast<i32>(m_rect.w / 2u),
            m_rect.y + static_cast<i32>(m_rect.h / 2u),
        };
    }

    [[nodiscard]] bool Rect::operator ==(const Rect& other) const noexcept
    {
        return m_rect.x == other.m_rect.x && m_rect.y == other.m_rect.y && m_rect.w == other.m_rect.w && m_rect.h == other.m_rect.h;
    }

    [[nodiscard]] bool Rect::operator !=(const Rect& other) const noexcept
    {
        return !(*this == other);
    }
}