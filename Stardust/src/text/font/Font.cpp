#include "stardust/text/font/Font.h"

#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb-ft.h>

#include "stardust/filesystem/vfs/VirtualFilesystem.h"

namespace stardust
{
    auto Font::TextureFontDeleter::operator()(ftgl::texture_font_t* const font) const noexcept -> void
    {
        ftgl::texture_font_delete(font);
    }

    auto Font::TextureAtlasDeleter::operator()(ftgl::texture_atlas_t* const textureAtlas) const noexcept -> void
    {
        ftgl::texture_atlas_delete(textureAtlas);
    }

    auto Font::ShaperFontDeleter::operator()(hb_font_t* const shaperFont) const noexcept -> void
    {
        hb_font_destroy(shaperFont);
    }

    Font::Font(const CreateInfo& createInfo)
    {
        Initialise(createInfo);
    }

    Font::Font(Font&& other) noexcept
    {
        Destroy();

        std::swap(m_handle, other.m_handle);
        std::swap(m_textureAtlasHandle, other.m_textureAtlasHandle);
        std::swap(m_shaper, other.m_shaper);
        std::swap(m_fontData, other.m_fontData);
        std::swap(m_texture, other.m_texture);
    }

    auto Font::operator =(Font&& other) noexcept -> Font&
    {
        Destroy();

        std::swap(m_handle, other.m_handle);
        std::swap(m_textureAtlasHandle, other.m_textureAtlasHandle);
        std::swap(m_shaper, other.m_shaper);
        std::swap(m_fontData, other.m_fontData);
        std::swap(m_texture, other.m_texture);

        return *this;
    }

    Font::~Font() noexcept
    {
        Destroy();
    }

    auto Font::Initialise(const CreateInfo& createInfo) -> void
    {
        auto fontFileResult = vfs::ReadFileBytes(createInfo.filepath);

        if (fontFileResult.is_err())
        {
            return;
        }

        m_fontData = std::move(fontFileResult).unwrap();

        m_textureAtlasHandle = UniquePointer<ftgl::texture_atlas_t, TextureAtlasDeleter>(
            ftgl::texture_atlas_new(
                static_cast<usize>(createInfo.textureAtlasSize.x),
                static_cast<usize>(createInfo.textureAtlasSize.y),
                static_cast<usize>(createInfo.textureAtlasDepth)
            )
        );

        if (m_textureAtlasHandle == nullptr)
        {
            return;
        }

        m_handle = UniquePointer<ftgl::texture_font_t, TextureFontDeleter>(
            ftgl::texture_font_new_from_memory(
                m_textureAtlasHandle.get(),
                createInfo.pointSize,
                m_fontData.data(),
                m_fontData.size()
            )
        );

        if (m_handle == nullptr)
        {
            return;
        }

        m_handle->rendermode = static_cast<ftgl::rendermode_t>(createInfo.renderMode);
        m_handle->outline_thickness = createInfo.outlineThickness;

        m_shaper = UniquePointer<hb_font_t, ShaperFontDeleter>(
            hb_ft_font_create_referenced(m_handle->face)
        );

        if (m_shaper == nullptr)
        {
            return;
        }

        hb_font_set_ptem(m_shaper.get(), createInfo.pointSize);

        m_texture.Initialise(
            m_textureAtlasHandle->data,
            UVector2{
                static_cast<u32>(m_textureAtlasHandle->width),
                static_cast<u32>(m_textureAtlasHandle->height),
            },
            static_cast<u32>(m_textureAtlasHandle->depth),
            createInfo.sampler
        );

        if (!m_texture.IsValid())
        {
            return;
        }

        m_textureAtlasHandle->id = m_texture.GetID();
    }

    auto Font::Destroy() noexcept -> void
    {
        if (m_texture.IsValid())
        {
            m_texture.Destroy();
        }

        if (m_shaper != nullptr)
        {
            m_shaper = nullptr;
        }

        if (m_handle != nullptr)
        {
            m_handle = nullptr;
        }

        if (m_textureAtlasHandle != nullptr)
        {
            m_textureAtlasHandle = nullptr;
        }
    }

    [[nodiscard]] auto Font::GetGlyphFromCodepoint(const u32 codepoint) const -> Glyph
    {
        return GetGlyphFromIndex(GetGlyphIndexFromCodepoint(codepoint));
    }

    [[nodiscard]] auto Font::GetGlyphFromIndex(const u32 glyphIndex) const -> Glyph
    {
        ftgl::texture_glyph_t* glyphPointer = ftgl::texture_font_get_glyph_gi(m_handle.get(), glyphIndex);

        while (glyphPointer == nullptr)
        {
            ResizeTextureAtlas();
            glyphPointer = ftgl::texture_font_get_glyph_gi(m_handle.get(), glyphIndex);
        }

        const Glyph glyph{
            .codepoint = glyphPointer->codepoint,
            .size = UVector2{
                glyphPointer->width,
                glyphPointer->height,
            },
            .bearing = IVector2{
                glyphPointer->offset_x,
                glyphPointer->offset_y,
            },
            .advance = Vector2{
                glyphPointer->advance_x,
                glyphPointer->advance_y,
            },
            .textureCoordinates = graphics::TextureCoordinatePair{
                .lowerLeft = Vector2{
                    glyphPointer->s0,
                    glyphPointer->t1,
                },
                .upperRight = Vector2{
                    glyphPointer->s1,
                    glyphPointer->t0,
                },
            },
        };

        return glyph;
    }

    [[nodiscard]] auto Font::GetGlyphTextureCoordinatesFromCodepoint(const u32 codepoint) const -> graphics::TextureCoordinatePair
    {
        return GetGlyphTextureCoordinatesFromIndex(GetGlyphIndexFromCodepoint(codepoint));
    }

    [[nodiscard]] auto Font::GetGlyphTextureCoordinatesFromIndex(const u32 glyphIndex) const -> graphics::TextureCoordinatePair
    {
        ftgl::texture_glyph_t* glyphPointer = ftgl::texture_font_get_glyph_gi(m_handle.get(), glyphIndex);

        while (glyphPointer == nullptr)
        {
            ResizeTextureAtlas();
            glyphPointer = ftgl::texture_font_get_glyph_gi(m_handle.get(), glyphIndex);
        }

        return graphics::TextureCoordinatePair{
            .lowerLeft = Vector2{
                glyphPointer->s0,
                glyphPointer->t1,
            },
            .upperRight = Vector2{
                glyphPointer->s1,
                glyphPointer->t0,
            },
        };
    }

    [[nodiscard]] auto Font::FindGlyphFromCodepoint(const u32 codepoint) const -> Optional<Glyph>
    {
        return FindGlyphFromIndex(GetGlyphIndexFromCodepoint(codepoint));
    }

    [[nodiscard]] auto Font::FindGlyphFromIndex(const u32 glyphIndex) const -> Optional<Glyph>
    {
        ftgl::texture_glyph_t* glyphPointer = ftgl::texture_font_find_glyph_gi(m_handle.get(), glyphIndex);

        if (glyphPointer == nullptr)
        {
            return None;
        }

        const Glyph glyph{
            .codepoint = glyphPointer->codepoint,
            .size = UVector2{
                glyphPointer->width,
                glyphPointer->height,
            },
            .bearing = IVector2{
                glyphPointer->offset_x,
                glyphPointer->offset_y,
            },
            .advance = Vector2{
                glyphPointer->advance_x,
                glyphPointer->advance_y,
            },
            .textureCoordinates = graphics::TextureCoordinatePair{
                .lowerLeft = Vector2{
                    glyphPointer->s0,
                    glyphPointer->t1,
                },
                .upperRight = Vector2{
                    glyphPointer->s1,
                    glyphPointer->t0,
                },
            },
        };

        return glyph;
    }

    [[nodiscard]] auto Font::ContainsGlyphCodepoint(const u32 codepoint) const -> bool
    {
        return ftgl::texture_font_find_glyph_gi(
            m_handle.get(),
            GetGlyphIndexFromCodepoint(codepoint)
        )!= nullptr;
    }

    [[nodiscard]] auto Font::ContainsGlyphIndex(const u32 glyphIndex) const -> bool
    {
        return ftgl::texture_font_find_glyph_gi(m_handle.get(), glyphIndex) != nullptr;
    }

    [[nodiscard]] auto Font::GetGlyphIndexFromCodepoint(const u32 codepoint) const -> u32
    {
        return FT_Get_Char_Index(m_handle->face, codepoint);
    }

    [[nodiscard]] auto Font::GetTexture() const -> const graphics::Texture&
    {
        if (m_textureAtlasHandle->modified != '\0')
        {
            m_texture.SetData(
                m_textureAtlasHandle->data,
                UVector2{
                    static_cast<u32>(m_textureAtlasHandle->width),
                    static_cast<u32>(m_textureAtlasHandle->height),
                },
                static_cast<u32>(m_textureAtlasHandle->depth)
            );

            m_textureAtlasHandle->modified = '\0';
        }

        return m_texture;
    }

    [[nodiscard]] auto Font::GetInternalTextureAtlasSize() const noexcept -> UVector2
    {
        return UVector2{
            static_cast<u32>(m_textureAtlasHandle->width),
            static_cast<u32>(m_textureAtlasHandle->height),
        };
    }

    auto Font::ResizeTextureAtlas() const -> void
    {
        const UVector2 textureAtlasSize = GetInternalTextureAtlasSize();

        ftgl::texture_font_enlarge_atlas(
            m_handle.get(),
            static_cast<usize>(textureAtlasSize.x) * 2u,
            static_cast<usize>(textureAtlasSize.y) * 2u
        );
    }
}
