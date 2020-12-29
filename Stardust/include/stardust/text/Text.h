#pragma once
#ifndef STRARDUST_TEXT_H
#define STRARDUST_TEXT_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/text/font/Font.h"

namespace stardust
{
	namespace text
	{
		[[nodiscard]] extern Texture RenderGlyph(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderGlyph(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderText(const Font& font, const String& text, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderText(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderTextWrapped(const Font& font, const String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextWrapped(const Font& font, const UTF16String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char16_t glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderTextQuick(const Font& font, const String& text, const Colour& colour);
		[[nodiscard]] extern Texture RenderTextQuick(const Font& font, const UTF16String& text, const Colour& colour);

		[[nodiscard]] extern Texture RenderShadedGlyph(const Font& font, const char glyph, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedGlyph(const Font& font, const char16_t glyph, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedText(const Font& font, const String& text, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedText(const Font& font, const UTF16String& text, const Colour& colour, const Colour& backgroundColour);

		[[nodiscard]] extern Texture RenderGlyphWithOutline(const Font& font, const char glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderGlyphWithOutline(const Font& font, const char16_t glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderTextWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderTextWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);

		[[nodiscard]] extern Texture RenderTextWrappedWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const u32 wrapLength);
		[[nodiscard]] extern Texture RenderTextWrappedWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const u32 wrapLength);

		[[nodiscard]] extern Texture RenderGlyphQuickWithOutline(const Font& font, const char glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderGlyphQuickWithOutline(const Font& font, const char16_t glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderTextQuickWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
		[[nodiscard]] extern Texture RenderTextQuickWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour);
	}
}

#endif