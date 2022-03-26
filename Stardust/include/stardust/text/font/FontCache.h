#pragma once
#ifndef STARDUST_FONT_CACHE_H
#define STARDUST_FONT_CACHE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/text/font/Font.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class FontCache final
        : private INoncopyable
    {
    private:
        HashMap<Font::Size, UniquePointer<Font>> m_pointSizes{ };

        Font::CreateInfo m_createInfo{ };

    public:
        FontCache() = default;
        explicit FontCache(const Font::CreateInfo& createInfo);

        FontCache(FontCache&&) noexcept = default;
        auto operator =(FontCache&&) noexcept -> FontCache& = default;

        ~FontCache() noexcept = default;

        auto SetFontData(const Font::CreateInfo& createInfo) -> void;

        [[nodiscard]] auto Add(const Font::Size pointSize) -> Status;
        [[nodiscard]] auto Add(const List<Font::Size>& pointSizes) -> Status;
        [[nodiscard]] auto Get(const Font::Size pointSize) -> Font&;
        auto Remove(const Font::Size pointSize) -> void;

        [[nodiscard]] inline auto operator [](const Font::Size pointSize) -> Font& { return Get(pointSize); }

        [[nodiscard]] inline auto Has(const Font::Size pointSize) const -> bool { return m_pointSizes.contains(pointSize); }
    };
}

#endif
