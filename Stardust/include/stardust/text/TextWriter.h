#pragma once
#ifndef STARDUST_TEXT_WRITER_H
#define STARDUST_TEXT_WRITER_H

#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/math/Math.h"
#include "stardust/text/font/Font.h"
#include "stardust/text/localisation/Localisation.h"
#include "stardust/text/shaping/ShapingBuffer.h"
#include "stardust/text/Markup.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    struct GlyphRenderInfo final
    {
        IVector2 offset;
        UVector2 size;

        components::Sprite sprite;
    };

    class TextWriter final
    {
    private:
        ObserverPointer<Font> m_font = nullptr;
        ShapingBuffer m_shapingBuffer;

        IVector2 m_initialPosition = IVector2Zero;
        IVector2 m_caretLocation = IVector2Zero;

        IVector2 m_startOfLineLocation = IVector2Zero;

    public:
        TextWriter() = default;
        TextWriter(Font& font, const IVector2 initialPosition);
        ~TextWriter() noexcept = default;

        auto Initialise(Font& font, const IVector2 initialPosition) -> void;

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_font != nullptr && m_shapingBuffer.IsValid(); }

        [[nodiscard]] auto WriteText(const String& text, const Markup& markup = Markup{ }) -> List<GlyphRenderInfo>;
        [[nodiscard]] auto WriteText(const UTF8String& text, const Markup& markup = Markup{ }) -> List<GlyphRenderInfo>;

        auto FeedNewLine() -> void;

        [[nodiscard]] auto GetTextWidth(const StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> u32;
        [[nodiscard]] auto GetTextWidth(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> u32;
        [[nodiscard]] auto GetTextHeight(const StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> u32;
        [[nodiscard]] auto GetTextHeight(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> u32;
        [[nodiscard]] auto GetTextSize(const StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> UVector2;
        [[nodiscard]] auto GetTextSize(const UTF8StringView text, const localisation::TextLocalisationInfo& localisation = localisation::TextLocalisationInfo{ }) const -> UVector2;

        [[nodiscard]] inline auto GetFont() const noexcept -> const Font& { return *m_font; }
        inline auto SetFont(Font& font) noexcept -> void { m_font = &font; }
        auto SetInitialPosition(const IVector2 initialPosition, const bool resetCaretLocation = true) noexcept -> void;

        [[nodiscard]] inline auto GetCaretLocation() const noexcept -> IVector2 { return m_caretLocation; }
        auto SetCaretLocation(const IVector2 caretLocation) noexcept -> void;
        auto ShiftCaretLocation(const IVector2 offset) noexcept -> void { m_caretLocation += offset; }
        auto ResetCaretLocation() noexcept -> void;

        [[nodiscard]] inline auto GetShapingBuffer() noexcept -> ShapingBuffer& { return m_shapingBuffer; }
        [[nodiscard]] inline auto GetShapingBuffer() const noexcept -> const ShapingBuffer& { return m_shapingBuffer; }

    private:
        [[nodiscard]] auto GetLinesFromText(const String& text, const Markup& markup) -> List<String>;

        auto PrintDropShadow(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void;
        auto PrintOutline(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void;
        auto PrintGlyph(const ShapedGlyph& glyph, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) -> void;
        auto PrintLineDecorations(const String& line, const Markup& markup, List<GlyphRenderInfo>& glyphsToRender) const -> void;
        [[nodiscard]] auto GetOffsetFromTextAlignment(const String& line, const Markup& markup) const -> i32;
    };

    [[nodiscard]] extern auto GetTextWidth(const List<GlyphRenderInfo>& glyphs) -> u32;
    [[nodiscard]] extern auto GetTextHeight(const List<GlyphRenderInfo>& glyphs) -> u32;
    [[nodiscard]] extern auto GetTextSize(const List<GlyphRenderInfo>& glyphs) -> UVector2;
}

#endif
