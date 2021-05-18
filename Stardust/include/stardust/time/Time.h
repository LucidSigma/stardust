#pragma once
#ifndef STARDUST_TIME_H
#define STARDUST_TIME_H

#include "stardust/data/Types.h"

namespace stardust
{
    extern void WaitForNanoseconds(const u64 nanoseconds);
    extern void WaitForMicroseconds(const u32 microseconds);
    extern void WaitForMilliseconds(const u32 milliseconds);

    extern void WaitForSeconds(const u32 seconds);
    extern void WaitForSeconds(const f32 seconds);
}

#endif