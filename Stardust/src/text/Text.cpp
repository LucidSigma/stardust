#include "stardust/text/Text.h"

#include <SDL2/SDL.h>

#include "stardust/graphics/Colours.h"

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
        [[nodiscard]] Texture RenderGlyph(const Font& font, const GlyphInfo& glyphInfo, const Sampler& sampler)
        {
            return RenderGlyph(
                font,
                UTF16GlyphInfo{
                    .glyph = static_cast<char16_t>(glyphInfo.glyph),
                    .outline = glyphInfo.outline,
                },
                sampler
            );
        }

        [[nodiscard]] Texture RenderGlyph(const Font& font, const UTF16GlyphInfo& glyphInfo, const Sampler& sampler)
        {
            const bool hasOutline = glyphInfo.outline.has_value();

            if (hasOutline)
            {
                font.SetOutlineThickness(glyphInfo.outline->thickness);
                SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(
                    font.GetRawHandle(),
                    static_cast<u16>(glyphInfo.glyph),
                    glyphInfo.outline->outerColour
                );
                font.RemoveOutline();

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                SDL_Surface* innerTextSurface = TTF_RenderGlyph_Blended(
                    font.GetRawHandle(),
                    static_cast<u16>(glyphInfo.glyph),
                    glyphInfo.outline->innerColour
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, glyphInfo.outline->thickness, sampler);
            }
            else
            {
                SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(
                    font.GetRawHandle(),
                    static_cast<u16>(glyphInfo.glyph),
                    colours::White
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateTextTexture(renderedTextSurface, sampler);
            }
        }

        [[nodiscard]] Texture RenderText(const Font& font, const TextInfo& textInfo, const Sampler& sampler)
        {
            const bool hasOutline = textInfo.outline.has_value();
            const bool isWrapped = textInfo.wrapLength.has_value();

            if (hasOutline && isWrapped)
            {
                font.SetOutlineThickness(textInfo.outline->thickness);
                SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended_Wrapped(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    textInfo.outline->outerColour,
                    textInfo.wrapLength.value()
                );
                font.RemoveOutline();

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                SDL_Surface* innerTextSurface = TTF_RenderUTF8_Blended_Wrapped(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    textInfo.outline->innerColour,
                    textInfo.wrapLength.value()
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, textInfo.outline->thickness, sampler);
            }
            else if (hasOutline && !isWrapped)
            {
                font.SetOutlineThickness(textInfo.outline->thickness);
                SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    textInfo.outline->outerColour
                );
                font.RemoveOutline();

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                SDL_Surface* innerTextSurface = TTF_RenderUTF8_Blended(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    textInfo.outline->innerColour
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, textInfo.outline->thickness, sampler);
            }
            else if (!hasOutline && isWrapped)
            {
                SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended_Wrapped(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    colours::White,
                    textInfo.wrapLength.value()
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateTextTexture(renderedTextSurface, sampler);
            }
            else
            {
                SDL_Surface* renderedTextSurface = TTF_RenderUTF8_Blended(
                    font.GetRawHandle(),
                    textInfo.text.c_str(),
                    colours::White
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateTextTexture(renderedTextSurface, sampler);
            }
        }

        [[nodiscard]] Texture RenderText(const Font& font, const UTF16TextInfo& textInfo, const Sampler& sampler)
        {
            const bool hasOutline = textInfo.outline.has_value();
            const bool isWrapped = textInfo.wrapLength.has_value();

            if (hasOutline && isWrapped)
            {
                font.SetOutlineThickness(textInfo.outline->thickness);
                SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    textInfo.outline->outerColour,
                    textInfo.wrapLength.value()
                );
                font.RemoveOutline();

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended_Wrapped(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    textInfo.outline->innerColour,
                    textInfo.wrapLength.value()
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, textInfo.outline->thickness, sampler);
            }
            else if (hasOutline && !isWrapped)
            {
                font.SetOutlineThickness(textInfo.outline->thickness);
                SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    textInfo.outline->outerColour
                );
                font.RemoveOutline();

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    textInfo.outline->innerColour
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateOutlinedTextTexture(innerTextSurface, renderedTextSurface, textInfo.outline->thickness, sampler);
            }
            else if (!hasOutline && isWrapped)
            {
                SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    colours::White,
                    textInfo.wrapLength.value()
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateTextTexture(renderedTextSurface, sampler);
            }
            else
            {
                SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(
                    font.GetRawHandle(),
                    reinterpret_cast<const u16*>(textInfo.text.data()),
                    colours::White
                );

                if (renderedTextSurface == nullptr)
                {
                    return Texture();
                }

                return CreateTextTexture(renderedTextSurface, sampler);
            }
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
    }
}