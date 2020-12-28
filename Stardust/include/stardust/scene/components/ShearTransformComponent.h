#pragma once
#ifndef STARDUST_SHEAR_TRANSFORM_COMPONENT_H
#define STARDUST_SHEAR_TRANSFORM_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace components
	{
		struct ShearTransform
		{
			f32 xShear;
			f32 yShear;

			ShearTransform()
				: xShear(0.0f), yShear(0.0f)
			{ }

			explicit ShearTransform(const f32 xShear, const f32 yShear = 0.0f)
				: xShear(xShear), yShear(yShear)
			{ }

			explicit ShearTransform(const Vec2 shear)
				: xShear(shear.x), yShear(shear.y)
			{ }

			~ShearTransform() noexcept = default;
		};
	}
}

#endif