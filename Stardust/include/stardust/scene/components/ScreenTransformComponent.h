#pragma once
#ifndef STARDUST_SCREEN_TRANSFORM_COMPONENT_H
#define STARDUST_SCREEN_TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/FlipType.h"

namespace stardust
{
	namespace components
	{
		struct ScreenTransform
		{
			IVec2 position;

			UVec2 size;
			FlipType flip;

			f32 rotation;
			Optional<IVec2> pivot;

			ScreenTransform()
				: position(IVec2{ 0, 0 }), size(UVec2{ 0u, 0u }), flip(FlipType::None), rotation(0.0f), pivot(NullOpt)
			{ }

			explicit ScreenTransform(const IVec2& position, const UVec2& size = IVec2{ 0u, 0u }, const FlipType flip = FlipType::None, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt)
				: position(position), size(size), flip(flip), rotation(rotation), pivot(pivot)
			{ }

			~ScreenTransform() noexcept = default;
		};
	}
}

#endif