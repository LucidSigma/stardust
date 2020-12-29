#include "stardust/text/Text.h"

#include <SDL2/SDL.h>

namespace stardust
{
	namespace
	{
		[[nodiscard]] Texture CreateTextTexture(SDL_Surface*& textSurface, const Sampler& sampler)
		{
			Texture textTexture(textSurface, true, sampler);
			SDL_FreeSurface(textSurface);
			textSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture CreateOutlinedTextTexture(SDL_Surface*& innerTextSurface, SDL_Surface*& textSurface, const u32 outlineSize, const Sampler& sampler)
		{
			SDL_Rect blitArea{
				static_cast<i32>(outlineSize), static_cast<i32>(outlineSize),
				innerTextSurface->w, innerTextSurface->h
			};

			if (SDL_BlitSurface(innerTextSurface, nullptr, textSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(textSurface);
				SDL_FreeSurface(innerTextSurface);
				textSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture(textSurface, true, sampler);

			SDL_FreeSurface(textSurface);
			SDL_FreeSurface(innerTextSurface);
			textSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}
	}

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

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderText(const Font& font, const String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderText(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderWrappedText(const Font& font, const String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended_Wrapped(font.GetRawHandle(), text.c_str(), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderWrappedText(const Font& font, const UTF16String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderGlyphQuick(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler)
		{
			return RenderGlyphQuick(font, static_cast<char16_t>(glyph), colour, sampler);
		}

		[[nodiscard]] Texture RenderGlyphQuick(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<u16>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderTextQuick(const Font& font, const String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Solid(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}

		[[nodiscard]] Texture RenderTextQuick(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateTextTexture(renderedTextSurface, sampler);
		}
		
		[[nodiscard]] Texture RenderGlyphWithOutline(const Font& font, const char glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			return RenderGlyphWithOutline(font, static_cast<char16_t>(glyph), colour, outlineSize, outlineColour, sampler);
		}

		[[nodiscard]] Texture RenderGlyphWithOutline(const Font& font, const char16_t glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<u32>(glyph), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<u32>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderTextWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended(font.GetRawHandle(), text.c_str(), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUTF8_Blended(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderTextWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderWrappedTextWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const u32 wrapLength, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended_Wrapped(font.GetRawHandle(), text.c_str(), outlineColour, wrapLength);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUTF8_Blended_Wrapped(font.GetRawHandle(), text.c_str(), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderWrappedTextWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const u32 wrapLength, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), outlineColour, wrapLength);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderGlyphQuickWithOutline(const Font& font, const char glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			return RenderGlyphQuickWithOutline(font, static_cast<char16_t>(glyph), colour, outlineSize, outlineColour, sampler);
		}

		[[nodiscard]] Texture RenderGlyphQuickWithOutline(const Font& font, const char16_t glyph, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<u16>(glyph), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<u16>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderTextQuickWithOutline(const Font& font, const String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Solid(font.GetRawHandle(), text.c_str(), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUTF8_Solid(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}

		[[nodiscard]] Texture RenderTextQuickWithOutline(const Font& font, const UTF16String& text, const Colour& colour, const u32 outlineSize, const Colour& outlineColour, const Sampler& sampler)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const u16*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, outlineSize, sampler);
		}
	}
}