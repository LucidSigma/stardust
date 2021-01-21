#pragma once
#ifndef STARDUST_TEXT_CACHE_H
#define STARDUST_TEXT_CACHE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/text/font/Font.h"

namespace stardust
{
	class TextCache
		: private INoncopyable
	{
	private:
		struct OutlineInfo
		{
			u32 thickness;

			Colour innerColour;
			Colour outerColour;
		};

		ObserverPtr<const Font> m_font = nullptr;
		Sampler m_sampler{ };

		HashMap<String, Texture> m_textures{ };
		HashMap<UTF16String, Texture> m_utf16Textures{ };

	public:
		TextCache() = default;
		TextCache(const Font& font, const Sampler& sampler = Sampler{ });
		~TextCache() noexcept;

		void Initialise(const Font& font, const Sampler& sampler = Sampler{ });

		[[nodiscard]] ObserverPtr<const Texture> Get(const String& text);
		[[nodiscard]] ObserverPtr<const Texture> Get(const UTF16String& text);

		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const String& text) { return Get(text); }
		[[nodiscard]] inline ObserverPtr<const Texture> operator [](const UTF16String& text) { return Get(text); }
	};
}

#endif