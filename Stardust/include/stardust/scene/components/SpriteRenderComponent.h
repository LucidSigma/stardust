#pragma once
#ifndef STARDUST_SPRITE_RENDER_COMPONENT_H
#define STARDUST_SPRITE_RENDER_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Colour.h"

namespace stardust
{
	namespace components
	{
		struct SpriteRender
		{
			ObserverPtr<const Texture> texture;
			Optional<Pair<Vec2, Vec2>> subTextureArea;

			f32 z;

			Colour colourMod;

			SpriteRender()
				: texture(nullptr), subTextureArea(NullOpt), z(0.0f), colourMod(colours::White)
			{ }

			explicit SpriteRender(const Texture& texture, const Optional<Pair<Vec2, Vec2>>& subtextureArea = NullOpt, const f32 z = 0.0f, const Colour colourMod = colours::White)
				: texture(&texture), subTextureArea(subtextureArea), z(z), colourMod(colourMod)
			{ }

			~SpriteRender() noexcept = default;
		};
	}
}

#endif