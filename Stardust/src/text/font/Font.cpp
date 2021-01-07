#include "stardust/text/font/Font.h"

#include <utility>

#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	void Font::FontDestroyer::operator()(TTF_Font* const font) const noexcept
	{
		if (TTF_WasInit())
		{
			TTF_CloseFont(font);
		}
	}

	Font::Font(const StringView& fontFilepath, const u32 pointSize)
	{
		Initialise(fontFilepath, pointSize);
	}

	Font::Font(Font&& other) noexcept
		: m_handle(nullptr), m_pointSize(0u)
	{
		if (m_fontFileRWOps != nullptr)
		{
			SDL_RWclose(m_fontFileRWOps);
			m_fontFileRWOps = nullptr;
			m_fontFileData.clear();
		}

		std::swap(m_handle, other.m_handle);
		std::swap(m_pointSize, other.m_pointSize);

		std::swap(m_fontFileRWOps, other.m_fontFileRWOps);
		std::swap(m_fontFileData, other.m_fontFileData);
	}

	Font& Font::operator =(Font&& other) noexcept
	{
		Destroy();

		m_handle = std::exchange(other.m_handle, nullptr);
		m_pointSize = std::exchange(other.m_pointSize, 0u);

		m_fontFileRWOps = std::exchange(other.m_fontFileRWOps, nullptr);
		m_fontFileData = std::exchange(other.m_fontFileData, { });

		return *this;
	}

	Font::~Font() noexcept
	{
		Destroy();
	}

	void Font::Initialise(const StringView& fontFilepath, const u32 pointSize)
	{
		m_fontFileData = vfs::ReadFileData(fontFilepath);

		if (m_fontFileData.empty())
		{
			return;
		}

		m_fontFileRWOps = SDL_RWFromConstMem(m_fontFileData.data(), static_cast<i32>(m_fontFileData.size()));

		if (m_fontFileRWOps == nullptr)
		{
			m_fontFileData.clear();

			return;
		}

		m_handle = UniquePtr<TTF_Font, FontDestroyer>(TTF_OpenFontRW(m_fontFileRWOps, SDL_FALSE, static_cast<i32>(pointSize)));

		if (m_handle != nullptr)
		{
			m_pointSize = pointSize;
		}
		else
		{
			SDL_RWclose(m_fontFileRWOps);
			m_fontFileRWOps = nullptr;
			m_fontFileData.clear();
		}
	}

	void Font::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			m_handle = nullptr;

			SDL_RWclose(m_fontFileRWOps);
			m_fontFileRWOps = nullptr;
			m_fontFileData.clear();

			m_pointSize = 0u;
		}
	}

	[[nodiscard]] Vector<Font::Style> Font::GetStyles() const
	{
		constexpr Array<Style, 4u> AllFontStyles{
			Style::Bold,
			Style::Italic,
			Style::Underline,
			Style::Strikethrough
		};

		const i32 styleBitmask = TTF_GetFontStyle(GetRawHandle());
		Vector<Style> appliedStyles{ };

		for (const Style style : AllFontStyles)
		{
			if (styleBitmask & static_cast<i32>(style))
			{
				appliedStyles.push_back(style);
			}
		}

		if (appliedStyles.empty())
		{
			appliedStyles.push_back(Style::Normal);
		}

		return appliedStyles;
	}

	void Font::SetStyles(const Vector<Style>& styles) const
	{
		i32 styleBitmask = 0;

		for (const Style style : styles)
		{
			styleBitmask |= static_cast<i32>(style);
		}

		TTF_SetFontStyle(GetRawHandle(), styleBitmask);
	}

	void Font::ClearStyles() const
	{
		TTF_SetFontStyle(GetRawHandle(), static_cast<i32>(Style::Normal));
	}

	[[nodiscard]] u32 Font::GetOutlineThickness() const
	{
		return static_cast<u32>(TTF_GetFontOutline(GetRawHandle()));
	}

	void Font::SetOutlineThickness(const u32 outlineThickness) const
	{
		TTF_SetFontOutline(GetRawHandle(), static_cast<int>(outlineThickness));
	}

	void Font::RemoveOutline() const
	{
		TTF_SetFontOutline(GetRawHandle(), 0);
	}

	[[nodiscard]] Font::Hinting Font::GetHinting() const
	{
		return static_cast<Hinting>(TTF_GetFontHinting(GetRawHandle()));
	}

	void Font::SetHinting(const Hinting hinting) const
	{
		TTF_SetFontHinting(GetRawHandle(), static_cast<i32>(hinting));
	}

	[[nodiscard]] u32 Font::GetKerning() const
	{
		return static_cast<u32>(TTF_GetFontKerning(GetRawHandle()));
	}

	void Font::SetKerning(const u32 kerning) const
	{
		TTF_SetFontKerning(GetRawHandle(), static_cast<i32>(kerning));
	}

	[[nodiscard]] u32 Font::GetKerningBetweenGlyphs(const char16_t leftGlyph, const char16_t rightGlyph) const
	{
		return static_cast<u32>(TTF_GetFontKerningSizeGlyphs(GetRawHandle(), static_cast<u16>(leftGlyph), static_cast<u16>(rightGlyph)));
	}

	[[nodiscard]] u32 Font::GetMaximumHeight() const
	{
		return static_cast<u32>(TTF_FontHeight(GetRawHandle()));
	}

	[[nodiscard]] i32 Font::GetFontAscent() const
	{
		return TTF_FontAscent(GetRawHandle());
	}

	[[nodiscard]] i32 Font::GetFontDescent() const
	{
		return TTF_FontDescent(GetRawHandle());
	}

	[[nodiscard]] u32 Font::GetLineSkip() const
	{
		return static_cast<u32>(TTF_FontLineSkip(GetRawHandle()));
	}

	[[nodiscard]] bool Font::IsFixedWidth() const
	{
		return static_cast<bool>(TTF_FontFaceIsFixedWidth(GetRawHandle()));
	}

	[[nodiscard]] Optional<usize> Font::GetGlyphIndex(const char16_t glyph) const
	{
		const int glyphIndex = TTF_GlyphIsProvided(GetRawHandle(), static_cast<u16>(glyph));

		if (glyphIndex != 0)
		{
			return static_cast<usize>(glyphIndex);
		}
		else
		{
			return NullOpt;
		}
	}

	[[nodiscard]] bool Font::DoesGlyphExist(const char16_t glyph) const
	{
		return GetGlyphIndex(glyph).has_value();
	}

	[[nodiscard]] Font::GlyphMetrics Font::GetGlyphMetrics(const char16_t glyph) const
	{
		GlyphMetrics glyphMetrics{ };

		TTF_GlyphMetrics(
			GetRawHandle(),
			static_cast<u16>(glyph),
			&glyphMetrics.minOffset.x,
			&glyphMetrics.maxOffset.x,
			&glyphMetrics.minOffset.y,
			&glyphMetrics.maxOffset.y,
			&glyphMetrics.advance
		);

		return glyphMetrics;
	}

	[[nodiscard]] UVec2 Font::GetTextSize(const String& text) const
	{
		i32 textWidth = 0;
		i32 textHeight = 0;
		TTF_SizeUTF8(GetRawHandle(), text.c_str(), &textWidth, &textHeight);

		return UVec2{ textWidth, textHeight };
	}

	[[nodiscard]] UVec2 Font::GetTextSize(const UTF16String& text) const
	{
		i32 textWidth = 0;
		i32 textHeight = 0;
		TTF_SizeUNICODE(GetRawHandle(), reinterpret_cast<const u16*>(text.data()), &textWidth, &textHeight);

		return UVec2{ textWidth, textHeight };
	}
}