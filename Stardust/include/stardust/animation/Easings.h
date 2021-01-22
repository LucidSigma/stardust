#pragma once
#ifndef STARDUST_EASINGS_H
#define STARDUST_EASINGS_H

#include <functional>

#include "stardust/data/Types.h"

namespace stardust
{
	using EasingFunction = std::function<f32(f32)>;

	namespace easings
	{
		[[nodiscard]] extern f32 EaseLinear(const f32 value);

		[[nodiscard]] extern f32 EaseInQuad(const f32 value);
		[[nodiscard]] extern f32 EaseOutQuad(const f32 value);
		[[nodiscard]] extern f32 EaseInOutQuad(const f32 value);

		[[nodiscard]] extern f32 EaseInCubic(const f32 value);
		[[nodiscard]] extern f32 EaseOutCubic(const f32 value);
		[[nodiscard]] extern f32 EaseInOutCubic(const f32 value);

		[[nodiscard]] extern f32 EaseInQuart(const f32 value);
		[[nodiscard]] extern f32 EaseOutQuart(const f32 value);
		[[nodiscard]] extern f32 EaseInOutQuart(const f32 value);

		[[nodiscard]] extern f32 EaseInQuint(const f32 value);
		[[nodiscard]] extern f32 EaseOutQuint(const f32 value);
		[[nodiscard]] extern f32 EaseInOutQuint(const f32 value);

		[[nodiscard]] extern f32 EaseInSine(const f32 value);
		[[nodiscard]] extern f32 EaseOutSine(const f32 value);
		[[nodiscard]] extern f32 EaseInOutSine(const f32 value);

		[[nodiscard]] extern f32 EaseInExponential(const f32 value);
		[[nodiscard]] extern f32 EaseOutExponential(const f32 value);
		[[nodiscard]] extern f32 EaseInOutExponential(const f32 value);

		[[nodiscard]] extern f32 EaseInCircle(const f32 value);
		[[nodiscard]] extern f32 EaseOutCircle(const f32 value);
		[[nodiscard]] extern f32 EaseInOutCircle(const f32 value);

		[[nodiscard]] extern f32 EaseInBounce(const f32 value);
		[[nodiscard]] extern f32 EaseOutBounce(f32 value);
		[[nodiscard]] extern f32 EaseInOutBounce(const f32 value);
	}
}

#endif