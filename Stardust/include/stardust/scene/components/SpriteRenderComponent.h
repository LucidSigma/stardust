#pragma once
#ifndef STARDUST_SPRITE_RENDER_COMPONENT_H
#define STARDUST_SPRITE_RENDER_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/SortingLayer.h"

namespace stardust
{
	namespace components
	{
		struct SpriteRender
		{
			ObserverPtr<const Texture> texture;
			ObserverPtr<const SortingLayer> sortingLayer;

			Optional<CoordinatePair> subTextureArea;
			Colour colourMod;

			SpriteRender()
				: texture(nullptr), sortingLayer(nullptr), subTextureArea(NullOpt), colourMod(colours::White)
			{ }

			SpriteRender(const Texture& texture, const SortingLayer& sortingLayer, const Optional<CoordinatePair>& subtextureArea = NullOpt, const Colour colourMod = colours::White)
				: texture(&texture), sortingLayer(&sortingLayer), subTextureArea(subtextureArea), colourMod(colourMod)
			{ }

			~SpriteRender() noexcept = default;
		};
	}
}

#endif