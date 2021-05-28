#include "stardust/math/Math.h"

#include "stardust/utility/random/Random.h"

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

    [[nodiscard]] Vec2 SetMagnitude(const Vec2& vector, const f32 magnitude)
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] Vec3 SetMagnitude(const Vec3& vector, const f32 magnitude)
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] Vec4 SetMagnitude(const Vec4& vector, const f32 magnitude)
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] Vec2 LimitMagnitude(const Vec2& vector, const f32 maxMagnitude)
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] Vec3 LimitMagnitude(const Vec3& vector, const f32 maxMagnitude)
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] Vec4 LimitMagnitude(const Vec4& vector, const f32 maxMagnitude)
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] Vec2 RandomVec2(const f32 min, const f32 max)
    {
        return Vec2{ Random::GenerateFloat(min, max), Random::GenerateFloat(min, max) };
    }

    [[nodiscard]] Vec3 RandomVec3(const f32 min, const f32 max)
    {
        return Vec3{ Random::GenerateFloat(min, max), Random::GenerateFloat(min, max), Random::GenerateFloat(min, max) };
    }

    [[nodiscard]] Vec4 RandomVec4(const f32 min, const f32 max)
    {
        return Vec4{ Random::GenerateFloat(min, max), Random::GenerateFloat(min, max), Random::GenerateFloat(min, max), Random::GenerateFloat(min, max) };
    }

    [[nodiscard]] Vec2 RandomUnitVec2()
    {
        return glm::normalize(RandomVec2(-1.0f, 1.0f));
    }

    [[nodiscard]] Vec3 RandomUnitVec3()
    {
        return glm::normalize(RandomVec3(-1.0f, 1.0f));
    }

    [[nodiscard]] Vec4 RandomUnitVec4()
    {
        return glm::normalize(RandomVec4(-1.0f, 1.0f));
    }
}