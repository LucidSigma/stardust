#pragma once
#ifndef STARDUST_FLIP_TYPE_H
#define STARDUST_FLIP_TYPE_H

#include "stardust/data/MathTypes.h"

namespace stardust
{
	enum class FlipType
	{
		None,
		Horizontal,
		Vertical,
		Both,
	};

	[[nodiscard]] extern Vec2 GetScaleFromFlipType(const FlipType flipType) noexcept;
}

#endif