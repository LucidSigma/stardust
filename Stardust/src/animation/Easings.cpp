#include "stardust/animation/Easings.h"

#include "stardust/math/Math.h"

namespace stardust
{
	namespace easings
	{
		[[nodiscard]] f32 EaseLinear(const f32 value)
		{
			return value;
		}

		[[nodiscard]] f32 EaseInQuad(const f32 value)
		{
			return value * value;
		}

		[[nodiscard]] f32 EaseOutQuad(const f32 value)
		{
			return 1.0f - (1.0f - value) * (1.0f - value);
		}

		[[nodiscard]] f32 EaseInOutQuad(const f32 value)
		{
			return value < 0.5f
				? 2.0f * value * value
				: 1.0f - glm::pow(-2.0f * value + 2.0f, 2.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInCubic(const f32 value)
		{
			return value * value * value;
		}

		[[nodiscard]] f32 EaseOutCubic(const f32 value)
		{
			return 1.0f - glm::pow(1.0f - value, 3.0f);
		}

		[[nodiscard]] f32 EaseInOutCubic(const f32 value)
		{
			return value < 0.5f
				? 4.0f * value * value * value
				: 1.0f - glm::pow(-2.0f * value + 2.0f, 3.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInQuart(const f32 value)
		{
			return value * value * value * value;
		}

		[[nodiscard]] f32 EaseOutQuart(const f32 value)
		{
			return 1.0f - glm::pow(1.0f - value, 4.0f);
		}

		[[nodiscard]] f32 EaseInOutQuart(const f32 value)
		{
			return value < 0.5f
				? 8.0f * value * value * value * value
				: 1.0f - glm::pow(-2.0f * value + 2.0f, 4.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInQuint(const f32 value)
		{
			return value * value * value * value * value;
		}

		[[nodiscard]] f32 EaseOutQuint(const f32 value)
		{
			return 1.0f - glm::pow(1.0f - value, 5.0f);
		}

		[[nodiscard]] f32 EaseInOutQuint(const f32 value)
		{
			return value < 0.5f
				? 16.0f * value * value * value * value * value
				: 1.0f - glm::pow(-2.0f * value + 2.0f, 5.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInSine(const f32 value)
		{
			return 1.0f - glm::cos((value * std::numbers::pi_v<f32>) / 2.0f);
		}

		[[nodiscard]] f32 EaseOutSine(const f32 value)
		{
			return 1.0f - glm::sin((value * std::numbers::pi_v<f32>) / 2.0f);
		}

		[[nodiscard]] f32 EaseInOutSine(const f32 value)
		{
			return -(glm::cos(value * std::numbers::pi_v<f32>) - 1.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInExponential(const f32 value)
		{
			return value == 0.0f
				? 0.0f
				: glm::exp(10.0f * value - 10.0f);
		}

		[[nodiscard]] f32 EaseOutExponential(const f32 value)
		{
			return value == 1.0f
				? 1.0f
				: 1.0f - glm::exp(-10.0f * value);
		}

		[[nodiscard]] f32 EaseInOutExponential(const f32 value)
		{
			if (value == 0.0f || value == 1.0f)
			{
				return value;
			}
			else if (value < 0.5f)
			{
				return glm::exp(20.0f * value - 10.0f) / 2.0f;
			}
			else
			{
				return (2.0f - glm::exp(-20.0f * value + 10.0f)) / 2.0f;
			}
		}

		[[nodiscard]] f32 EaseInCircle(const f32 value)
		{
			return 1.0f - glm::sqrt(1.0f - glm::pow(value, 2.0f));
		}

		[[nodiscard]] f32 EaseOutCircle(const f32 value)
		{
			return glm::sqrt(1.0f - glm::pow(value - 1.0f, 2.0f));
		}

		[[nodiscard]] f32 EaseInOutCircle(const f32 value)
		{
			return value < 0.5f
				? (1.0f - glm::sqrt(1.0f - glm::pow(2.0f * value, 2.0f))) / 2.0f
				: (glm::sqrt(1.0f - glm::pow(-2.0f * value + 2.0f, 2.0f)) + 1.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInBack(const f32 value)
		{
			constexpr f32 BackBounceConstant = 1.70158f;
			constexpr f32 ExtraBackBounceConstant = BackBounceConstant + 1.0f;

			return ExtraBackBounceConstant * value * value * value - BackBounceConstant * value * value;
		}

		[[nodiscard]] f32 EaseOutBack(const f32 value)
		{
			constexpr f32 BackBounceConstant = 1.70158f;
			constexpr f32 ExtraBackBounceConstant = BackBounceConstant + 1.0f;

			return 1.0f + ExtraBackBounceConstant * glm::pow(value - 1.0f, 3.0f) + BackBounceConstant * glm::pow(value - 1.0f, 2.0f);
		}

		[[nodiscard]] f32 EaseInOutBack(const f32 value)
		{
			constexpr f32 BackBounceConstant = 1.70158f;
			constexpr f32 ExtraBackBounceConstant = BackBounceConstant * 1.525f;

			return value < 0.5f
				? (glm::pow(2.0f * value, 2.0f) * ((ExtraBackBounceConstant + 1.0f) * 2.0f * value - ExtraBackBounceConstant)) / 2.0f
				: (glm::pow(2.0f * value - 2.0f, 2.0f) * ((ExtraBackBounceConstant + 1.0f) * (value * 2.0f - 2.0f) + ExtraBackBounceConstant) + 2.0f) / 2.0f;
		}

		[[nodiscard]] f32 EaseInElastic(const f32 value)
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
		
		[[nodiscard]] f32 EaseOutElastic(const f32 value)
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
		
		[[nodiscard]] f32 EaseInOutElastic(const f32 value)
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

		[[nodiscard]] f32 EaseInBounce(const f32 value)
		{
			return 1.0f - EaseOutBounce(1.0f - value);
		}

		[[nodiscard]] f32 EaseOutBounce(f32 value)
		{
			constexpr f32 Numerator = 7.5625f;
			constexpr f32 Denominator = 2.75f;

			if (value < 1.0f / Denominator)
			{
				return Numerator * value * value;
			}
			else if (value < 2.0f / Denominator)
			{
				value -= 1.5f / Denominator;

				return Numerator * value * value + 0.75f;
			}
			else if (value < 2.5f / Denominator)
			{
				value -= 2.25f / Denominator;

				return Numerator * value * value + 0.9375f;
			}
			else
			{
				value -= 2.625f / Denominator;

				return Numerator * value * value + 0.984375f;
			}
		}

		[[nodiscard]] f32 EaseInOutBounce(const f32 value)
		{
			return value < 0.5f
				? (1.0f - EaseOutBounce(1.0f - 2.0f * value)) / 2.0f
				: (1.0f + EaseOutBounce(2.0f * value - 1.0f)) / 2.0f;
		}
	}

	[[nodiscard]] Optional<EasingFunction> GetEasingFunctionFromString(const String& easingFunction)
	{
		static const HashMap<String, EasingFunction> easingFunctionLookup{
			{ "linear", easings::EaseLinear },

			{ "quad-in", easings::EaseInQuad },
			{ "quad-out", easings::EaseOutQuad },
			{ "quad-in-out", easings::EaseInOutQuad },

			{ "cubic-in", easings::EaseInCubic },
			{ "cubic-out", easings::EaseOutCubic },
			{ "cubic-in-out", easings::EaseInOutCubic },

			{ "quart-in", easings::EaseInQuart },
			{ "quart-out", easings::EaseOutQuart },
			{ "quart-in-out", easings::EaseInOutQuart },

			{ "quint-in", easings::EaseInQuint },
			{ "quint-out", easings::EaseOutQuint },
			{ "quint-in-out", easings::EaseInOutQuint },

			{ "sine-in", easings::EaseInSine },
			{ "sine-out", easings::EaseOutSine },
			{ "sine-in-out", easings::EaseInOutSine },

			{ "exponential-in", easings::EaseInExponential },
			{ "exponential-out", easings::EaseOutExponential },
			{ "exponential-in-out", easings::EaseInOutExponential },

			{ "circle-in", easings::EaseInCircle },
			{ "circle-out", easings::EaseOutCircle },
			{ "circle-in-out", easings::EaseInOutCircle },

			{ "back-in", easings::EaseInBack },
			{ "back-out", easings::EaseOutBack },
			{ "back-in-out", easings::EaseInOutBack },

			{ "elastic-in", easings::EaseInElastic },
			{ "elastic-out", easings::EaseOutElastic },
			{ "elastic-in-out", easings::EaseInOutElastic },

			{ "bounce-in", easings::EaseInBounce },
			{ "bounce-out", easings::EaseOutBounce },
			{ "bounce-in-out", easings::EaseInOutBounce },
		};

		if (const auto easingFunctionLocation = easingFunctionLookup.find(easingFunction);
			easingFunctionLocation != std::cend(easingFunctionLookup))
		{
			return easingFunctionLocation->second;
		}
		else
		{
			return NullOpt;
		}
	}
}