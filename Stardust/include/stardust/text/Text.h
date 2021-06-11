#pragma once
#ifndef STRARDUST_TEXT_H
#define STRARDUST_TEXT_H

#include <cstddef>
#include <functional>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/text/font/Font.h"

namespace stardust
{
    namespace text
    {
        struct OutlineInfo
        {
            u32 thickness;

            Colour innerColour;
            Colour outerColour;

            [[nodiscard]] bool operator ==(const OutlineInfo&) const noexcept = default;
            [[nodiscard]] bool operator !=(const OutlineInfo&) const noexcept = default;
        };

        struct TextInfo
        {
            String text;

            Optional<OutlineInfo> outline = NullOpt;
            Optional<u32> wrapLength = NullOpt;

            [[nodiscard]] bool operator ==(const TextInfo&) const noexcept = default;
            [[nodiscard]] bool operator !=(const TextInfo&) const noexcept = default;
        };

        struct UTF16TextInfo
        {
            UTF16String text;

            Optional<OutlineInfo> outline = NullOpt;
            Optional<u32> wrapLength = NullOpt;

            [[nodiscard]] bool operator ==(const UTF16TextInfo&) const noexcept = default;
            [[nodiscard]] bool operator !=(const UTF16TextInfo&) const noexcept = default;
        };

        struct GlyphInfo
        {
            char glyph;
            Optional<OutlineInfo> outline = NullOpt;

            [[nodiscard]] bool operator ==(const GlyphInfo&) const noexcept = default;
            [[nodiscard]] bool operator !=(const GlyphInfo&) const noexcept = default;
        };

        struct UTF16GlyphInfo
        {
            char16_t glyph;
            Optional<OutlineInfo> outline = NullOpt;

            [[nodiscard]] bool operator ==(const UTF16GlyphInfo&) const noexcept = default;
            [[nodiscard]] bool operator !=(const UTF16GlyphInfo&) const noexcept = default;
        };

        [[nodiscard]] extern Texture RenderGlyph(const Font& font, const GlyphInfo& glyphInfo, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderGlyph(const Font& font, const UTF16GlyphInfo& glyphInfo, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderText(const Font& font, const TextInfo& textInfo, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderText(const Font& font, const UTF16TextInfo& textInfo, const Sampler& sampler = Sampler{ });

        [[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderTextQuick(const Font& font, const String& text, const Colour& colour, const Sampler& sampler = Sampler{ });
        [[nodiscard]] extern Texture RenderTextQuick(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler = Sampler{ });
    }
}

namespace std
{
    template <>
    struct hash<stardust::text::OutlineInfo>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::text::OutlineInfo& outlineInfo) const noexcept
        {
            std::size_t seed = 0u;

            glm::detail::hash_combine(seed, std::hash<stardust::u32>()(outlineInfo.thickness));
            glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(outlineInfo.innerColour));
            glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(outlineInfo.outerColour));

            return seed;
        }
    };

    template <>
    struct hash<stardust::text::TextInfo>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::text::TextInfo& textInfo) const noexcept
        {
            std::size_t seed = 0u;

            glm::detail::hash_combine(seed, std::hash<stardust::String>()(textInfo.text));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::text::OutlineInfo>>()(textInfo.outline));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::u32>>()(textInfo.wrapLength));

            return seed;
        }
    };

    template <>
    struct hash<stardust::text::UTF16TextInfo>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::text::UTF16TextInfo& textInfo) const noexcept
        {
            std::size_t seed = 0u;

            glm::detail::hash_combine(seed, std::hash<stardust::UTF16String>()(textInfo.text));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::text::OutlineInfo>>()(textInfo.outline));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::u32>>()(textInfo.wrapLength));

            return seed;
        }
    };

    template <>
    struct hash<stardust::text::GlyphInfo>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::text::GlyphInfo& glyphInfo) const noexcept
        {
            std::size_t seed = 0u;

            glm::detail::hash_combine(seed, std::hash<char>()(glyphInfo.glyph));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::text::OutlineInfo>>()(glyphInfo.outline));

            return seed;
        }
    };

    template <>
    struct hash<stardust::text::UTF16GlyphInfo>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::text::UTF16GlyphInfo& glyphInfo) const noexcept
        {
            std::size_t seed = 0u;

            glm::detail::hash_combine(seed, std::hash<char32_t>()(glyphInfo.glyph));
            glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::text::OutlineInfo>>()(glyphInfo.outline));

            return seed;
        }
    };
}

#endif