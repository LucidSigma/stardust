#include "stardust/graphics/colour/Colour.h"

#include "stardust/math/Math.h"

namespace stardust
{
    [[nodiscard]] auto Colour::Lighten(const Colour& colour, const f32 percentage) -> Colour
    {
        Colour::HSV hsv = colour.GetHSV();
        hsv.saturation = glm::lerp(hsv.saturation, 0.0f, glm::clamp(percentage, 0.0f, 1.0f));

        return Colour(hsv);
    }

    [[nodiscard]] auto Colour::Darken(const Colour& colour, const f32 percentage) -> Colour
    {
        Colour::HSV hsv = colour.GetHSV();
        hsv.value = glm::lerp(hsv.value, 0.0f, glm::clamp(percentage, 0.0f, 1.0f));

        return Colour(hsv);
    }

    Colour::Colour(const HSV& hsv, const u32 alpha)
        : alpha(static_cast<u8>(alpha))
    {
        const Vector3 rgbValues = glm::rgbColor(Vector3{ hsv.hue, hsv.saturation, hsv.value });

        red = static_cast<u8>(rgbValues.r * 255.0f);
        green = static_cast<u8>(rgbValues.g * 255.0f);
        blue = static_cast<u8>(rgbValues.b * 255.0f);
    }

    [[nodiscard]] auto Colour::GetHex() const noexcept -> u32
    {
        u32 colourHex = 0u;

        colourHex |= static_cast<u32>(red);
        colourHex <<= 8u;

        colourHex |= static_cast<u32>(green);
        colourHex <<= 8u;

        colourHex |= static_cast<u32>(blue);

        return colourHex;
    }

    [[nodiscard]] auto Colour::GetHSV() const noexcept -> HSV
    {
        const Vector3 hsvColour = glm::hsvColor(Vector3(*this));

        return HSV{
            .hue = hsvColour.r,
            .saturation = hsvColour.g,
            .value = hsvColour.b,
        };
    }

    [[nodiscard]] Colour::operator SDL_Colour() const noexcept
    {
        return SDL_Colour{
            .r = red,
            .g = green,
            .b = blue,
            .a = alpha,
        };
    }

    [[nodiscard]] Colour::operator Vector3() const noexcept
    {
        return Vector3{
            static_cast<f32>(red) / 255.0f,
            static_cast<f32>(green) / 255.0f,
            static_cast<f32>(blue) / 255.0f,
        };
    }

    [[nodiscard]] Colour::operator Vector4() const noexcept
    {
        return Vector4{
            static_cast<f32>(red) / 255.0f,
            static_cast<f32>(green) / 255.0f,
            static_cast<f32>(blue) / 255.0f,
            static_cast<f32>(alpha) / 255.0f,
        };
    }

    inline namespace literals
    {
        [[nodiscard]] auto operator ""_colour(const u64 hexCode) -> Colour
        {
            return Colour(static_cast<u32>(hexCode));
        }
    }
}

namespace std
{
    [[nodiscard]] auto hash<stardust::Colour>::operator ()(const stardust::Colour& colour) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;
        std::hash<stardust::u8> hasher;

        glm::detail::hash_combine(seed, hasher(colour.red));
        glm::detail::hash_combine(seed, hasher(colour.green));
        glm::detail::hash_combine(seed, hasher(colour.blue));
        glm::detail::hash_combine(seed, hasher(colour.alpha));

        return seed;
    }
}
