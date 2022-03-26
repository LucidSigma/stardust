#pragma once
#ifndef STARDUST_TEXT_CACHE_H
#define STARDUST_TEXT_CACHE_H

#include "stardust/math/Math.h"
#include "stardust/text/Markup.h"
#include "stardust/text/font/Font.h"
#include "stardust/text/TextWriter.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    class TextCache final
    {
    private:
        ObserverPointer<TextWriter> m_textWriter = nullptr;
        ObserverPointer<const Font> m_font = nullptr;
        UVector2 m_currentFontTextureAtlasSize = UVector2Zero;

        HashMap<String, HashMap<Markup, List<GlyphRenderInfo>>> m_glyphs{ };

    public:
        TextCache() = default;
        explicit TextCache(TextWriter& textWriter);
        ~TextCache() noexcept = default;

        auto Initialise(TextWriter& textWriter) -> void;

        [[nodiscard]] auto Get(const String& text, const Markup& markup = Markup{ }, const bool resetTextWriterCaret = true) -> const List<GlyphRenderInfo>&;
        [[nodiscard]] auto Get(const UTF8String& text, const Markup& markup = Markup{ }, const bool resetTextWriterCaret = true) -> const List<GlyphRenderInfo>&;
        [[nodiscard]] inline auto operator ()(const String& text, const Markup& markup = Markup{ }, const bool resetTextWriterCaret = true) -> const List<GlyphRenderInfo>& { return Get(text, markup, resetTextWriterCaret); }
        [[nodiscard]] inline auto operator ()(const UTF8String& text, const Markup& markup = Markup{ }, const bool resetTextWriterCaret = true) -> const List<GlyphRenderInfo>& { return Get(text, markup, resetTextWriterCaret); }

        auto Clear() -> void;

        inline auto ResetTextWriterCaretLocation() const noexcept -> void { m_textWriter->ResetCaretLocation(); }

        [[nodiscard]] auto GetFont() const noexcept -> const Font& { return *m_font; }
        [[nodiscard]] auto GetTextWriter() noexcept -> TextWriter& { return *m_textWriter; }
        [[nodiscard]] auto GetTextWriter() const noexcept -> const TextWriter& { return *m_textWriter; }

    private:
        auto CheckIfFontChanged() -> void;
    };
}

#endif
