#include "stardust/math/Math.h"

namespace stardust
{
    // All of these functions are based on Unity's C# implementation of SmoothDamp, which itself is based on the algorithm from Game Programming Gems 4 Chapter 1.10.
    [[nodiscard]] f32 SmoothDamp(const f32 currentValue, const f32 targetValue, f32& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime)
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        
        const f32 valueDelta = currentValue - targetValue;
        const f32 newTargetValue = currentValue - valueDelta;
        
        const f32 velocitySmoothingDelta = (velocitySmoothing + omega * valueDelta) * deltaTime;
        velocitySmoothing = (velocitySmoothing - omega * velocitySmoothingDelta) * exponential;
        
        f32 smoothingResult = newTargetValue + (valueDelta + velocitySmoothingDelta) * exponential;
        
        const bool doesOriginalValueOvershoot = targetValue - currentValue > 0.0f;
        const bool doesNewValueOvershoot = smoothingResult > targetValue;

        if (doesOriginalValueOvershoot == doesNewValueOvershoot)
        {
            smoothingResult = targetValue;
            velocitySmoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] Vec2 SmoothDamp(const Vec2 currentValue, const Vec2 targetValue, Vec2& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime)
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vec2 valueDelta = currentValue - targetValue;
        const Vec2 newTargetValue = currentValue - valueDelta;

        const Vec2 velocitySmoothingDelta = (velocitySmoothing + omega * valueDelta) * deltaTime;
        velocitySmoothing = (velocitySmoothing - omega * velocitySmoothingDelta) * exponential;

        Vec2 smoothingResult = newTargetValue + (valueDelta + velocitySmoothingDelta) * exponential;

        const Vec2 originalMinusCurrent = targetValue - currentValue;
        const Vec2 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            velocitySmoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] Vec3 SmoothDamp(const Vec3 currentValue, const Vec3 targetValue, Vec3& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime)
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vec3 valueDelta = currentValue - targetValue;
        const Vec3 newTargetValue = currentValue - valueDelta;

        const Vec3 velocitySmoothingDelta = (velocitySmoothing + omega * valueDelta) * deltaTime;
        velocitySmoothing = (velocitySmoothing - omega * velocitySmoothingDelta) * exponential;

        Vec3 smoothingResult = newTargetValue + (valueDelta + velocitySmoothingDelta) * exponential;

        const Vec3 originalMinusCurrent = targetValue - currentValue;
        const Vec3 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            velocitySmoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }
    
    [[nodiscard]] Vec4 SmoothDamp(const Vec4 currentValue, const Vec4 targetValue, Vec4& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime)
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vec4 valueDelta = currentValue - targetValue;
        const Vec4 newTargetValue = currentValue - valueDelta;

        const Vec4 velocitySmoothingDelta = (velocitySmoothing + omega * valueDelta) * deltaTime;
        velocitySmoothing = (velocitySmoothing - omega * velocitySmoothingDelta) * exponential;

        Vec4 smoothingResult = newTargetValue + (valueDelta + velocitySmoothingDelta) * exponential;

        const Vec4 originalMinusCurrent = targetValue - currentValue;
        const Vec4 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            velocitySmoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }
}