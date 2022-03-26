#include "stardust/text/shaping/ShapingBuffer.h"

#include <algorithm>
#include <utility>

#include "stardust/math/Math.h"

namespace stardust
{
    auto ShapingBuffer::BufferDeleter::operator()(hb_buffer_t* const buffer) const noexcept -> void
    {
        hb_buffer_destroy(buffer);
    }

    ShapingBuffer::ShapingBuffer(ShapingBuffer&& other) noexcept
    {
        Destroy();

        std::swap(m_handle, other.m_handle);
    }

    auto ShapingBuffer::operator =(ShapingBuffer&& other) noexcept -> ShapingBuffer&
    {
        Destroy();

        std::swap(m_handle, other.m_handle);

        return *this;
    }

    ShapingBuffer::~ShapingBuffer() noexcept
    {
        Destroy();
    }

    auto ShapingBuffer::Initialise() -> void
    {
        m_handle = UniquePointer<hb_buffer_t, BufferDeleter>(
            hb_buffer_create()
        );
    }

    auto ShapingBuffer::Destroy() noexcept -> void
    {
        if (m_handle != nullptr)
        {
            m_handle = nullptr;
        }
    }

    auto ShapingBuffer::SetLocalisationInfo(const localisation::TextLocalisationInfo& localisation) const -> void
    {
        SetTextDirection(localisation.direction);
        SetTextScript(localisation.script);
        SetTextLanguage(localisation.language);

        SetAsStartOfText(localisation.isStartOfText);
        SetAsEndOfText(localisation.isEndOfText);
    }

    [[nodiscard]] auto ShapingBuffer::GetTextDirection() const noexcept -> localisation::Direction
    {
        return static_cast<localisation::Direction>(hb_buffer_get_direction(m_handle.get()));
    }

    auto ShapingBuffer::SetTextDirection(const localisation::Direction direction) const -> void
    {
        hb_buffer_set_direction(m_handle.get(), static_cast<hb_direction_t>(direction));
    }

    [[nodiscard]] auto ShapingBuffer::GetTextScript() const noexcept -> localisation::Script
    {
        return static_cast<localisation::Script>(hb_buffer_get_script(m_handle.get()));
    }

    auto ShapingBuffer::SetTextScript(const localisation::Script script) const -> void
    {
        hb_buffer_set_script(m_handle.get(), static_cast<hb_script_t>(script));
    }

    [[nodiscard]] auto ShapingBuffer::GetTextLanguage() const -> String
    {
        return hb_language_to_string(hb_buffer_get_language(m_handle.get()));
    }

    auto ShapingBuffer::SetTextLanguage(const String& languageCode) const -> void
    {
        hb_buffer_set_language(m_handle.get(), hb_language_from_string(languageCode.c_str(), -1));
    }

    [[nodiscard]] auto ShapingBuffer::IsSetAsStartOfText() const -> bool
    {
        return hb_buffer_get_flags(m_handle.get()) & HB_BUFFER_FLAG_BOT;
    }

    auto ShapingBuffer::SetAsStartOfText(const bool isStartOfText) const -> void
    {
        if (isStartOfText)
        {
            hb_buffer_set_flags(m_handle.get(), static_cast<hb_buffer_flags_t>(hb_buffer_get_flags(m_handle.get()) | HB_BUFFER_FLAG_BOT));
        }
        else
        {
            hb_buffer_set_flags(m_handle.get(), static_cast<hb_buffer_flags_t>(hb_buffer_get_flags(m_handle.get()) & ~HB_BUFFER_FLAG_BOT));
        }
    }

    auto ShapingBuffer::SetAsEndOfText(const bool isEndOfText) const -> void
    {
        if (isEndOfText)
        {
            hb_buffer_set_flags(m_handle.get(), static_cast<hb_buffer_flags_t>(hb_buffer_get_flags(m_handle.get()) | HB_BUFFER_FLAG_EOT));
        }
        else
        {
            hb_buffer_set_flags(m_handle.get(), static_cast<hb_buffer_flags_t>(hb_buffer_get_flags(m_handle.get()) & ~HB_BUFFER_FLAG_EOT));
        }
    }

    [[nodiscard]] auto ShapingBuffer::IsSetAsEndOfText() const -> bool
    {
        return hb_buffer_get_flags(m_handle.get()) & HB_BUFFER_FLAG_EOT;
    }

    auto ShapingBuffer::AddASCII(const StringView data) const -> void
    {
        hb_buffer_add_latin1(m_handle.get(), reinterpret_cast<const u8*>(data.data()), -1, 0u, -1);
    }

    auto ShapingBuffer::AddUTF8(const StringView data) const -> void
    {
        hb_buffer_add_utf8(m_handle.get(), data.data(), -1, 0u, -1);
    }

    auto ShapingBuffer::AddUTF8(const UTF8StringView data) const -> void
    {
        hb_buffer_add_utf8(m_handle.get(), reinterpret_cast<const char*>(data.data()), -1, 0u, -1);
    }

    auto ShapingBuffer::AddUTF16(const UTF16StringView data) const -> void
    {
        hb_buffer_add_utf16(m_handle.get(), reinterpret_cast<const u16*>(data.data()), -1, 0u, -1);
    }

    auto ShapingBuffer::AddUTF32(const UTF32StringView data) const -> void
    {
        hb_buffer_add_utf32(m_handle.get(), reinterpret_cast<const u32*>(data.data()), -1, 0u, -1);
    }

    auto ShapingBuffer::ShapeText(Font& font) const -> void
    {
        hb_shape(font.GetShaper(), m_handle.get(), nullptr, 0u);
    }

    [[nodiscard]] auto ShapingBuffer::GetShapedGlyphs(const Font& font) const -> List<ShapedGlyph>
    {
        List<ShapedGlyph> shapedGlyphs{ };

        u32 glyphCount = 0u;
        hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(m_handle.get(), &glyphCount);
        hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(m_handle.get(), &glyphCount);

        const UVector2 originalTextureAtlasSize = font.GetInternalTextureAtlasSize();

        for (usize i = 0u; i < static_cast<usize>(glyphCount); ++i)
        {
            const u32 glyphIndex = glyphInfos[i].codepoint;
            const Glyph glyphMetrics = font.GetGlyphFromIndex(glyphIndex);

            const i32 xAdvance = glyphPositions[i].x_advance >> 12;
            const i32 yAdvance = glyphPositions[i].y_advance >> 6;
            const i32 xOffset = glyphPositions[i].x_offset >> 12;
            const i32 yOffset = glyphPositions[i].y_offset >> 6;

            shapedGlyphs.emplace_back(
                glyphIndex,
                glyphMetrics.size,
                glyphMetrics.bearing,
                Vector2{ static_cast<f32>(xAdvance), static_cast<f32>(yAdvance) },
                IVector2{ xOffset, yOffset },
                glyphMetrics.textureCoordinates
            );
        }

        const UVector2 newTextureAtlasSize = font.GetInternalTextureAtlasSize();

        if (originalTextureAtlasSize != newTextureAtlasSize)
        {
            for (auto& shapedGlyph : shapedGlyphs)
            {
                const Glyph glyphMetrics = font.GetGlyphFromIndex(shapedGlyph.index);
                shapedGlyph.textureCoordinates = glyphMetrics.textureCoordinates;
            }
        }

        return shapedGlyphs;
    }

    [[nodiscard]] auto ShapingBuffer::HasBeenShaped() const -> bool
    {
        return hb_buffer_get_content_type(m_handle.get()) == HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }

    [[nodiscard]] auto ShapingBuffer::GetLength() const -> usize
    {
        return static_cast<usize>(hb_buffer_get_length(m_handle.get()));
    }

    auto ShapingBuffer::Reset() const -> void
    {
        hb_buffer_reset(m_handle.get());
    }

    [[nodiscard]] auto ShapingBuffer::GetTextWidth(const Font& font) const -> u32
    {
        u32 width = 0u;

        u32 glyphCount = 0u;
        hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(m_handle.get(), &glyphCount);
        hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(m_handle.get(), &glyphCount);

        for (usize i = 0u; i < static_cast<usize>(glyphCount); ++i)
        {
            const u32 glyphIndex = glyphInfos[i].codepoint;
            const Glyph glyphMetrics = font.GetGlyphFromIndex(glyphIndex);

            width += static_cast<u32>(glyphMetrics.advance.x);
        }

        return width;
    }

    [[nodiscard]] auto ShapingBuffer::GetTextHeight(const Font& font) const -> u32
    {
        i32 minHeight = 0;
        i32 maxHeight = 0;

        u32 glyphCount = 0u;
        hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(m_handle.get(), &glyphCount);
        hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(m_handle.get(), &glyphCount);

        for (usize i = 0u; i < static_cast<usize>(glyphCount); ++i)
        {
            const u32 glyphIndex = glyphInfos[i].codepoint;
            const Glyph glyphMetrics = font.GetGlyphFromIndex(glyphIndex);

            const i32 height = static_cast<i32>(glyphMetrics.size.y);
            const i32 offset = glyphMetrics.bearing.y - height;

            minHeight = std::min(minHeight, offset);
            maxHeight = std::max(maxHeight, height);
        }

        return static_cast<u32>(glm::abs(minHeight) + glm::abs(maxHeight));
    }

    [[nodiscard]] auto ShapingBuffer::GetTextSize(const Font& font) const -> UVector2
    {
        return UVector2{
            GetTextWidth(font),
            GetTextHeight(font),
        };
    }

    [[nodiscard]] auto ShapingBuffer::IsValid() const noexcept -> bool
    {
        return m_handle != nullptr && hb_buffer_allocation_successful(m_handle.get()) != 0;
    }
}
