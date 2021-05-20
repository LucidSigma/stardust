#pragma once
#ifndef STARDUST_VERSION_H
#define STARDUST_VERSION_H

#include "stardust/data/Types.h"

namespace stardust
{
    struct VersionInfo
    {
        u32 major;
        u32 minor;
        u32 patch;
    };

    constexpr VersionInfo Version{
        .major = 1u,
        .minor = 0u,
        .patch = 7u,
    };
}

#endif