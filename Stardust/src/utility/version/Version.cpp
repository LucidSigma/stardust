#include "stardust/utility/version/Version.h"

#include <string>

namespace stardust
{
    [[nodiscard]] String VersionInfo::ToString(const bool includePatch) const
    {
        String versionString = std::to_string(major) + "." + std::to_string(minor);
        
        if (includePatch)
        {
            versionString += "." + std::to_string(patch);
        }

        return versionString;
    }

    [[nodiscard]] VersionInfo CreateVersionInfo(const u32 major, const u32 minor, const u32 patch) noexcept
    {
        return VersionInfo{
            .major = major,
            .minor = minor,
            .patch = patch,
        };
    }

    [[nodiscard]] VersionInfo CreateVersionFromString(const String& versionString)
    {
        VersionInfo version{ };
        const auto versionCategories = string::Split(versionString, '.');

        for (usize i = 0u; i < versionCategories.size(); ++i)
        {
            switch (i)
            {
            case 0u:
                version.major = static_cast<u32>(std::stoi(versionCategories[i]));

                break;

            case 1u:
                version.minor = static_cast<u32>(std::stoi(versionCategories[i]));

                break;

            case 2u:
                version.patch = static_cast<u32>(std::stoi(versionCategories[i]));

                break;

            default:
                break;
            }
        }

        if (versionCategories.size() < 2u)
        {
            version.minor = 0u;
        }

        if (versionCategories.size() < 3u)
        {
            version.patch = 0u;
        }

        return version;
    }
}