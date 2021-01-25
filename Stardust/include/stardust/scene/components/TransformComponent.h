#pragma once
#ifndef STARDUST_TRANSFORM_COMPONENT_H
#define STARDUST_TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"

namespace stardust
{
	namespace components
	{
		struct Transform
		{
			Vec2 position;
			f32 rotation;
			Optional<Vec2> pivot;
			Vec2 scale;

			Transform()
				: position(Vec2Zero), rotation(0.0f), pivot(NullOpt), scale(Vec2One)
			{ }

			explicit Transform(const Vec2& position, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt, const Vec2& scale = Vec2One)
				: position(position), rotation(rotation), pivot(pivot), scale(scale)
			{ }

			~Transform() noexcept = default;
		};
	}
}

#endif