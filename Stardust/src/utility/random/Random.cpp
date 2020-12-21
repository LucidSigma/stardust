#include "stardust/utility/random/Random.h"

#include <algorithm>

namespace stardust
{
	void Random::ReSeed()
	{
		s_randomEngine.seed(std::random_device()());
	}

	[[nodiscard]] bool Random::FlipCoin()
	{
		return GenerateInt(0, 1) == 0;
	}

	[[nodiscard]] u32 Random::RollDice(const u32 sideCount)
	{
		return GenerateInt<u32>(1u, sideCount);
	}

	[[nodiscard]] bool Random::BernoulliDistribution(const f32 probability)
	{
		const std::bernoulli_distribution bernoulliDistribution(std::clamp(probability, 0.0f, 1.0f));

		return bernoulliDistribution(s_randomEngine);
	}

	[[nodiscard]] u32 Random::BinomialDistribution(const u32 trials, const f32 probability, const BinomialDistributionType distributionType)
	{
		switch (distributionType)
		{
		case BinomialDistributionType::NegativeBinomial:
		{
			const std::negative_binomial_distribution negativeNinomialDistribution(trials, std::clamp(probability, 0.0f, 1.0f));

			return negativeNinomialDistribution(s_randomEngine);
		}

		case BinomialDistributionType::Geometric:
		{
			const std::geometric_distribution<u32> geometricDistribution(std::clamp(probability, 0.0f, 1.0f));

			return geometricDistribution(s_randomEngine);
		}

		case BinomialDistributionType::Binomial:
		default:
		{
			const std::binomial_distribution binomialDistribution(trials, std::clamp(probability, 0.0f, 1.0f));

			return binomialDistribution(s_randomEngine);
		}
		}
	}

	[[nodiscard]] u32 Random::PoissonDistribution(const u32 frequency)
	{
		const std::poisson_distribution poissonDistribution(frequency);

		return poissonDistribution(s_randomEngine);
	}

	[[nodiscard]] usize Random::DiscreteDistribution(const Vector<f64>& weights)
	{
		const std::discrete_distribution discreteDistribution(std::cbegin(weights), std::cend(weights));

		return discreteDistribution(s_randomEngine);
	}
}