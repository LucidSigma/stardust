#pragma once
#ifndef STARDUST_TEXT_CACHE_H
#define STARDUST_TEXT_CACHE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/text/font/Font.h"
#include "stardust/text/Text.h"

namespace stardust
{
	class TextCache
		: private INoncopyable
	{
	private:
		ObserverPtr<const Font> m_font = nullptr;
		Sampler m_sampler{ };

		HashMap<text::GlyphInfo, Texture> m_glyphs{ };
		HashMap<text::UTF16GlyphInfo, Texture> m_utf16glyphs{ };

		HashMap<text::TextInfo, Texture> m_textures{ };
		HashMap<text::UTF16TextInfo, Texture> m_utf16Textures{ };

		HashMap<String, ObserverPtr<const Texture>> m_quickTextLookup{ };
		HashMap<UTF16String, ObserverPtr<const Texture>> m_quickUTF16TextLookup{ };

	public:
		TextCache() = default;
		TextCache(const Font& font, const Sampler& sampler = Sampler{ });
		~TextCache() noexcept;

		void Initialise(const Font& font, const Sampler& sampler = Sampler{ });

		[[nodiscard]] ObserverPtr<const Texture> Get(const char glyph);
		[[nodiscard]] ObserverPtr<const Texture> Get(const text::GlyphInfo& glyphInfo);
		[[nodiscard]] ObserverPtr<const Texture> Get(const char16_t glyph);
		[[nodiscard]] ObserverPtr<const Texture> Get(const text::UTF16GlyphInfo& glyphInfo);

		[[nodiscard]] ObserverPtr<const Texture> Get(const String& text);
		[[nodiscard]] ObserverPtr<const Texture> Get(const text::TextInfo& textInfo);
		[[nodiscard]] ObserverPtr<const Texture> Get(const UTF16String& text);
		[[nodiscard]] ObserverPtr<const Texture> Get(const text::UTF16TextInfo& textInfo);

		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const String& text) { return Get(text); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const text::TextInfo& textInfo) { return Get(textInfo); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const UTF16String& text) { return Get(text); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const text::UTF16TextInfo& textInfo) { return Get(textInfo); }

		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const char glyph) { return Get(glyph); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const text::GlyphInfo& glyphInfo) { return Get(glyphInfo); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const char16_t glyph) { return Get(glyph); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const text::UTF16GlyphInfo& glyphInfo) { return Get(glyphInfo); }
	};
}

#endif