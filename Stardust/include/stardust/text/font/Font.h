#pragma once
#ifndef STARDUST_FONT_H
#define STARDUST_FONT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    using FontSize = u32;

    class Font
        : private INoncopyable
    {
    public:
        enum class Style
            : decltype(TTF_STYLE_NORMAL)
        {
            Normal = TTF_STYLE_NORMAL,
            Bold = TTF_STYLE_BOLD,
            Italic = TTF_STYLE_ITALIC,
            Underline = TTF_STYLE_UNDERLINE,
            Strikethrough = TTF_STYLE_STRIKETHROUGH,
        };

        enum class Hinting
            : decltype(TTF_HINTING_NORMAL)
        {
            Normal = TTF_HINTING_NORMAL,
            Light = TTF_HINTING_LIGHT,
            Monochrome = TTF_HINTING_MONO,
            None = TTF_HINTING_NONE,
        };

        struct GlyphMetrics
        {
            IVec2 minOffset;
            IVec2 maxOffset;

            i32 advance;
        };

    private:
        struct FontDestroyer
        {
            void operator ()(TTF_Font* const font) const noexcept;
        };

        UniquePtr<TTF_Font, FontDestroyer> m_handle = nullptr;
        Vector<ubyte> m_fontFileData{ };
        SDL_RWops* m_fontFileRWOps = nullptr;

        FontSize m_pointSize = 0u;

    public:
        Font() = default;
        Font(const StringView& fontFilepath, const FontSize pointSize);

        Font(Font&& other) noexcept;
        Font& operator =(Font&& other) noexcept;

        ~Font() noexcept;

        void Initialise(const StringView& fontFilepath, const FontSize pointSize);
        void Destroy() noexcept;

        inline bool IsValid() const noexcept { return m_handle != nullptr; }

        FontSize GetPointSize() const noexcept { return m_pointSize; }

        [[nodiscard]] Vector<Style> GetStyles() const;
        void SetStyles(const Vector<Style>& styles) const;
        void ClearStyles() const;

        [[nodiscard]] u32 GetOutlineThickness() const;
        void SetOutlineThickness(const u32 outlineThickness) const;
        void RemoveOutline() const;

        [[nodiscard]] Hinting GetHinting() const;
        void SetHinting(const Hinting hinting) const;

        [[nodiscard]] u32 GetKerning() const;
        void SetKerning(const u32 kerning) const;
        [[nodiscard]] u32 GetKerningBetweenGlyphs(const char16_t leftGlyph, const char16_t rightGlyph) const;

        [[nodiscard]] u32 GetMaximumHeight() const;
        [[nodiscard]] i32 GetFontAscent() const;
        [[nodiscard]] i32 GetFontDescent() const;
        [[nodiscard]] u32 GetLineSkip() const;

        [[nodiscard]] bool IsFixedWidth() const;

        [[nodiscard]] Optional<usize> GetGlyphIndex(const char16_t glyph) const;
        [[nodiscard]] bool DoesGlyphExist(const char16_t glyph) const;
        [[nodiscard]] GlyphMetrics GetGlyphMetrics(const char16_t glyph) const;

        [[nodiscard]] UVec2 GetTextSize(const String& text) const;
        [[nodiscard]] UVec2 GetTextSize(const UTF16String& text) const;

        [[nodiscard]] inline TTF_Font* const GetRawHandle() const noexcept { return m_handle.get(); }
    };
}

#endif