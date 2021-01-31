#include "stardust/text/TextCache.h"

#include "stardust/data/Types.h"

namespace stardust
{
	TextCache::TextCache(const Font& font, const Sampler& sampler)
	{
		Initialise(font, sampler);
	}

	void TextCache::Initialise(const Font& font, const Sampler& sampler)
	{
		m_font = &font;
		m_sampler = sampler;
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const char glyph)
	{
		return Get(text::GlyphInfo{
			.glyph = glyph,
			.outline = NullOpt,
		});
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const text::GlyphInfo& glyphInfo)
	{
		if (const auto textLocation = m_glyphs.find(glyphInfo);
			textLocation == std::cend(m_glyphs))
		{
			const auto insertionResult = m_glyphs.emplace(glyphInfo, text::RenderGlyph(*m_font, glyphInfo, m_sampler));
			ObserverPtr<const Texture> glyphTexture = &insertionResult.first->second;

			return glyphTexture;
		}
		else
		{
			return &textLocation->second;
		}
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const char16_t glyph)
	{
		return Get(text::UTF16GlyphInfo{
			.glyph = glyph,
			.outline = NullOpt,
		});
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const text::UTF16GlyphInfo& glyphInfo)
	{
		if (const auto textLocation = m_utf16glyphs.find(glyphInfo);
			textLocation == std::cend(m_utf16glyphs))
		{
			const auto insertionResult = m_utf16glyphs.emplace(glyphInfo, text::RenderGlyph(*m_font, glyphInfo, m_sampler));
			ObserverPtr<const Texture> glyphTexture = &insertionResult.first->second;

			return glyphTexture;
		}
		else
		{
			return &textLocation->second;
		}
	}
	
	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const String& text)
	{
		if (const auto textLocation = m_quickTextLookup.find(text);
			textLocation != std::cend(m_quickTextLookup))
		{
			return textLocation->second;
		}
		else
		{
			return Get(text::TextInfo{
				.text = text,
				.outline = NullOpt,
				.wrapLength = NullOpt,
			});
		}
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const text::TextInfo& textInfo)
	{
		if (const auto textLocation = m_textures.find(textInfo);
			textLocation == std::cend(m_textures))
		{
			const auto insertionResult = m_textures.emplace(textInfo, text::RenderText(*m_font, textInfo, m_sampler));
			ObserverPtr<const Texture> textTexture = &insertionResult.first->second;

			if (!textInfo.outline.has_value() && !textInfo.wrapLength.has_value() && !m_quickTextLookup.contains(textInfo.text))
			{
				m_quickTextLookup.insert({ textInfo.text, textTexture });
			}
		
			return textTexture;
		}
		else
		{
			return &textLocation->second;
		}
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const UTF16String& text)
	{
		if (const auto textLocation = m_quickUTF16TextLookup.find(text);
			textLocation != std::cend(m_quickUTF16TextLookup))
		{
			return textLocation->second;
		}
		else
		{
			return Get(text::UTF16TextInfo{
				.text = text,
				.outline = NullOpt,
				.wrapLength = NullOpt,
			});
		}
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const text::UTF16TextInfo& textInfo)
	{
		if (const auto textLocation = m_utf16Textures.find(textInfo);
			textLocation == std::cend(m_utf16Textures))
		{
			const auto insertionResult = m_utf16Textures.emplace(textInfo, text::RenderText(*m_font, textInfo, m_sampler));
			ObserverPtr<const Texture> textTexture = &insertionResult.first->second;

			if (!textInfo.outline.has_value() && !textInfo.wrapLength.has_value() && !m_quickUTF16TextLookup.contains(textInfo.text))
			{
				m_quickUTF16TextLookup.insert({ textInfo.text, textTexture });
			}

			return textTexture;
		}
		else
		{
			return &textLocation->second;
		}
	}
}