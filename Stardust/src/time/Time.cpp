#include "stardust/types/Primitives.h"

#include <chrono>
#include <thread>

namespace stardust
{
    namespace time
    {
        auto WaitForNanoseconds(const u64 nanoseconds) -> void
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
        }

        auto WaitForMicroseconds(const u64 microseconds) -> void
        {
            std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
        }

        auto WaitForMilliseconds(const u32 milliseconds) -> void
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }

        auto WaitForSeconds(const u32 seconds) -> void
        {
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
        }

        auto WaitForSeconds(const f32 seconds) -> void
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<u32>(seconds * 1'000.0f)));
        }
    }
}
