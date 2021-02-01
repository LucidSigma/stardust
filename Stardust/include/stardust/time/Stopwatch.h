#pragma once
#ifndef STARDUST_STOPWATCH_H
#define STARDUST_STOPWATCH_H

#include <chrono>
#include <ratio>

#include "stardust/data/Types.h"

namespace stardust
{
	class Stopwatch
	{
	private:
		using Clock = std::chrono::high_resolution_clock;

		static constexpr u64 s_Precision = Clock::period::den;

		static constexpr bool s_CanCountMicroseconds = std::ratio_greater_equal_v<std::micro, Clock::period>;
		static constexpr bool s_CanCountNanoseconds = std::ratio_greater_equal_v<std::nano, Clock::period>;

		bool m_isRunning;
		bool m_isPaused = false;

		Clock::time_point m_startTimepoint;
		u64 m_pausedTickCount = 0u;

	public:
		static constexpr u64 GetPrecision() noexcept { return s_Precision; }

		static constexpr bool CanCountMicroseconds() noexcept { return s_CanCountMicroseconds; }
		static constexpr bool CanCountNanoseconds() noexcept { return s_CanCountNanoseconds; }

		explicit Stopwatch(const bool startTicking = false);
		~Stopwatch() noexcept = default;

		void Start();
		void Stop();
		void Restart();

		void Pause();
		void Resume();

		bool IsRunning();
		bool IsStopped();
		bool IsPaused();

		[[nodiscard]] u32 GetElapsedSeconds() const;
		[[nodiscard]] u32 GetElapsedMilliseconds() const;
		[[nodiscard]] u64 GetElapsedMicroseconds() const;
		[[nodiscard]] u64 GetElapsedNanoseconds() const;
		
		[[nodiscard]] bool HasPassedSeconds() const;
		[[nodiscard]] bool HasPassedMilliSeconds() const;
		[[nodiscard]] bool HasPassedMicroSeconds() const;
		[[nodiscard]] bool HasPassedNanoSeconds() const;
	};
}

#endif