#include "stardust/text/TextCache.h"

#include <utility>

#include "stardust/graphics/Colour.h"
#include "stardust/text/Text.h"

namespace stardust
{
	TextCache::TextCache(const Font& font, const Sampler& sampler)
	{
		Initialise(font, sampler);
	}

	TextCache::~TextCache() noexcept
	{
		m_textures.clear();
		m_utf16Textures.clear();
	}

	void TextCache::Initialise(const Font& font, const Sampler& sampler)
	{
		m_font = &font;
		m_sampler = sampler;
	}
	
	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const String& text)
	{
		const auto textLocation = m_textures.find(text);

		if (textLocation == std::cend(m_textures))
		{
			const auto insertionResult = m_textures.emplace(text, text::RenderText(*m_font, text, colours::White, m_sampler));

			return &insertionResult.first->second;
		}
		else
		{
			return &textLocation->second;
		}
	}

	[[nodiscard]] ObserverPtr<const Texture> TextCache::Get(const UTF16String& text)
	{
		const auto textLocation = m_utf16Textures.find(text);

		if (textLocation == std::cend(m_utf16Textures))
		{
			const auto insertionResult = m_utf16Textures.emplace(text, text::RenderText(*m_font, text, colours::White, m_sampler));

			return &insertionResult.first->second;
		}
		else
		{
			return &textLocation->second;
		}
	}
}