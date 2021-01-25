#include "stardust/math/Math.h"

namespace stardust
{
	[[nodiscard]] f32 SmoothDamp(const f32 currentValue, f32 targetValue, f32& velocity, f32 smoothingTime, const f32 deltaTime, const f32 maxSpeed)
	{
		// Based on Unity's C# implementation of SmoothDamp, which itself is based on the algorithm from Game Programming Gems 4 Chapter 1.10.
		const f32 omega = 2.0f / smoothingTime;
		const f32 x = omega * deltaTime;
		const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		
		const f32 valueDeltaLimit = maxSpeed * smoothingTime;
		const f32 valueDelta = glm::clamp(currentValue - targetValue, -valueDeltaLimit, valueDeltaLimit);
		
		const f32 originalTargetValue = targetValue;
		targetValue = currentValue - valueDelta;
		
		const f32 velocityDelta = (velocity + omega * valueDelta) * deltaTime;
		velocity = (velocity - omega * velocityDelta) * exponential;
		
		f32 smoothingResult = targetValue + (valueDelta + velocityDelta) * exponential;
		
		const bool doesOriginalValueOvershoot = originalTargetValue - currentValue > 0.0f;
		const bool doesNewValueOvershoot = smoothingResult > originalTargetValue;

		if (doesOriginalValueOvershoot == doesNewValueOvershoot)
		{
			smoothingResult = originalTargetValue;
			velocity = (smoothingResult - originalTargetValue) / deltaTime;
		}

		return smoothingResult;
	}
}