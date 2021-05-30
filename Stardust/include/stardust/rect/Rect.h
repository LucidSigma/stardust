#pragma once
#ifndef STARDUST_RECT_H
#define STARDUST_RECT_H

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

#include <SDL2/SDL.h>

namespace stardust
{
    class Rect
    {
    private:
        SDL_Rect m_rect{ };

    public:
        [[nodiscard]] static Rect GetMinimumEnslosingRect(const Vector<IVec2>& points);
        [[nodiscard]] static Rect GetMinimumEnslosingRect(const Vector<IVec2>& points, const Rect& clippingRect);

        Rect() = default;
        Rect(const SDL_Rect& rect);
        Rect(const IVec2& topLeft, const UVec2& size);
        Rect(const i32 x, const i32 y, const u32 width, const u32 height);
        ~Rect() noexcept = default;

        [[nodiscard]] u32 GetArea() const noexcept;
        [[nodiscard]] u32 GetPerimeter() const noexcept;
        [[nodiscard]] f32 GetDiagonal() const;

        [[nodiscard]] bool IsEmpty() const noexcept;
        [[nodiscard]] bool IsSquare() const noexcept;

        [[nodiscard]] bool IsPointEnclosed(const IVec2& point) const;
        [[nodiscard]] bool IsAnyPointEnclosed(const Vector<IVec2>& points) const;

        [[nodiscard]] bool HasIntersection(const Rect& other) const;
        [[nodiscard]] bool HasIntersection(const Pair<IVec2, IVec2>& line) const;

        [[nodiscard]] Optional<Rect> GetIntersection(const Rect& other) const;
        [[nodiscard]] Rect GetUnion(const Rect& other) const;
        [[nodiscard]] Pair<IVec2, IVec2> ClipLineToRect(const Pair<IVec2, IVec2>& line) const;

        [[nodiscard]] IVec2 GetTopLeft() const noexcept;
        [[nodiscard]] IVec2 GetBottomRight() const noexcept;
        [[nodiscard]] IVec2 GetCentre() const noexcept;

        [[nodiscard]] inline i32 GetX() const noexcept { return m_rect.x; }
        [[nodiscard]] inline void SetX(const i32 x) noexcept { m_rect.x = x; }

        [[nodiscard]] inline i32 GetY() const noexcept { return m_rect.y; }
        [[nodiscard]] inline void SetY(const i32 y) noexcept { m_rect.y = y; }

        [[nodiscard]] inline u32 GetWidth() const noexcept { return static_cast<u32>(m_rect.w); }
        [[nodiscard]] inline void SetWidth(const u32 width) noexcept { m_rect.w = static_cast<i32>(width); }

        [[nodiscard]] inline u32 GetHeight() const noexcept { return static_cast<u32>(m_rect.h); }
        [[nodiscard]] inline void SetHeight(const u32 height) noexcept { m_rect.h = static_cast<i32>(height); }

        [[nodiscard]] bool operator ==(const Rect& other) const noexcept;
        [[nodiscard]] bool operator !=(const Rect& other) const noexcept;

        [[nodiscard]] inline SDL_Rect& GetRawRect() noexcept { return m_rect; }
        [[nodiscard]] inline const SDL_Rect& GetRawRect() const noexcept { return m_rect; }
        [[nodiscard]] inline operator SDL_Rect() const noexcept { return m_rect; }
    };
}

#endif