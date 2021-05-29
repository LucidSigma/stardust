#include "stardust/locale/Locale.h"

#include <utility>

#include <SDL2/SDL.h>

#include "stardust/filesystem/Filesystem.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
    [[nodiscard]] const Vector<String>& Locale::GetSystemPreferredLocales()
    {
        if (s_systemPreferredLocales.empty())
        {
            Vector<String> preferredLocaleStrings{ };
            SDL_Locale* preferredLocales = SDL_GetPreferredLocales();

            for (const SDL_Locale* currentLocale = preferredLocales; currentLocale != nullptr && currentLocale->language != nullptr; ++currentLocale)
            {
                String currentLocaleString = string::MakeLower(currentLocale->language);

                if (currentLocale->country != nullptr && currentLocale->country != "")
                {
                    currentLocaleString += "_" + string::MakeLower(currentLocale->country);
                }

                preferredLocaleStrings.push_back(currentLocaleString);
            }

            SDL_free(preferredLocales);
            preferredLocales = nullptr;

            s_systemPreferredLocales = preferredLocaleStrings;
        }

        return s_systemPreferredLocales;
    }

    void Locale::Initialise(const StringView& baseLocaleDirectory)
    {
        m_baseLocaleDirectory = baseLocaleDirectory;
    }

    [[nodiscard]] Status Locale::SetLocale(const String& localeName)
    {
        const String localeDirectory = m_baseLocaleDirectory + "/" + localeName;

        if (!vfs::IsDirectory(localeDirectory))
        {
            return Status::Fail;
        }

        const auto localeFiles = vfs::GetAllFilesInDirectory(localeDirectory);

        if (localeFiles.empty())
        {
            return Status::Fail;
        }

        nlohmann::json localeAccumulator{ };

        for (const auto& localeFile : localeFiles)
        {
            if (filesystem::GetFileExtension(localeFile) == ".json")
            {
                const auto localeSubdata = LoadLocaleFile(localeFile);

                if (!localeSubdata.has_value())
                {
                    return Status::Fail;
                }

                localeAccumulator.merge_patch(*localeSubdata);

                if (localeAccumulator.is_discarded())
                {
                    return Status::Fail;
                }
            }
        }

        m_currentLocale = std::move(localeAccumulator);
        m_currentLocaleName = localeName;

        return Status::Success;
    }

    [[nodiscard]] Optional<nlohmann::json> Locale::LoadLocaleFile(const String& filepath) const
    {
        const nlohmann::json locale = vfs::ReadJSON(filepath);

        if (locale.is_discarded())
        {
            return NullOpt;
        }
        else
        {
            return locale;
        }
    }
}