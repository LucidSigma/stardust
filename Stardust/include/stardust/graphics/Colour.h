#pragma once
#ifndef COLOUR_H
#define COLOUR_H

#include <SDL2/SDL.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	using Colour = SDL_Color;

	namespace colours
	{
		constexpr Colour Black{ 0u, 0u, 0u, 255u };
		constexpr Colour Red{ 255u, 0u, 0u, 255u };
		constexpr Colour Lime{ 0u, 255u, 0u, 255u };
		constexpr Colour Blue{ 0u, 0u, 255u, 255u };
		constexpr Colour Yellow{ 255u, 255u, 0u, 255u };
		constexpr Colour Magenta{ 255u, 0u, 255u, 255u };
		constexpr Colour Cyan{ 255u, 255u, 255u, 255u };
		constexpr Colour White{ 255u, 255u, 255u, 255u };

		constexpr Colour Silver{ 192u, 192u, 192u, 255u };
		constexpr Colour Grey{ 128u, 128u, 128u, 255u };
		constexpr Colour Maroon{ 128u, 0u, 0u, 255u };
		constexpr Colour Olive{ 128u, 128u, 0u, 255u };
		constexpr Colour Green{ 0u, 128u, 0u, 255u };
		constexpr Colour Teal{ 0u, 128u, 128u, 255u };
		constexpr Colour Navy{ 0u, 0u, 128u, 255u };
		constexpr Colour Purple{ 128u, 0u, 128u, 255u };

		constexpr Colour Orange{ 255u, 128u, 0u, 255u };
		constexpr Colour Brown{ 150u, 75u, 0u, 255u };
		constexpr Colour Pink{ 255u, 192u, 203u, 255u };
		constexpr Colour Tan{ 210u, 180u, 140u, 255u };
		constexpr Colour Gold{ 255u, 215u, 0u, 255u };
		constexpr Colour Azure{ 0u, 128u, 255u, 255u };
		constexpr Colour Beige{ 245u, 245u, 220u, 255u };
		constexpr Colour Lavender{ 230u, 230u, 255u, 255u };

		constexpr Colour Clear{ 0u, 0u, 0u, 0u };
	}

	[[nodiscard]] extern Colour CreateColour(const u8 red, const u8 green, const u8 blue);
	[[nodiscard]] extern Colour CreateColour(const u8 red, const u8 green, const u8 blue, const u8 alpha);
	[[nodiscard]] extern Colour CreateColour(const f32 red, const f32 green, const f32 blue);
	[[nodiscard]] extern Colour CreateColour(const f32 red, const f32 green, const f32 blue, const f32 alpha);

	[[nodiscard]] extern Vec4 ColourToVec4(const Colour& colour);
	[[nodiscard]] extern Colour Vec4ToColour(const Vec4& vector);
}

#endif