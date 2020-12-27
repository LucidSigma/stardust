#pragma once
#ifndef STARDUST_SPRITE_RENDER_COMPONENT_H
#define STARDUST_SPRITE_RENDER_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"

namespace stardust
{
	namespace components
	{
		struct SpriteRender
		{
			ObserverPtr<const Texture> texture;
			Optional<Pair<Vec2, Vec2>> subTextureArea;

			f32 z;

			SpriteRender()
				: texture(nullptr), subTextureArea(NullOpt), z(0.0f)
			{ }

			explicit SpriteRender(const Texture& texture, const Optional<Pair<Vec2, Vec2>>& subtextureArea = NullOpt, const f32 z = 0.0f)
				: texture(&texture), subTextureArea(subtextureArea), z(z)
			{ }

			~SpriteRender() noexcept = default;
		};
	}
}

#endif