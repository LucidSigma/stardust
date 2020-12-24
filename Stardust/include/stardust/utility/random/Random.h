#pragma once
#ifndef STARDUST_RANDOM_H
#define STARDUST_RANDOM_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <concepts>
#include <random>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class Random
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
		inline static std::mt19937 s_randomEngine{ std::random_device()() };

	public:
		static void ReSeed();

		template <std::floating_point T = f32>
		[[nodiscard]] static T GenerateFloat(const T min = 0.0f, const T max = 1.0f)
		{
			const std::uniform_real_distribution<T> uniformDistribution(min, max);

			return uniformDistribution(s_randomEngine);
		}

		template <std::integral T = i32>
		[[nodiscard]] static T GenerateInt(const T min, const T max)
		{
			const std::uniform_int_distribution<T> uniformDistribution(min, max);

			return uniformDistribution(s_randomEngine);
		}

		[[nodiscard]] static bool FlipCoin();
		[[nodiscard]] static u32 RollDice(const u32 sideCount = 6u);

		[[nodiscard]] static bool BernoulliDistribution(const f32 probability);
		[[nodiscard]] static u32 BinomialDistribution(const u32 trials, const f32 probability, const BinomialDistributionType distributionType = BinomialDistributionType::Binomial);
		[[nodiscard]] static u32 PoissonDistribution(const u32 frequency);

		template <std::floating_point T = f32>
		[[nodiscard]] static T NormalDistribution(const T mean, const T standardDeviation)
		{
			std::normal_distribution<T> normalDistribution(mean, standardDeviation);

			return normalDistribution(s_randomEngine);
		}

		[[nodiscard]] static usize DiscreteDistribution(const Vector<f64>& weights);

		Random() = delete;
		~Random() noexcept = delete;
	};
}

#endif