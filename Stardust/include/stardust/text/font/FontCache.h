#pragma once
#ifndef STARDUST_FONT_CACHE_H
#define STARDUST_FONT_CACHE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/Containers.h"
#include "stardust/text/font/Font.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    class FontCache
        : private INoncopyable
    {
    private:
        HashMap<FontSize, Font> m_pointSizes{ };

        String m_filepath;
        
    public:
        FontCache() = default;
        explicit FontCache(const StringView& fontFilepath);
        ~FontCache() noexcept = default;

        void SetFont(const StringView& fontFilepath);

        [[nodiscard]] Status Add(const FontSize pointSize);
        [[nodiscard]] Status Add(const Vector<FontSize>& pointSizes);
        [[nodiscard]] Font& Get(const FontSize pointSize);
        void Remove(const FontSize pointSize);

        [[nodiscard]] inline Font& operator [](const FontSize pointSize) { return Get(pointSize); }

        [[nodiscard]] inline bool Has(const FontSize pointSize) const { return m_pointSizes.contains(pointSize); }
    };
}

#endif