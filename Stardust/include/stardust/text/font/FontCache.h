#pragma once
#ifndef STARDUST_FONT_CACHE_H
#define STARDUST_FONT_CACHE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/graphics/texture/Sampler.h"
#include "stardust/text/font/Font.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class FontCache final
        : private INoncopyable
    {
    public:
        struct FontData final
        {
            String filepath;

            Font::RenderMode renderMode = Font::RenderMode::Normal;
            f32 outlineThickness = 0.0f;

            UVector2 textureAtlasSize{ 512u, 512u };
            u32 textureAtlasDepth = 4u;

            graphics::Sampler sampler = graphics::DefaultSampler;
        };

    private:
        HashMap<Font::Size, UniquePointer<Font>> m_pointSizes{ };

        FontData m_fontData{ };

    public:
        FontCache() = default;
        explicit FontCache(const FontData& fontData);

        FontCache(FontCache&&) noexcept = default;
        auto operator =(FontCache&&) noexcept -> FontCache& = default;

        ~FontCache() noexcept = default;

        auto Initialise(const FontData& fontData) -> void;

        [[nodiscard]] auto Add(const Font::Size pointSize) -> Status;
        [[nodiscard]] auto Add(const List<Font::Size>& pointSizes) -> Status;
        [[nodiscard]] auto Get(const Font::Size pointSize) -> Font&;
        auto Remove(const Font::Size pointSize) -> void;

        [[nodiscard]] inline auto operator [](const Font::Size pointSize) -> Font& { return Get(pointSize); }

        [[nodiscard]] inline auto Has(const Font::Size pointSize) const -> bool { return m_pointSizes.contains(pointSize); }
    };
}

#endif
