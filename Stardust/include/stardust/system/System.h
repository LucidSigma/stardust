#pragma once
#ifndef STARDUST_SYSTEM_H
#define STARDUST_SYSTEM_H

#include "stardust/data/Types.h"

namespace stardust
{
    namespace system
    {
        [[nodiscard]] extern u32 GetSystemRAMCount();
    }

    namespace sys = system;
}

#endif