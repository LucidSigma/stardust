#pragma once
#ifndef STARDUST_VERSION_H
#define STARDUST_VERSION_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    struct VersionInfo
    {
        u32 major;
        u32 minor;
        u32 patch;

        [[nodiscard]] String ToString(const bool includePatch = true) const;
        [[nodiscard]] inline operator String() const { return ToString(); }
    };

    constexpr VersionInfo Version{
        .major = 1u,
        .minor = 0u,
        .patch = 7u,
    };

    [[nodiscard]] VersionInfo GetVersionFromString(const String& versionString);
}

#endif