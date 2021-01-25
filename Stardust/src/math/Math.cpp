#include "stardust/math/Math.h"

namespace stardust
{
	[[nodiscard]] f32 SmoothDamp(const f32 currentValue, f32 targetValue, f32& velocity, f32 smoothingTime, const f32 maxSpeed, const f32 deltaTime)
	{
		const f32 omega = 2.0f / smoothingTime;
		const f32 x = omega * deltaTime;
		const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		
		const f32 maxValueDelta = maxSpeed * smoothingTime;
		const f32 valueDelta = glm::clamp(currentValue - targetValue, -maxValueDelta, maxValueDelta);
		
		const f32 oldTargetValue = targetValue;
		targetValue = currentValue - valueDelta;
		
		const f32 velocityDelta = (velocity + omega * valueDelta) * deltaTime;
		velocity = (velocity - omega * velocityDelta) * exponential;
		
		f32 smoothingResult = targetValue + (valueDelta + velocityDelta) * exponential;
		
		if (oldTargetValue - currentValue > 0.0f == smoothingResult > oldTargetValue)
		{
			smoothingResult = oldTargetValue;
			velocity = (smoothingResult - oldTargetValue) / deltaTime;
		}

		return smoothingResult;
	}
}