#pragma once
#ifndef STARDUST_COLOUR_H
#define STARDUST_COLOUR_H

#include <functional>

#include <SDL2/SDL.h>

#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    struct Colour final
    {
        struct HSV final
        {
            f32 hue;
            f32 saturation;
            f32 value;
        };

        static constexpr u8 TransparentChannel = 0x00u;
        static constexpr u8 OpaqueChannel = 0xFFu;

        u8 red = 0u;
        u8 green = 0u;
        u8 blue = 0u;

        u8 alpha = 0u;

        [[nodiscard]] static auto Lighten(const Colour& colour, const f32 percentage) -> Colour;
        [[nodiscard]] static auto Darken(const Colour& colour, const f32 percentage) -> Colour;

        constexpr Colour() = default;

        constexpr Colour(const u8 red, const u8 green, const u8 blue, const u8 alpha = OpaqueChannel)
            : red(red), green(green), blue(blue), alpha(alpha)
        { }

        constexpr Colour(const u32 red, const u32 green, const u32 blue, const u32 alpha = OpaqueChannel)
            : red(static_cast<u8>(red)), green(static_cast<u8>(green)), blue(static_cast<u8>(blue)), alpha(static_cast<u8>(alpha))
        { }

        constexpr Colour(const f32 red, const f32 green, const f32 blue, const f32 alpha = 1.0f)
            : red(static_cast<u8>(red * 255.0f)),
              green(static_cast<u8>(green * 255.0f)),
              blue(static_cast<u8>(blue * 255.0f)),
              alpha(static_cast<u8>(alpha * 255.0f))
        { }

        explicit constexpr Colour(u32 hexCode, const u32 alpha = OpaqueChannel)
            : alpha(static_cast<u8>(alpha))
        {
            constexpr u32 Bitmask = 0xFFu;

            blue = static_cast<u8>(hexCode & Bitmask);
            hexCode >>= 8u;

            green = static_cast<u8>(hexCode & Bitmask);
            hexCode >>= 8u;

            red = static_cast<u8>(hexCode & Bitmask);
        }

        explicit Colour(const HSV& hsv, const u32 alpha = OpaqueChannel);

        explicit constexpr Colour(const UVector3 values)
            : red(values.r), green(values.g), blue(values.b), alpha(OpaqueChannel)
        { }

        explicit constexpr Colour(const UVector4 values)
            : red(values.r), green(values.g), blue(values.b), alpha(values.a)
        { }

        explicit constexpr Colour(const Vector3 values)
            : Colour(values.r, values.g, values.b)
        { }

        explicit constexpr Colour(const Vector4 values)
            : Colour(values.r, values.g, values.b, values.a)
        { }

        explicit constexpr Colour(const SDL_Colour& colour)
            : red(colour.r), green(colour.g), blue(colour.b), alpha(colour.a)
        { }

        [[nodiscard]] auto GetHex() const noexcept -> u32;
        [[nodiscard]] auto GetHSV() const noexcept -> HSV;

        [[nodiscard]] auto operator ==(const Colour&) const noexcept -> bool = default;
        [[nodiscard]] auto operator !=(const Colour&) const noexcept -> bool = default;

        [[nodiscard]] operator SDL_Colour() const noexcept;

        [[nodiscard]] explicit operator Vector3() const noexcept;
        [[nodiscard]] explicit operator Vector4() const noexcept;
    };

    inline namespace literals
    {
        [[nodiscard]] extern auto operator ""_colour(const u64 hexCode) -> Colour;
    }
}

namespace std
{
    template <>
    struct hash<stardust::Colour> final
    {
        [[nodiscard]] auto operator ()(const stardust::Colour& colour) const noexcept -> stardust::usize;
    };
}

#endif
