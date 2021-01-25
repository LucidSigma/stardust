#pragma once
#ifndef STARDUST_SCREEN_TRANSFORM_COMPONENT_H
#define STARDUST_SCREEN_TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/math/Math.h"

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
				: position(IVec2Zero), size(UVec2Zero), flip(FlipType::None), rotation(0.0f), pivot(NullOpt)
			{ }

			explicit ScreenTransform(const IVec2& position, const UVec2& size = UVec2Zero, const FlipType flip = FlipType::None, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt)
				: position(position), size(size), flip(flip), rotation(rotation), pivot(pivot)
			{ }

			~ScreenTransform() noexcept = default;
		};
	}
}

#endif