#include "stardust/text/TextWriter.h"

#include <harfbuzz/hb.h>

#include "stardust/text/Glyph.h"
#include "stardust/utility/string/String.h"

namespace stardust
{
    TextWriter::TextWriter(Font& font, const IVector2 initialPosition)
    {
        Initialise(font, initialPosition);
    }

    auto TextWriter::Initialise(Font& font, const IVector2 initialPosition) -> void
    {
        SetFont(font);
        m_shapingBuffer.Initialise();

        SetInitialPosition(initialPosition);
    }

    [[nodiscard]] auto TextWriter::WriteText(const String& text, const Markup& markup) -> List<GlyphRenderInfo>
    {
        List<GlyphRenderInfo> glyphsToRender{ };
        const List<String> lines = GetLinesFromText(text, markup);

        for (usize currentLineIndex = 0u;
            const auto& line : lines)
        {
            m_shapingBuffer.Reset();

            m_shapingBuffer.SetTextDirection(markup.localisation.direction);
            m_shapingBuffer.SetTextScript(markup.localisation.script);
            m_shapingBuffer.SetTextLanguage(markup.localisation.language);

            if (currentLineIndex == 0u && markup.localisation.isStartOfText)
            {
                m_shapingBuffer.SetAsStartOfText(true);
            }

            if (currentLineIndex == lines.size() - 1u && markup.localisation.isEndOfText)
            {
                m_shapingBuffer.SetAsEndOfText(true);
            }

            m_shapingBuffer.AddUTF8(line);
            m_shapingBuffer.ShapeText(*m_font);

            const i32 textAlignmentOffset = GetOffsetFromTextAlignment(line, markup);
            m_caretLocation.x -= textAlignmentOffset;
            m_startOfLineLocation = m_caretLocation;

            m_font->SetRenderMode(markup.defaultRenderMode);
            const auto glyphs = m_shapingBuffer.GetShapedGlyphs(*m_font);

            if (markup.dropShadow.has_value())
            {
                for (const auto& glyph : glyphs)
                {
                    PrintDropShadow(glyph, markup, glyphsToRender);
                }
            }

            m_caretLocation = m_startOfLineLocation;

            if (markup.outline.has_value())
            {
                for (const auto& glyph : glyphs)
                {
                    PrintOutline(glyph, markup, glyphsToRender);
                }
            }

            m_caretLocation = m_startOfLineLocation;

            for (const auto& glyph : glyphs)
            {
                PrintGlyph(glyph, markup, glyphsToRender);
            }

            PrintLineDecorations(line, markup, glyphsToRender);

            if (lines.size() > 1u)
            {
                FeedNewLine();
            }

            ++currentLineIndex;
        }

        return glyphsToRender;
    }

    [[nodiscard]] auto TextWriter::WriteText(const UTF8String& text, const Markup& markup) -> List<GlyphRenderInfo>
    {
        return WriteText(String(std::cbegin(text), std::cend(text)), markup);
    }

    auto TextWriter::FeedNewLine() -> void
    {
        m_caretLocation = IVector2{
            m_initialPosition.x,
            m_caretLocation.y + static_cast<i32>(m_font->GetLineHeight()),
        };
    }

    [[nodiscard]] auto TextWriter::GetTextWidth(const StringView text, const localisation::TextLocalisationInfo& localisation) const -> u32
    {
        m_shapingBuffer.Reset();
        m_shapingBuffer.SetLocalisationInfo(localisation);
        m_shapingBuffer.AddUTF8(text);
        m_shapingBuffer.ShapeText(*m_font);

        return m_shapingBuffer.GetTextWidth(*m_font);
    }

    [[nodiscard]] auto TextWriter::GetTextWidth(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation) const -> u32
    {
        return GetTextWidth(
            String(std::cbegin(text), std::cend(text)),
            localisation
        );
    }

    [[nodiscard]] auto TextWriter::GetTextHeight(const StringView text, const localisation::TextLocalisationInfo& localisation) const -> u32
    {
        m_shapingBuffer.Reset();
        m_shapingBuffer.SetLocalisationInfo(localisation);
        m_shapingBuffer.AddUTF8(text);
        m_shapingBuffer.ShapeText(*m_font);

        return m_shapingBuffer.GetTextHeight(*m_font);
    }

    [[nodiscard]] auto TextWriter::GetTextHeight(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation) const -> u32
    {
        return GetTextHeight(
            String(std::cbegin(text), std::cend(text)),
            localisation
        );
    }

    [[nodiscard]] auto TextWriter::GetTextSize(const StringView text, const localisation::TextLocalisationInfo& localisation) const -> UVector2
    {
        m_shapingBuffer.Reset();
        m_shapingBuffer.SetLocalisationInfo(localisation);
        m_shapingBuffer.AddUTF8(text);
        m_shapingBuffer.ShapeText(*m_font);

        return UVector2{
            m_shapingBuffer.GetTextWidth(*m_font),
            m_shapingBuffer.GetTextHeight(*m_font),
        };
    }

    [[nodiscard]] auto TextWriter::GetTextSize(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation) const -> UVector2
    {
        return GetTextSize(
            String(std::cbegin(text), std::cend(text)),
            localisation
        );
    }

    auto TextWriter::SetInitialPosition(const IVector2 initialPosition, const bool resetCaretLocation) noexcept -> void
    {
        m_initialPosition = initialPosition;

        if (resetCaretLocation)
        {
            ResetCaretLocation();
        }
    }

    auto TextWriter::SetCaretLocation(const IVector2 caretLocation) noexcept -> void
    {
        m_caretLocation = caretLocation;
        m_caretLocation.y += static_cast<i32>(m_font->GetLineAscender());
    }

    auto TextWriter::ResetCaretLocation() noexcept -> void
    {
        m_caretLocation = m_initialPosition;
        m_caretLocation.y += static_cast<i32>(m_font->GetLineAscender());
    }

    [[nodiscard]] auto TextWriter::GetLinesFromText(const String& text, const Markup& markup) -> List<String>
    {
        List<String> lines = string::Split(text, '\n');

        if (!markup.wrapLength.has_value())
        {
            return lines;
        }

        List<String> wrappedLines{ };

        for (auto iterator = std::cbegin(lines); iterator != std::cend(lines); ++iterator)
        {
            String wrappedLine = *iterator;

            m_shapingBuffer.Reset();
            m_shapingBuffer.SetLocalisationInfo(markup.localisation);
            m_shapingBuffer.AddUTF8(wrappedLine);
            m_shapingBuffer.ShapeText(*m_font);

            u32 wrappedLineLength = m_shapingBuffer.GetTextWidth(*m_font);
            String insertedLine;

            while (wrappedLineLength > markup.wrapLength.value())
            {
                const auto lastWordBreakPosition = wrappedLine.find_last_of(" \t");

                if (lastWordBreakPosition == String::npos)
                {
                    break;
                }

                if (insertedLine.empty())
                {
                    insertedLine.insert(0u, wrappedLine.substr(lastWordBreakPosition + 1u));
                }
                else
                {
                    insertedLine.insert(0u, wrappedLine.substr(lastWordBreakPosition + 1u) + ' ');
                }

                wrappedLine.erase(lastWordBreakPosition);

                m_shapingBuffer.Reset();
                m_shapingBuffer.SetLocalisationInfo(markup.localisation);
                m_shapingBuffer.AddUTF8(wrappedLine);
                m_shapingBuffer.ShapeText(*m_font);

                wrappedLineLength = m_shapingBuffer.GetTextWidth(*m_font);
            }

            wrappedLines.push_back(wrappedLine);

            if (!insertedLine.empty())
            {
                iterator = lines.insert(iterator + 1u, insertedLine) - 1u;
            }
        }

        return wrappedLines;
    }

    auto TextWriter::PrintDropShadow(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void
    {
        if (markup.dropShadow.value().thickness.has_value())
        {
            m_font->SetRenderMode(Font::RenderMode::OutlinePositive);
            m_font->SetOutlineThickness(markup.dropShadow.value().thickness.value());

            const Glyph outlinedGlyph = m_font->GetGlyphFromIndex(glyph.index);

            glyphsToRender.emplace_back(
                IVector2{
                    m_caretLocation.x + glyph.offset.x + outlinedGlyph.bearing.x,
                    static_cast<i32>(glm::floor(m_caretLocation.y - glyph.offset.y - outlinedGlyph.bearing.y)),
                } + markup.dropShadow.value().offset,
                outlinedGlyph.size,
                components::Sprite{
                    .texture = &m_font->GetTexture(),
                    .subTextureArea = outlinedGlyph.textureCoordinates,
                    .colourMod = markup.dropShadow.value().colour,
                }
            );

            m_font->SetRenderMode(markup.defaultRenderMode);
            m_font->RemoveOutline();
        }
        else
        {
            glyphsToRender.emplace_back(
                IVector2{
                    m_caretLocation.x + glyph.offset.x + glyph.bearing.x,
                    static_cast<i32>(glm::floor(m_caretLocation.y - glyph.offset.y - glyph.bearing.y)),
                } + markup.dropShadow.value().offset,
                glyph.size,
                components::Sprite{
                    .texture = &m_font->GetTexture(),
                    .subTextureArea = glyph.textureCoordinates,
                    .colourMod = markup.dropShadow.value().colour,
                }
            );
        }

        m_caretLocation.x += static_cast<i32>(glyph.advance.x);
        m_caretLocation.y += static_cast<i32>(glyph.advance.y);
    }

    auto TextWriter::PrintOutline(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void
    {
        m_font->SetRenderMode(Font::RenderMode::OutlinePositive);
        m_font->SetOutlineThickness(markup.outline.value().thickness);

        const Glyph outlinedGlyph = m_font->GetGlyphFromIndex(glyph.index);

        glyphsToRender.emplace_back(
            IVector2{
                m_caretLocation.x + glyph.offset.x + outlinedGlyph.bearing.x,
                static_cast<i32>(glm::floor(m_caretLocation.y - glyph.offset.y - outlinedGlyph.bearing.y)),
            } + markup.outline.value().offset,
            outlinedGlyph.size,
            components::Sprite{
                .texture = &m_font->GetTexture(),
                .subTextureArea = outlinedGlyph.textureCoordinates,
                .colourMod = markup.outline.value().colour,
            }
        );

        m_font->SetRenderMode(markup.defaultRenderMode);
        m_font->RemoveOutline();

        m_caretLocation.x += static_cast<i32>(glyph.advance.x);
        m_caretLocation.y += static_cast<i32>(glyph.advance.y);
    }

    auto TextWriter::PrintGlyph(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void
    {
        glyphsToRender.emplace_back(
            IVector2{
                m_caretLocation.x + glyph.offset.x + glyph.bearing.x,
                static_cast<i32>(glm::floor(m_caretLocation.y - glyph.offset.y - glyph.bearing.y)),
            },
            glyph.size,
            components::Sprite{
                .texture = &m_font->GetTexture(),
                .subTextureArea = glyph.textureCoordinates,
                .colourMod = markup.colour,
            }
        );

        m_caretLocation.x += static_cast<i32>(glyph.advance.x);
        m_caretLocation.y += static_cast<i32>(glyph.advance.y);
    }

    auto TextWriter::PrintLineDecorations(const String& line, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) const -> void
    {
        i32 startX = m_startOfLineLocation.x;
        u32 lineLength = m_caretLocation.x - m_startOfLineLocation.x;

        if (markup.outline.has_value())
        {
            startX -= static_cast<i32>(markup.outline.value().thickness);
            lineLength += static_cast<u32>(markup.outline.value().thickness * 2.0f);
        }

        if (markup.underline.has_value())
        {
            glyphsToRender.emplace_back(
                IVector2{
                    startX,
                    m_startOfLineLocation.y - static_cast<i32>(m_font->GetUnderlinePosition()),
                } + markup.underline.value().offset,
                UVector2{
                    lineLength,
                    static_cast<u32>(markup.underline.value().thickness),
                },
                components::Sprite{
                    .texture = nullptr,
                    .subTextureArea = None,
                    .colourMod = markup.underline.value().colour,
                }
            );
        }

        if (markup.strikethrough.has_value())
        {
            glyphsToRender.emplace_back(
                IVector2{
                    startX,
                    m_startOfLineLocation.y - static_cast<i32>(m_font->GetLineAscender() / 3u) - static_cast<i32>(markup.underline.value().thickness / 2.0f),
                } + markup.strikethrough.value().offset,
                UVector2{
                    lineLength,
                    static_cast<u32>(markup.underline.value().thickness),
                },
                components::Sprite{
                    .texture = nullptr,
                    .subTextureArea = None,
                    .colourMod = markup.underline.value().colour,
                }
            );
        }

        if (markup.overline.has_value())
        {
            glyphsToRender.emplace_back(
                IVector2{
                    startX,
                    m_startOfLineLocation.y - static_cast<i32>(m_font->GetLineAscender()) - static_cast<i32>(markup.underline.value().thickness),
                } + markup.overline.value().offset,
                UVector2{
                    lineLength,
                    static_cast<u32>(markup.underline.value().thickness),
                },
                components::Sprite{
                    .texture = nullptr,
                    .subTextureArea = None,
                    .colourMod = markup.underline.value().colour,
                }
            );
        }
    }

    [[nodiscard]] auto TextWriter::GetOffsetFromTextAlignment(const String& line, const Markup& markup) const -> i32
    {
        switch (markup.textAlignment)
        {
        case TextAlignment::Left:
        [[unlikely]] default:
            return 0;

        case TextAlignment::Centre:
            return static_cast<i32>(m_shapingBuffer.GetTextWidth(*m_font) / 2u);

        case TextAlignment::Right:
        {
            return static_cast<i32>(m_shapingBuffer.GetTextWidth(*m_font));
        }
        }
    }

    [[nodiscard]] auto GetTextWidth(const List<GlyphRenderInfo>& glyphs) -> u32
    {
        if (glyphs.empty())
        {
            return 0u;
        }

        const i32 firstGlyphX = glyphs.front().offset.x;
        const i32 lastGlyphX = glyphs.back().offset.x;

        return static_cast<u32>(lastGlyphX - firstGlyphX) + glyphs.back().size.x;
    }

    [[nodiscard]] auto GetTextHeight(const List<GlyphRenderInfo>& glyphs) -> u32
    {
        if (glyphs.empty())
        {
            return 0u;
        }

        i32 minHeight = 0;
        i32 maxHeight = 0;

        for (const auto& glyph : glyphs)
        {
            const i32 topY = glyph.offset.y;
            const i32 bottomY = glyph.offset.y + static_cast<i32>(glyph.size.y);

            minHeight = std::min(minHeight, topY);
            maxHeight = std::max(maxHeight, bottomY);
        }

        return static_cast<u32>(glm::abs(minHeight) + glm::abs(maxHeight));
    }

    [[nodiscard]] auto GetTextSize(const List<GlyphRenderInfo>& glyphs) -> UVector2
    {
        return UVector2{
            GetTextWidth(glyphs),
            GetTextHeight(glyphs),
        };
    }
}
