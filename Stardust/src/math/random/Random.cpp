#include "stardust/math/random/Random.h"

#include "stardust/math/Math.h"

namespace stardust
{
    auto Random::ReSeed() -> void
    {
        s_randomEngine.seed(std::random_device()());
    }

    auto Random::ReSeed(const u64 seed) -> void
    {
        s_randomEngine.seed(seed);
    }

    [[nodiscard]] auto Random::GenerateSign() -> i32
    {
        return GenerateInt(-1, 1);
    }

    [[nodiscard]] auto Random::FlipCoin() -> bool
    {
        return GenerateInt(0, 1) == 0;
    }

    [[nodiscard]] auto Random::RollDice(const u32 sideCount) -> u32
    {
        return GenerateInt<u32>(1u, sideCount);
    }

    [[nodiscard]] auto Random::BernoulliDistribution(const f32 probability) -> bool
    {
        const std::bernoulli_distribution bernoulliDistribution(glm::clamp(probability, 0.0f, 1.0f));

        return bernoulliDistribution(s_randomEngine);
    }

    [[nodiscard]] auto Random::BinomialDistribution(const u32 trials, const f32 probability, const BinomialDistributionType distributionType) -> u32
    {
        switch (distributionType)
        {
        case BinomialDistributionType::NegativeBinomial:
        {
            const std::negative_binomial_distribution negativeNinomialDistribution(trials, glm::clamp(probability, 0.0f, 1.0f));

            return negativeNinomialDistribution(s_randomEngine);
        }

        case BinomialDistributionType::Geometric:
        {
            const std::geometric_distribution<u32> geometricDistribution(glm::clamp(probability, 0.0f, 1.0f));

            return geometricDistribution(s_randomEngine);
        }

        case BinomialDistributionType::Binomial:
        {
            const std::binomial_distribution binomialDistribution(trials, glm::clamp(probability, 0.0f, 1.0f));

            return binomialDistribution(s_randomEngine);
        }
        
        [[unlikely]] default:
            return 0u;
        }
    }

    [[nodiscard]] auto Random::PoissonDistribution(const u32 frequency) -> u32
    {
        const std::poisson_distribution poissonDistribution(frequency);

        return poissonDistribution(s_randomEngine);
    }

    [[nodiscard]] auto Random::DiscreteDistribution(const List<f64>& weights) -> usize
    {
        const std::discrete_distribution discreteDistribution(std::cbegin(weights), std::cend(weights));

        return discreteDistribution(s_randomEngine);
    }
}