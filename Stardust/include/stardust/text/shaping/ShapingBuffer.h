#pragma once
#ifndef STARDUST_SHAPING_BUFFER_H
#define STARDUST_SHAPING_BUFFER_H

#include <harfbuzz/hb.h>

#include "stardust/text/font/Font.h"
#include "stardust/text/localisation/Localisation.h"
#include "stardust/text/Glyph.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/interfaces/INoncopyable.h"

namespace stardust
{
    class ShapingBuffer final
        : private INoncopyable
    {
    private:
        struct BufferDeleter final
        {
            auto operator ()(hb_buffer_t* const buffer) const noexcept -> void;
        };

        UniquePointer<hb_buffer_t, BufferDeleter> m_handle = nullptr;

    public:
        ShapingBuffer() = default;
        ShapingBuffer(ShapingBuffer&& other) noexcept;
        auto operator =(ShapingBuffer&& other) noexcept -> ShapingBuffer&;
        ~ShapingBuffer() noexcept;

        auto Initialise() -> void;
        auto Destroy() noexcept -> void;

        [[nodiscard]] auto IsValid() const noexcept -> bool;

        auto SetLocalisationInfo(const localisation::TextLocalisationInfo& localisation) const -> void;

        [[nodiscard]] auto GetTextDirection() const noexcept -> localisation::Direction;
        auto SetTextDirection(const localisation::Direction direction) const -> void;
        [[nodiscard]] auto GetTextScript() const noexcept -> localisation::Script;
        auto SetTextScript(const localisation::Script script) const -> void;
        [[nodiscard]] auto GetTextLanguage() const -> String;
        auto SetTextLanguage(const String& languageCode) const -> void;

        [[nodiscard]] auto IsSetAsStartOfText() const -> bool;
        auto SetAsStartOfText(const bool isStartOfText) const -> void;
        [[nodiscard]] auto IsSetAsEndOfText() const -> bool;
        auto SetAsEndOfText(const bool isEndOfText) const -> void;

        auto AddASCII(const StringView data) const -> void;
        auto AddUTF8(const StringView data) const -> void;
        auto AddUTF8(const UTF8StringView data) const -> void;
        auto AddUTF16(const UTF16StringView data) const -> void;
        auto AddUTF32(const UTF32StringView data) const -> void;

        auto ShapeText(Font& font) const -> void;
        [[nodiscard]] auto GetShapedGlyphs(const Font& font) const -> List<ShapedGlyph>;
        [[nodiscard]] auto HasBeenShaped() const -> bool;

        [[nodiscard]] auto GetLength() const -> usize;

        auto Reset() const -> void;

        [[nodiscard]] auto GetTextWidth(const Font& font) const -> u32;
        [[nodiscard]] auto GetTextHeight(const Font& font) const -> u32;
        [[nodiscard]] auto GetTextSize(const Font& font) const -> UVector2;

        [[nodiscard]] inline auto GetRawHandle() noexcept -> ObserverPointer<hb_buffer_t> { return m_handle.get(); }
        [[nodiscard]] inline auto GetRawHandle() const noexcept -> ObserverPointer<const hb_buffer_t> { return m_handle.get(); }
    };
}

#endif
