#include "stardust/animation/easings/Easings.h"

#include <numbers>

#include "stardust/math/Math.h"

namespace stardust
{
    namespace easings
    {
        [[nodiscard]] auto EaseIn(const f32 value, const f32 magnitude) -> f32
        {
            return glm::clamp(glm::pow(value, magnitude), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOut(const f32 value, const f32 magnitude) -> f32
        {
            return glm::clamp(1.0f - glm::pow(1.0f - value, magnitude), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOut(const f32 value, const f32 magnitude) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? glm::pow(2.0f, magnitude - 1.0f) * glm::pow(value, magnitude)
                    : 1.0f - glm::pow(-2.0f * value + 2.0f, magnitude) / 2.0f,
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseLinear(const f32 value) -> f32
        {
            return glm::clamp(value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInQuad(const f32 value) -> f32
        {
            return glm::clamp(value * value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutQuad(const f32 value) -> f32
        {
            return glm::clamp(1.0f - (1.0f - value) * (1.0f - value), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutQuad(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? 2.0f * value * value
                    : 1.0f - glm::pow(-2.0f * value + 2.0f, 2.0f) / 2.0f,
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseInCubic(const f32 value) -> f32
        {
            return glm::clamp(value * value * value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutCubic(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::pow(1.0f - value, 3.0f), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutCubic(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? 4.0f * value * value * value
                    : 1.0f - glm::pow(-2.0f * value + 2.0f, 3.0f) / 2.0f,
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseInQuart(const f32 value) -> f32
        {
            return glm::clamp(value * value * value * value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutQuart(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::pow(1.0f - value, 4.0f), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutQuart(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? 8.0f * value * value * value * value
                    : 1.0f - glm::pow(-2.0f * value + 2.0f, 4.0f) / 2.0f,
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseInQuint(const f32 value) -> f32
        {
            return glm::clamp(value * value * value * value * value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutQuint(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::pow(1.0f - value, 5.0f), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutQuint(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? 16.0f * value * value * value * value * value
                    : 1.0f - glm::pow(-2.0f * value + 2.0f, 5.0f) / 2.0f,
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseInSine(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::cos((value * std::numbers::pi_v<f32>) / 2.0f), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutSine(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::sin((value * std::numbers::pi_v<f32>) / 2.0f), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutSine(const f32 value) -> f32
        {
            return glm::clamp(-(glm::cos(value * std::numbers::pi_v<f32>) - 1.0f) / 2.0f, 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInExponential(const f32 value) -> f32
        {
            return glm::clamp(
                value == 0.0f
                    ? 0.0f
                    : glm::exp(10.0f * value - 10.0f),
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseOutExponential(const f32 value) -> f32
        {
            return glm::clamp(
                value == 1.0f
                    ? 1.0f
                    : 1.0f - glm::exp(-10.0f * value),
                0.0f,
                1.0f
            );
        }

        [[nodiscard]] auto EaseInOutExponential(const f32 value) -> f32
        {
            if (value == 0.0f || value == 1.0f)
            {
                return glm::clamp(value, 0.0f, 1.0f);
            }
            else if (value < 0.5f)
            {
                return glm::clamp(glm::exp(20.0f * value - 10.0f) / 2.0f, 0.0f, 1.0f);
            }
            else
            {
                return glm::clamp((2.0f - glm::exp(-20.0f * value + 10.0f)) / 2.0f, 0.0f, 1.0f);
            }
        }

        [[nodiscard]] auto EaseInCircle(const f32 value) -> f32
        {
            return glm::clamp(1.0f - glm::sqrt(1.0f - glm::pow(value, 2.0f)), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutCircle(const f32 value) -> f32
        {
            return glm::clamp(glm::sqrt(1.0f - glm::pow(value - 1.0f, 2.0f)), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseInOutCircle(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? (1.0f - glm::sqrt(1.0f - glm::pow(2.0f * value, 2.0f))) / 2.0f
                    : (glm::sqrt(1.0f - glm::pow(-2.0f * value + 2.0f, 2.0f)) + 1.0f) / 2.0f,
                0.0f,
                1.0f
            );
        }
        
        [[nodiscard]] auto EaseInBack(const f32 value) -> f32
        {
            constexpr f32 BackBounceConstant = 1.70158f;
            constexpr f32 ExtraBackBounceConstant = BackBounceConstant + 1.0f;

            return ExtraBackBounceConstant * value * value * value - BackBounceConstant * value * value;
        }

        [[nodiscard]] auto EaseOutBack(const f32 value) -> f32
        {
            constexpr f32 BackBounceConstant = 1.70158f;
            constexpr f32 ExtraBackBounceConstant = BackBounceConstant + 1.0f;

            return 1.0f + ExtraBackBounceConstant * glm::pow(value - 1.0f, 3.0f) + BackBounceConstant * glm::pow(value - 1.0f, 2.0f);
        }

        [[nodiscard]] auto EaseInOutBack(const f32 value) -> f32
        {
            constexpr f32 BackBounceConstant = 1.70158f;
            constexpr f32 ExtraBackBounceConstant = BackBounceConstant * 1.525f;

            return value < 0.5f
                ? (glm::pow(2.0f * value, 2.0f) * ((ExtraBackBounceConstant + 1.0f) * 2.0f * value - ExtraBackBounceConstant)) / 2.0f
                : (glm::pow(2.0f * value - 2.0f, 2.0f) * ((ExtraBackBounceConstant + 1.0f) * (value * 2.0f - 2.0f) + ExtraBackBounceConstant) + 2.0f) / 2.0f;
        }

        [[nodiscard]] auto EaseInElastic(const f32 value) -> f32
        {
            constexpr f32 ElasticBounceConstant = (2.0f * std::numbers::pi_v<f32>) / 3.0f;

            if (value == 0.0f || value == 1.0f)
            {
                return value;
            }
            else
            {
                return -glm::exp(10.0f * value - 10.0f) * glm::sin((value * 10.0f - 10.75f) * ElasticBounceConstant);
            }
        }
        
        [[nodiscard]] auto EaseOutElastic(const f32 value) -> f32
        {
            constexpr f32 ElasticBounceConstant = (2.0f * std::numbers::pi_v<f32>) / 3.0f;

            if (value == 0.0f || value == 1.0f)
            {
                return value;
            }
            else
            {
                return glm::exp(-10.0f * value) * glm::sin((value * 10.0f - 0.75f) * ElasticBounceConstant) + 1.0f;
            }
        }
        
        [[nodiscard]] auto EaseInOutElastic(const f32 value) -> f32
        {
            constexpr f32 ElasticBounceConstant = (2.0f * std::numbers::pi_v<f32>) / 4.5f;

            if (value == 0.0f || value == 1.0f)
            {
                return value;
            }
            else if (value < 0.5f)
            {
                return -(glm::exp(20.0f * value - 10.0f) * glm::sin((20.0f * value - 11.125f) * ElasticBounceConstant)) / 2.0f;
            }
            else
            {
                return (glm::exp(-20.0f * value + 10.0f) * glm::sin((20.0f * value - 11.125f) * ElasticBounceConstant)) / 2.0f + 1.0f;
            }
        }

        [[nodiscard]] auto EaseInBounce(const f32 value) -> f32
        {
            return glm::clamp(1.0f - EaseOutBounce(1.0f - value), 0.0f, 1.0f);
        }

        [[nodiscard]] auto EaseOutBounce(f32 value) -> f32
        {
            constexpr f32 Numerator = 7.5625f;
            constexpr f32 Denominator = 2.75f;

            if (value < 1.0f / Denominator)
            {
                return glm::clamp(Numerator * value * value, 0.0f, 1.0f);
            }
            else if (value < 2.0f / Denominator)
            {
                value -= 1.5f / Denominator;

                return glm::clamp(Numerator * value * value + 0.75f, 0.0f, 1.0f);
            }
            else if (value < 2.5f / Denominator)
            {
                value -= 2.25f / Denominator;

                return glm::clamp(Numerator * value * value + 0.9375f, 0.0f, 1.0f);
            }
            else
            {
                value -= 2.625f / Denominator;

                return glm::clamp(Numerator * value * value + 0.984375f, 0.0f, 1.0f);
            }
        }

        [[nodiscard]] auto EaseInOutBounce(const f32 value) -> f32
        {
            return glm::clamp(
                value < 0.5f
                    ? (1.0f - EaseOutBounce(1.0f - 2.0f * value)) / 2.0f
                    : (1.0f + EaseOutBounce(2.0f * value - 1.0f)) / 2.0f,
                0.0f,
                1.0f
            );
        }
        
        [[nodiscard]] auto EaseHeavisideStep(const f32 value) -> f32
        {
            return value <= 0.5f
                ? 0.0f
                : 1.0f;
        }
    }
}
