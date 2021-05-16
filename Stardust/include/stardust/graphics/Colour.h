#pragma once
#ifndef STARDUST_COLOUR_H
#define STARDUST_COLOUR_H

#include <cstddef>
#include <functional>

#include <SDL2/SDL.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"

namespace stardust
{
    struct Colour
    {
        [[nodiscard]] static Colour Random();
        [[nodiscard]] static Colour RandomOpaque();

        u8 red;
        u8 green;
        u8 blue;

        u8 alpha;

        constexpr Colour()
            : red(0u), green(0u), blue(0u), alpha(SDL_ALPHA_OPAQUE)
        { }

        constexpr Colour(const u8 red, const u8 green, const u8 blue, const u8 alpha = SDL_ALPHA_OPAQUE)
            : red(red), green(green), blue(blue), alpha(alpha)
        { }

        constexpr Colour(const u32 red, const u32 green, const u32 blue, const u32 alpha = SDL_ALPHA_OPAQUE)
            : red(red), green(green), blue(blue), alpha(alpha)
        { }

        constexpr Colour(const f32 red, const f32 green, const f32 blue, const f32 alpha = 1.0f)
            : red(static_cast<u8>(red * 255.0f)), green(static_cast<u8>(green * 255.0f)), blue(static_cast<u8>(blue * 255.0f)), alpha(static_cast<u8>(alpha * 255.0f))
        { }

        constexpr Colour(const UVec3& values)
            : red(values.r), green(values.g), blue(values.b), alpha(SDL_ALPHA_OPAQUE)
        { }

        constexpr Colour(const UVec4& values)
            : red(values.r), green(values.g), blue(values.b), alpha(values.a)
        { }

        constexpr Colour(const Vec3& values)
            : Colour(values.r, values.g, values.b)
        { }

        constexpr Colour(const Vec4& values)
            : Colour(values.r, values.g, values.b, values.a)
        { }

        constexpr Colour(const SDL_Colour& colour)
            : red(colour.r), green(colour.g), blue(colour.b), alpha(colour.a)
        { }

        ~Colour() noexcept = default;

        [[nodiscard]] bool operator ==(const Colour&) const noexcept = default;
        [[nodiscard]] bool operator !=(const Colour&) const noexcept = default;

        [[nodiscard]] operator SDL_Colour() const noexcept;

        [[nodiscard]] explicit operator Vec3() const noexcept;
        [[nodiscard]] explicit operator Vec4() const noexcept;
    };

    struct HSVColour
    {
        [[nodiscard]] static HSVColour Random();
        [[nodiscard]] static HSVColour RandomOpaque();

        f32 hue;
        f32 saturation;
        f32 value;

        f32 alpha;

        constexpr HSVColour()
            : hue(0.0f), saturation(0.0f), value(0.0f), alpha(1.0f)
        { }

        constexpr HSVColour(const f32 hue, const f32 saturation, const f32 value, const f32 alpha = 1.0f)
            : hue(hue), saturation(saturation), value(value), alpha(alpha)
        { }

        constexpr HSVColour(const Vec3& values)
            : hue(values.r), saturation(values.g), value(values.b), alpha(1.0f)
        { }

        constexpr HSVColour(const Vec4& values)
            : hue(values.r), saturation(values.g), value(values.b), alpha(values.a)
        { }

        ~HSVColour() noexcept = default;

        [[nodiscard]] bool operator ==(const HSVColour&) const noexcept = default;
        [[nodiscard]] bool operator !=(const HSVColour&) const noexcept = default;

        [[nodiscard]] explicit operator Vec3() const noexcept;
        [[nodiscard]] explicit operator Vec4() const noexcept;
    };

    namespace colours
    {
        constexpr Colour Black{ 0u, 0u, 0u, 255u };
        constexpr Colour Red{ 255u, 0u, 0u, 255u };
        constexpr Colour Lime{ 0u, 255u, 0u, 255u };
        constexpr Colour Blue{ 0u, 0u, 255u, 255u };
        constexpr Colour Yellow{ 255u, 255u, 0u, 255u };
        constexpr Colour Magenta{ 255u, 0u, 255u, 255u };
        constexpr Colour Cyan{ 255u, 255u, 255u, 255u };
        constexpr Colour White{ 255u, 255u, 255u, 255u };

        constexpr Colour Silver{ 192u, 192u, 192u, 255u };
        constexpr Colour Grey{ 128u, 128u, 128u, 255u };
        constexpr Colour Maroon{ 128u, 0u, 0u, 255u };
        constexpr Colour Olive{ 128u, 128u, 0u, 255u };
        constexpr Colour Green{ 0u, 128u, 0u, 255u };
        constexpr Colour Teal{ 0u, 128u, 128u, 255u };
        constexpr Colour Navy{ 0u, 0u, 128u, 255u };
        constexpr Colour Purple{ 128u, 0u, 128u, 255u };

        constexpr Colour Orange{ 255u, 128u, 0u, 255u };
        constexpr Colour Brown{ 150u, 75u, 0u, 255u };
        constexpr Colour Pink{ 255u, 192u, 203u, 255u };
        constexpr Colour Tan{ 210u, 180u, 140u, 255u };
        constexpr Colour Gold{ 255u, 215u, 0u, 255u };
        constexpr Colour Azure{ 0u, 128u, 255u, 255u };
        constexpr Colour Beige{ 245u, 245u, 220u, 255u };
        constexpr Colour Lavender{ 230u, 230u, 255u, 255u };

        constexpr Colour Clear{ 0u, 0u, 0u, 0u };
    }

    [[nodiscard]] extern HSVColour RGBToHSV(const Colour& rgbColour);
    [[nodiscard]] extern Colour HSVToRGB(const HSVColour& hsvColour);
    [[nodiscard]] extern Colour HexToRGB(u32 colourHex, const u8 alpha = SDL_ALPHA_OPAQUE);
    [[nodiscard]] extern u32 RGBToHex(const Colour& rgbColour);
}

namespace std
{
    template <>
    struct hash<stardust::Colour>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::Colour& colour) const noexcept
        {
            std::size_t seed = 0u;
            std::hash<stardust::u8> hasher;

            glm::detail::hash_combine(seed, hasher(colour.red));
            glm::detail::hash_combine(seed, hasher(colour.green));
            glm::detail::hash_combine(seed, hasher(colour.blue));
            glm::detail::hash_combine(seed, hasher(colour.alpha));

            return seed;
        }
    };

    template <>
    struct hash<stardust::HSVColour>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::HSVColour& colour) const noexcept
        {
            std::size_t seed = 0u;
            std::hash<stardust::u8> hasher;

            glm::detail::hash_combine(seed, hasher(colour.hue));
            glm::detail::hash_combine(seed, hasher(colour.saturation));
            glm::detail::hash_combine(seed, hasher(colour.value));
            glm::detail::hash_combine(seed, hasher(colour.alpha));

            return seed;
        }
    };
}

[[nodiscard]] extern stardust::Colour operator ""_colour(const stardust::u64 hexCode);

#endif