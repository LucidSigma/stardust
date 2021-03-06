#include "stardust/config/Config.h"

#include <algorithm>

#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/locale/Locale.h"

namespace stardust
{
    [[nodiscard]] Status Config::Initialise(const StringView& preferenceDirectory, const StringView& defaultConfigFilepath)
    {
        const String configDirectory = String(preferenceDirectory) + "config";

        if (!filesystem::DoesFileExist(configDirectory))
        {
            if (filesystem::CreateDirectory(configDirectory) != Status::Success)
            {
                return Status::Fail;
            }
        }

        const String configFilepath = String(configDirectory) + "/config.json";
        bool doesConfigFileExist = filesystem::DoesFileExist(configFilepath);

        if (!doesConfigFileExist)
        {
            Log::EngineWarn("config.json not found; copying from default.");

            const auto defaultConfigData = vfs::ReadFileData(defaultConfigFilepath);

            if (defaultConfigData.empty())
            {
                return Status::Fail;
            }

            String defaultConfigString(
                reinterpret_cast<const char*>(defaultConfigData.data()),
                reinterpret_cast<const char*>(defaultConfigData.data() + defaultConfigData.size())
            );

        #ifdef WIN32
            defaultConfigString.erase(std::remove(std::begin(defaultConfigString), std::end(defaultConfigString), '\r'), std::cend(defaultConfigString));
        #endif

            if (filesystem::WriteToFile(configFilepath, defaultConfigString) != Status::Success)
            {
                return Status::Fail;
            }
        }

        m_filepath = configFilepath;

        if (filesystem::ReadJSON(configFilepath, m_data) == Status::Fail)
        {
            return Status::Fail;
        }

        if (!doesConfigFileExist)
        {
            String preferredLocale = "en_gb";
            const auto& preferredLocales = Locale::GetSystemPreferredLocales();

            for (const auto& locale : preferredLocales)
            {
                if (vfs::IsDirectory("locales/" + locale))
                {
                    preferredLocale = locale;
                    Log::EngineWarn("Locale {} loaded from system preferences.", preferredLocale);

                    break;
                }
            }

            m_data["locale"] = preferredLocale;
            
            return Save();
        }

        return Status::Success;
    }

    [[nodiscard]] Status Config::Save() const
    {
        return filesystem::WriteToFile(m_filepath, m_data);
    }
}