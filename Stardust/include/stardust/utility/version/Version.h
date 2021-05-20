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

        [[nodiscard]] bool operator ==(const VersionInfo&) const noexcept = default;
        [[nodiscard]] bool operator !=(const VersionInfo&) const noexcept = default;
    };

    constexpr VersionInfo Version{
        .major = 1u,
        .minor = 0u,
        .patch = 7u,
    };

    [[nodiscard]] extern VersionInfo CreateVersionInfo(const u32 major, const u32 minor = 0u, const u32 patch = 0u) noexcept;
    [[nodiscard]] extern VersionInfo CreateVersionFromString(const String& versionString);
}

#endif