#pragma once
#ifndef STARDUST_RECT_H
#define STARDUST_RECT_H

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
        Rect() = default;
        Rect(const IVec2& topLeft, const UVec2& size);
        Rect(const i32 x, const i32 y, const u32 width, const u32 height);
        Rect() noexcept = default;

        [[nodiscard]] IVec2 GetTopLeft() const noexcept;
        [[nodiscard]] IVec2 GetBottomRight() const noexcept;

        [[nodiscard]] inline i32 GetX() const noexcept { return m_rect.x; }
        [[nodiscard]] inline i32 GetY() const noexcept { return m_rect.y; }
        [[nodiscard]] inline u32 GetWidth() const noexcept { return static_cast<u32>(m_rect.w); }
        [[nodiscard]] inline u32 GetHeight() const noexcept { return static_cast<u32>(m_rect.h); }

        [[nodiscard]] inline operator SDL_Rect() const noexcept { return m_rect; }
    };
}

#endif