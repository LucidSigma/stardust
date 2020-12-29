#include "stardust/text/Text.h"

namespace stardust
{
	namespace text
	{
		[[nodiscard]] Texture RenderGlyph(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler)
		{
			return RenderGlyph(font, static_cast<char16_t>(glyph), colour, sampler);
		}

		[[nodiscard]] Texture RenderGlyph(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<u16>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture(renderedTextSurface, true, sampler);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		Texture RenderText(const Font& font, const String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderText_Blended(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture(renderedTextSurface, true, sampler);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderText(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture(renderedTextSurface, true, sampler);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}
	}
}