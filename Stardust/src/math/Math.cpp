#include "stardust/math/Math.h"

namespace stardust
{
    [[nodiscard]] auto SmoothDamp(const f32 currentValue, const f32 targetValue, f32& smoothing, const f32 smoothingTime, const f32 deltaTime) -> f32
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const f32 valueDelta = currentValue - targetValue;
        const f32 newTargetValue = currentValue - valueDelta;

        const f32 smoothingDelta = (smoothing + omega * valueDelta) * deltaTime;
        smoothing = (smoothing - omega * smoothingDelta) * exponential;

        f32 smoothingResult = newTargetValue + (valueDelta + smoothingDelta) * exponential;

        const bool doesOriginalValueOvershoot = targetValue - currentValue > 0.0f;
        const bool doesNewValueOvershoot = smoothingResult > targetValue;

        if (doesOriginalValueOvershoot == doesNewValueOvershoot)
        {
            smoothingResult = targetValue;
            smoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] auto SmoothDamp(const Vector2 currentValue, const Vector2 targetValue, Vector2& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector2
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vector2 valueDelta = currentValue - targetValue;
        const Vector2 newTargetValue = currentValue - valueDelta;

        const Vector2 smoothingDelta = (smoothing + omega * valueDelta) * deltaTime;
        smoothing = (smoothing - omega * smoothingDelta) * exponential;

        Vector2 smoothingResult = newTargetValue + (valueDelta + smoothingDelta) * exponential;

        const Vector2 originalMinusCurrent = targetValue - currentValue;
        const Vector2 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            smoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] auto SmoothDamp(const Vector3 currentValue, const Vector3 targetValue, Vector3& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector3
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vector3 valueDelta = currentValue - targetValue;
        const Vector3 newTargetValue = currentValue - valueDelta;

        const Vector3 smoothingDelta = (smoothing + omega * valueDelta) * deltaTime;
        smoothing = (smoothing - omega * smoothingDelta) * exponential;

        Vector3 smoothingResult = newTargetValue + (valueDelta + smoothingDelta) * exponential;

        const Vector3 originalMinusCurrent = targetValue - currentValue;
        const Vector3 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            smoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] auto SmoothDamp(const Vector4 currentValue, const Vector4 targetValue, Vector4& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector4
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Vector4 valueDelta = currentValue - targetValue;
        const Vector4 newTargetValue = currentValue - valueDelta;

        const Vector4 smoothingDelta = (smoothing + omega * valueDelta) * deltaTime;
        smoothing = (smoothing - omega * smoothingDelta) * exponential;

        Vector4 smoothingResult = newTargetValue + (valueDelta + smoothingDelta) * exponential;

        const Vector4 originalMinusCurrent = targetValue - currentValue;
        const Vector4 resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            smoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] auto SmoothDamp(const Quaternion& currentValue, const Quaternion& targetValue, Quaternion& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Quaternion
    {
        const f32 omega = 2.0f / smoothingTime;
        const f32 x = omega * deltaTime;
        const f32 exponential = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        const Quaternion valueDelta = currentValue - targetValue;
        const Quaternion newTargetValue = currentValue - valueDelta;

        const Quaternion smoothingDelta = (smoothing + omega * valueDelta) * deltaTime;
        smoothing = (smoothing - omega * smoothingDelta) * exponential;

        Quaternion smoothingResult = newTargetValue + (valueDelta + smoothingDelta) * exponential;

        const Quaternion originalMinusCurrent = targetValue - currentValue;
        const Quaternion resultMinusOriginal = smoothingResult - targetValue;

        if (glm::dot(originalMinusCurrent, resultMinusOriginal) > 0.0f)
        {
            smoothingResult = targetValue;
            smoothing = (smoothingResult - targetValue) / deltaTime;
        }

        return smoothingResult;
    }

    [[nodiscard]] auto SetMagnitude(const Vector2 vector, const f32 magnitude) -> Vector2
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] auto SetMagnitude(const Vector3 vector, const f32 magnitude) -> Vector3
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] auto SetMagnitude(const Vector4 vector, const f32 magnitude) -> Vector4
    {
        return glm::normalize(vector) * magnitude;
    }

    [[nodiscard]] auto LimitMagnitude(const Vector2 vector, const f32 maxMagnitude) -> Vector2
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] auto LimitMagnitude(const Vector3 vector, const f32 maxMagnitude) -> Vector3
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] auto LimitMagnitude(const Vector4 vector, const f32 maxMagnitude) -> Vector4
    {
        const f32 magnitude = glm::length(vector);

        return magnitude > maxMagnitude ? SetMagnitude(vector, maxMagnitude) : vector;
    }

    [[nodiscard]] auto InterpolateAngles(const f32 startAngle, const f32 endAngle, const f32 amount) -> f32
    {
        const Quaternion startQuaternion = glm::angleAxis(glm::radians(startAngle), Vector3Forward);
        const Quaternion endQuaternion = glm::angleAxis(glm::radians(endAngle), Vector3Forward);

        return glm::degrees(glm::roll(glm::slerp(startQuaternion, endQuaternion, amount)));
    }

    [[nodiscard]] auto InterpolateAngles(const f32 startAngle, const f32 endAngle, const f32 amount, const i32 additionalSpinCount) -> f32
    {
        const Quaternion startQuaternion = glm::angleAxis(glm::radians(startAngle), Vector3Forward);
        const Quaternion endQuaternion = glm::angleAxis(glm::radians(endAngle), Vector3Forward);

        return glm::degrees(glm::roll(glm::slerp(startQuaternion, endQuaternion, amount, additionalSpinCount)));
    }
}
