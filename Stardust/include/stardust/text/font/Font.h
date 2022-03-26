#pragma once
#ifndef STARDUST_FONT_H
#define STARDUST_FONT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <ftgl/freetype-gl.h>
#include <harfbuzz/hb.h>

#include "stardust/graphics/texture/Sampler.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/text/Glyph.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Font final
        : private INoncopyable
    {
    public:
        using Size = f32;

        enum class RenderMode
            : std::underlying_type_t<ftgl::rendermode_t>
        {
            Normal = ftgl::RENDER_NORMAL,
            OutlineEdge = ftgl::RENDER_OUTLINE_EDGE,
            OutlinePositive = ftgl::RENDER_OUTLINE_POSITIVE,
            OutlineNegative = ftgl::RENDER_OUTLINE_NEGATIVE,
            SignedDistanceField = ftgl::RENDER_SIGNED_DISTANCE_FIELD,
        };

        struct CreateInfo final
        {
            String filepath;
            Size pointSize;

            RenderMode renderMode = RenderMode::Normal;
            f32 outlineThickness = 0.0f;

            UVector2 textureAtlasSize{ 512u, 512u };
            u32 textureAtlasDepth = 4u;

            graphics::Sampler sampler = graphics::DefaultSampler;
        };

    private:
        struct TextureFontDeleter final
        {
            auto operator ()(ftgl::texture_font_t* const font) const noexcept -> void;
        };

        struct TextureAtlasDeleter final
        {
            auto operator ()(ftgl::texture_atlas_t* const textureAtlas) const noexcept -> void;
        };

        struct ShaperFontDeleter final
        {
            auto operator ()(hb_font_t* const shaperFont) const noexcept -> void;
        };

        UniquePointer<ftgl::texture_font_t, TextureFontDeleter> m_handle = nullptr;
        UniquePointer<ftgl::texture_atlas_t, TextureAtlasDeleter> m_textureAtlasHandle = nullptr;

        UniquePointer<hb_font_t, ShaperFontDeleter> m_shaper = nullptr;

        List<ubyte> m_fontData{ };

        mutable graphics::Texture m_texture;

    public:
        Font() = default;
        explicit Font(const CreateInfo& createInfo);
        Font(Font&& other) noexcept;
        auto operator =(Font&& other) noexcept -> Font&;
        ~Font() noexcept;

        auto Initialise(const CreateInfo& createInfo) -> void;
        auto Destroy() noexcept -> void;

        [[nodiscard]] auto GetGlyphFromCodepoint(const u32 codepoint) const -> Glyph;
        [[nodiscard]] auto GetGlyphFromIndex(const u32 glyphIndex) const -> Glyph;
        [[nodiscard]] auto GetGlyphTextureCoordinatesFromCodepoint(const u32 codepoint) const -> graphics::TextureCoordinatePair;
        [[nodiscard]] auto GetGlyphTextureCoordinatesFromIndex(const u32 glyphIndex) const -> graphics::TextureCoordinatePair;

        [[nodiscard]] auto FindGlyphFromCodepoint(const u32 codepoint) const -> Optional<Glyph>;
        [[nodiscard]] auto FindGlyphFromIndex(const u32 glyphIndex) const -> Optional<Glyph>;

        [[nodiscard]] auto ContainsGlyphCodepoint(const u32 codepoint) const -> bool;
        [[nodiscard]] auto ContainsGlyphIndex(const u32 glyphIndex) const -> bool;

        [[nodiscard]] auto GetGlyphIndexFromCodepoint(const u32 codepoint) const -> u32;

        [[nodiscard]] inline auto GetPointSize() const noexcept -> f32 { return m_handle->size; }

        [[nodiscard]] inline auto IsKerningEnabled() const noexcept -> bool { return m_handle->kerning != '\0'; }
        inline auto EnableKerning(const bool enableKerning) noexcept -> void { m_handle->kerning = enableKerning ? static_cast<unsigned char>(1u) : '\0'; }

        [[nodiscard]] inline auto IsAutohintingEnabled() const noexcept -> bool { return m_handle->hinting != '\0'; }
        inline auto EnableAutohinting(const bool enableAutohinting) noexcept -> void { m_handle->hinting = enableAutohinting ? static_cast<unsigned char>(1u) : '\0'; }

        [[nodiscard]] inline auto GetLineHeight() const noexcept -> f32 { return m_handle->height; }
        [[nodiscard]] inline auto GetLineGap() const noexcept -> f32 { return m_handle->linegap; }
        [[nodiscard]] inline auto GetLineAscender() const noexcept -> f32 { return m_handle->ascender; }
        [[nodiscard]] inline auto GetLineDescender() const noexcept -> f32 { return m_handle->descender; }

        [[nodiscard]] inline auto GetOutlineThickness() const noexcept -> f32 { return m_handle->outline_thickness; }
        inline auto SetOutlineThickness(const f32 thickness) noexcept -> void { m_handle->outline_thickness = thickness; }
        inline auto RemoveOutline() noexcept -> void { m_handle->outline_thickness = 0.0f; }

        [[nodiscard]] inline auto GetUnderlinePosition() const noexcept -> f32 { return m_handle->underline_position; }

        [[nodiscard]] inline auto GetRenderMode() const noexcept -> RenderMode { return static_cast<RenderMode>(m_handle->rendermode); }
        inline auto SetRenderMode(const RenderMode renderMode) noexcept -> void { m_handle->rendermode = static_cast<ftgl::rendermode_t>(renderMode); }

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr && m_textureAtlasHandle != nullptr && m_shaper != nullptr && m_texture.IsValid(); }

        [[nodiscard]] auto GetTexture() const -> const graphics::Texture&;
        [[nodiscard]] auto GetInternalTextureAtlasSize() const noexcept -> UVector2;

        [[nodiscard]] inline auto GetRawHandle() noexcept -> ObserverPointer<ftgl::texture_font_t> { return m_handle.get(); }
        [[nodiscard]] inline auto GetRawHandle() const noexcept -> ObserverPointer<const ftgl::texture_font_t> { return m_handle.get(); }

        [[nodiscard]] inline auto GetShaper() noexcept -> ObserverPointer<hb_font_t> { return m_shaper.get(); }
        [[nodiscard]] inline auto GetShaper() const noexcept -> ObserverPointer<const hb_font_t> { return m_shaper.get(); }

    private:
        auto ResizeTextureAtlas() const -> void;
    };
}

#endif
