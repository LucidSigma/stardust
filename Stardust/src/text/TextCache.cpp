#include "stardust/text/TextCache.h"

#include "stardust/utility/unicode/Unicode.h"

namespace stardust
{
    TextCache::TextCache(TextWriter& textWriter)
    {
        Initialise(textWriter);
    }

    auto TextCache::Initialise(TextWriter& textWriter) -> void
    {
        m_textWriter = &textWriter;

        m_font = &m_textWriter->GetFont();
        m_currentFontTextureAtlasSize = m_font->GetInternalTextureAtlasSize();
    }

    [[nodiscard]] auto TextCache::Get(const String& text, const Markup& markup, const bool resetTextWriterCaret) -> const List<GlyphRenderInfo>&
    {
        CheckIfFontChanged();

        if (const auto textLocation = m_glyphs.find(text);
            textLocation == std::cend(m_glyphs))
        {
            if (resetTextWriterCaret)
            {
                m_textWriter->ResetCaretLocation();
            }

            const auto markupInsertionResult = m_glyphs.emplace(text, HashMap<Markup, List<GlyphRenderInfo>>{ });
            HashMap<Markup, List<GlyphRenderInfo>>& newGlyphs = markupInsertionResult.first->second;
        
            const auto glyphInsertionResult = newGlyphs.emplace(markup, m_textWriter->WriteText(text, markup));
        
            return glyphInsertionResult.first->second;
        }
        else if (const auto markupLocation = textLocation->second.find(markup);
            markupLocation == std::cend(textLocation->second))
        {
            const auto glyphInsertionResult = textLocation->second.emplace(markup, m_textWriter->WriteText(text, markup));
        
            return glyphInsertionResult.first->second;
        }
        else
        {
            return markupLocation->second;
        }
    }

    [[nodiscard]] auto TextCache::Get(const UTF8String& text, const Markup& markup, const bool resetTextWriterCaret) -> const List<GlyphRenderInfo>&
    {
        return Get(unicode::UTF8ToStandardString(text), markup, resetTextWriterCaret);
    }

    auto TextCache::Clear() -> void
    {
        m_glyphs.clear();
    }

    auto TextCache::CheckIfFontChanged() -> void
    {
        if (m_font != &m_textWriter->GetFont() || m_textWriter->GetFont().GetInternalTextureAtlasSize() != m_currentFontTextureAtlasSize)
        {
            Clear();

            m_font = &m_textWriter->GetFont();
            m_currentFontTextureAtlasSize = m_font->GetInternalTextureAtlasSize();
        }
    }
}
