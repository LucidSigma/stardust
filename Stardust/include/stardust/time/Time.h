#pragma once
#ifndef STARDUST_TIME_H
#define STARDUST_TIME_H

#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace time
    {
        extern auto WaitForNanoseconds(const u64 nanoseconds) -> void;
        extern auto WaitForMicroseconds(const u64 microseconds) -> void;
        extern auto WaitForMilliseconds(const u32 milliseconds) -> void;

        extern auto WaitForSeconds(const u32 seconds) -> void;
        extern auto WaitForSeconds(const f32 seconds) -> void;
    }
}

#endif
