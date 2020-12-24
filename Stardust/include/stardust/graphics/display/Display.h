#pragma once
#ifndef STARDUST_DISPLAY_H
#define STARDUST_DISPLAY_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace display
	{
		enum class DisplayOrientation
			: std::underlying_type_t<SDL_DisplayOrientation>
		{
			Unknown = SDL_ORIENTATION_UNKNOWN,
			Landscape = SDL_ORIENTATION_LANDSCAPE,
			LandscapeFlipped = SDL_ORIENTATION_LANDSCAPE_FLIPPED,
			Portrait = SDL_ORIENTATION_PORTRAIT,
			PortraitFlipped = SDL_ORIENTATION_PORTRAIT_FLIPPED,
		};

		struct DisplayData
		{
			String name;

			UVec2 size;
			u32 refreshRate;
			u32 format;

			Pair<IVec2, IVec2> bounds;
			Pair<IVec2, IVec2> usableBounds;

			DisplayOrientation orientation;

			f32 verticalDPI;
			f32 horizontalDPI;
			f32 diagonalDPI;
		};

		[[nodiscard]] extern u32 GetVideoDisplayCount();
		[[nodiscard]] extern u32 GetVideoDriverCount();

		[[nodiscard]] extern DisplayData GetDisplayData(const i32 displayIndex);
	}
}

#endif