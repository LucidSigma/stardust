#include "stardust/graphics/Colour.h"

#include "stardust/data/Types.h"

namespace stardust
{
	[[nodiscard]] Vec4 ColourToVec4(const Colour& colour)
	{
		return Vec4{
			colour.r,
			colour.g,
			colour.b,
			colour.a,
		};
	}

	[[nodiscard]] Colour Vec4ToColour(const Vec4& vector)
	{
		return Colour{
			static_cast<u8>(vector.x),
			static_cast<u8>(vector.y),
			static_cast<u8>(vector.z),
			static_cast<u8>(vector.w),
		};
	}
}