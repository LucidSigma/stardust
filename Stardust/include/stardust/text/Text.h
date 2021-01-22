#pragma once
#ifndef STRARDUST_TEXT_H
#define STRARDUST_TEXT_H

#include <cstddef>
#include <functional>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/text/font/Font.h"

namespace stardust
{
	namespace text
	{
		struct OutlineInfo
		{
			u32 thickness;

			Colour innerColour;
			Colour outerColour;

			[[nodiscard]] bool operator ==(const OutlineInfo&) const noexcept = default;
			[[nodiscard]] bool operator !=(const OutlineInfo&) const noexcept = default;
		};

		[[nodiscard]] extern Texture RenderGlyph(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderGlyph(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderText(const Font& font, const String& text, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderText(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderWrappedText(const Font& font, const String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderWrappedText(const Font& font, const UTF16String& text, const Colour& colour, const u32 wrapLength, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderGlyphQuick(const Font& font, const char16_t glyph, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextQuick(const Font& font, const String& text, const Colour& colour, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextQuick(const Font& font, const UTF16String& text, const Colour& colour, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderGlyphWithOutline(const Font& font, const char glyph, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderGlyphWithOutline(const Font& font, const char16_t glyph, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextWithOutline(const Font& font, const String& text, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextWithOutline(const Font& font, const UTF16String& text, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderWrappedTextWithOutline(const Font& font, const String& text, const OutlineInfo& outlineInfo, const u32 wrapLength, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderWrappedTextWithOutline(const Font& font, const UTF16String& text, const OutlineInfo& outlineInfo, const u32 wrapLength, const Sampler& sampler = Sampler{ });

		[[nodiscard]] extern Texture RenderGlyphQuickWithOutline(const Font& font, const char glyph, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderGlyphQuickWithOutline(const Font& font, const char16_t glyph, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextQuickWithOutline(const Font& font, const String& text, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
		[[nodiscard]] extern Texture RenderTextQuickWithOutline(const Font& font, const UTF16String& text, const OutlineInfo& outlineInfo, const Sampler& sampler = Sampler{ });
	}
}

namespace std
{
	template <>
	struct hash<stardust::text::OutlineInfo>
	{
		[[nodiscard]] inline std::size_t operator ()(const stardust::text::OutlineInfo& outlineInfo) const noexcept
		{
			std::size_t seed = 0u;

			glm::detail::hash_combine(seed, std::hash<stardust::u32>()(outlineInfo.thickness));
			glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(outlineInfo.innerColour));
			glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(outlineInfo.outerColour));

			return seed;
		}
	};
}

#endif