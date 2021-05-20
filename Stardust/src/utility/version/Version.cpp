#include "stardust/utility/version/Version.h"

#include <string>

namespace stardust
{
    [[nodiscard]] String VersionInfo::GetString() const
    {
        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }

    [[nodiscard]] VersionInfo GetVersionFromString(const String& versionString)
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

        return version;
    }
}