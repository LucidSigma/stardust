#pragma once
#ifndef STARDUST_SCREEN_TRANSFORM_COMPONENT_H
#define STARDUST_SCREEN_TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace components
	{
		struct ScreenTransform
		{
			IVec2 position;
			f32 rotation;
			Optional<IVec2> pivot;
			UVec2 size;

			ScreenTransform()
				: position(IVec2{ 0, 0 }), rotation(0.0f), pivot(NullOpt), size(UVec2{ 0u, 0u })
			{ }

			explicit ScreenTransform(const IVec2& position, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt, const UVec2& size = IVec2{ 0u, 0u })
				: position(position), rotation(rotation), pivot(pivot), size(size)
			{ }

			~ScreenTransform() noexcept = default;
		};
	}
}

#endif