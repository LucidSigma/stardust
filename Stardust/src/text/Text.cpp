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
	}
}