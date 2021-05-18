#include "stardust/time/Time.h"

#include <chrono>
#include <thread>

namespace stardust
{
    void WaitForNanoseconds(const u64 nanoseconds)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }

    void WaitForMicroseconds(const u32 microseconds)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }

    void WaitForMilliseconds(const u32 milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void WaitForSeconds(const u32 seconds)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void WaitForSeconds(const f32 seconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<u32>(seconds * 1'000.0f)));
    }
}