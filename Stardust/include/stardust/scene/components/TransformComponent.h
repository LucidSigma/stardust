#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace components
	{
		struct Transform
		{
			Vec2 position;
			f32 rotation;
			Vec2 scale;

			Transform()
				: position(Vec2{ 0.0f, 0.0f }), rotation(0.0f), scale(Vec2{ 1.0f, 1.0f })
			{ }

			explicit Transform(const Vec2& position, const f32 rotation = 0.0f, const Vec2& scale = Vec2{ 1.0f, 1.0f })
				: position(position), rotation(rotation), scale(scale)
			{ }

			~Transform() noexcept = default;
		};
	}
}

#endif