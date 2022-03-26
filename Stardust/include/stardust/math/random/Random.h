#pragma once
#ifndef STARDUST_RANDOM_H
#define STARDUST_RANDOM_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <concepts>
#include <random>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Random final
        : private INoncopyable, private INonmovable
    {
    public:
        enum class BinomialDistributionType
        {
            Binomial,
            NegativeBinomial,
            Geometric,
        };

    private:
        inline static std::mt19937_64 s_randomEngine{ std::random_device()() };

    public:
        static auto ReSeed() -> void;
        static auto ReSeed(const u64 seed) -> void;

        template <std::floating_point T = f32>
        [[nodiscard]] static auto GenerateFloat(const T min = 0.0f, const T max = 1.0f) -> T
        {
            const std::uniform_real_distribution<T> uniformDistribution(min, max);

            return uniformDistribution(s_randomEngine);
        }

        template <std::integral T = i32>
        [[nodiscard]] static auto GenerateInt(const T min, const T max) -> T
        {
            const std::uniform_int_distribution<T> uniformDistribution(min, max - 1u);

            return uniformDistribution(s_randomEngine);
        }

        [[nodiscard]] static auto GenerateSign() -> i32;
        [[nodiscard]] static auto FlipCoin() -> bool;
        [[nodiscard]] static auto RollDice(const u32 sideCount = 6u) -> u32;

        [[nodiscard]] static auto BernoulliDistribution(const f32 probability) -> bool;
        [[nodiscard]] static auto BinomialDistribution(const u32 trials, const f32 probability, const BinomialDistributionType distributionType = BinomialDistributionType::Binomial) -> u32;
        [[nodiscard]] static auto PoissonDistribution(const u32 frequency) -> u32;

        template <std::floating_point T = f32>
        [[nodiscard]] static auto NormalDistribution(const T mean, const T standardDeviation) -> T
        {
            std::normal_distribution<T> normalDistribution(mean, standardDeviation);

            return normalDistribution(s_randomEngine);
        }

        [[nodiscard]] static auto DiscreteDistribution(const List<f64>& weights) -> usize;

        Random() = delete;
        ~Random() noexcept = delete;
    };
}

#endif
