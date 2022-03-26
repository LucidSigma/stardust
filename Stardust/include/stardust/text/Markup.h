#pragma once
#ifndef STARDUST_MARKUP_H
#define STARDUST_MARKUP_H

#include <functional>

#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/math/Math.h"
#include "stardust/text/font/Font.h"
#include "stardust/text/localisation/Localisation.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class TextAlignment
    {
        Left,
        Centre,
        Right,
    };

    struct TextOutline final
    {
        f32 thickness = 0.0f;
        Colour colour = colours::White;

        IVector2 offset = IVector2Zero;

        [[nodiscard]] auto operator ==(const TextOutline&) const noexcept -> bool = default;
        [[nodiscard]] auto operator !=(const TextOutline&) const noexcept -> bool = default;
    };

    struct TextDropShadow final
    {
        Optional<f32> thickness = None;
        Colour colour = colours::White;

        IVector2 offset = IVector2Zero;

        [[nodiscard]] auto operator ==(const TextDropShadow&) const noexcept -> bool = default;
        [[nodiscard]] auto operator !=(const TextDropShadow&) const noexcept -> bool = default;
    };

    struct TextDecorativeLine final
    {
        f32 thickness = 0.0f;
        Colour colour = colours::White;

        IVector2 offset = IVector2Zero;

        [[nodiscard]] auto operator ==(const TextDecorativeLine&) const noexcept -> bool = default;
        [[nodiscard]] auto operator !=(const TextDecorativeLine&) const noexcept -> bool = default;
    };

    struct Markup final
    {
        Colour colour = colours::White;
        TextAlignment textAlignment = TextAlignment::Left;

        Optional<TextOutline> outline = None;
        Optional<TextDropShadow> dropShadow = None;

        Optional<TextDecorativeLine> underline = None;
        Optional<TextDecorativeLine> strikethrough = None;
        Optional<TextDecorativeLine> overline = None;

        Optional<u32> wrapLength = None;

        localisation::TextLocalisationInfo localisation{ };
        Font::RenderMode defaultRenderMode = Font::RenderMode::Normal;

        [[nodiscard]] auto operator ==(const Markup&) const noexcept -> bool = default;
        [[nodiscard]] auto operator !=(const Markup&) const noexcept -> bool = default;
    };
}

namespace std
{
    template <>
    struct hash<stardust::TextOutline> final
    {
        [[nodiscard]] auto operator ()(const stardust::TextOutline& outline) const noexcept -> stardust::usize;
    };

    template <>
    struct hash<stardust::TextDropShadow> final
    {
        [[nodiscard]] auto operator ()(const stardust::TextDropShadow& dropShadow) const noexcept -> stardust::usize;
    };

    template <>
    struct hash<stardust::TextDecorativeLine> final
    {
        [[nodiscard]] auto operator ()(const stardust::TextDecorativeLine& decorativeLine) const noexcept -> stardust::usize;
    };

    template <>
    struct hash<stardust::Markup> final
    {
        [[nodiscard]] auto operator ()(const stardust::Markup& markup) const noexcept -> stardust::usize;
    };
}

#endif
