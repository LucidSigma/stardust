#include "stardust/graphics/renderer/FlipType.h"

namespace stardust
{
	Vec2 GetScaleFromFlipType(const FlipType flipType) noexcept
	{
		Vec2 flipScale{ 1.0f, 1.0f };

		switch (flipType)
		{
		case FlipType::Horizontal:
			flipScale.x *= -1.0f;

			break;

		case FlipType::Vertical:
			flipScale.y *= -1.0f;

			break;

		case FlipType::Both:
			flipScale *= -1.0f;

			break;

		case FlipType::None:
		default:
			break;
		}

		return flipScale;
	}
}