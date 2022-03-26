#include "stardust/locale/Locale.h"

#include <utility>

#include <SDL2/SDL.h>

#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/utility/string/String.h"

namespace stardust
{
    auto Locale::SetLocaleChangeEventID(const u32 eventID) noexcept -> void
    {
        s_localeChangeEventID = eventID;
    }

    auto Locale::Initialise(const StringView baseLocaleDirectory) -> void
    {
        m_baseLocaleDirectory = baseLocaleDirectory;
    }
    
    [[nodiscard]] auto Locale::SetLocale(const String& localeName, const bool triggerEvent) -> Status
    {
        const String virtualLocaleDirectory = m_baseLocaleDirectory + "/" + localeName;

        if (!vfs::IsDirectory(virtualLocaleDirectory))
        {
            return Status::Fail;
        }

        const auto localeFiles = vfs::GetAllFiles(virtualLocaleDirectory);

        if (localeFiles.empty())
        {
            return Status::Fail;
        }

        JSON localeAccumulator{ };

        for (const auto& localeFile : localeFiles)
        {
            if (filesystem::GetExtension(localeFile) == ".json")
            {
                const auto subLocale = LoadLocaleFile(virtualLocaleDirectory + "/" + localeFile);

                if (!subLocale.has_value())
                {
                    return Status::Fail;
                }

                localeAccumulator.merge_patch(subLocale.value());

                if (localeAccumulator.is_discarded())
                {
                    return Status::Fail;
                }
            }
        }

        m_currentLocaleEntries = std::move(localeAccumulator);
        m_currentLocaleName = localeName;

        if (triggerEvent)
        {
            PushLocaleChangeEvent();
        }

        return Status::Success;
    }

    [[nodiscard]] auto Locale::Get(const StringView localeEntryName, const HashMap<String, String>& replacementMap) const -> String
    {
        String localeString = Get(localeEntryName);
        
        for (const auto& [oldValue, newValue] : replacementMap)
        {
            localeString = string::ReplaceAll(localeString, "${" + oldValue + "}", newValue);
        }

        return localeString;
    }

    [[nodiscard]] auto Locale::Get(const List<String>& localeEntryNames) const -> String
    {
        JSON currentEntry = m_currentLocaleEntries;

        for (const auto& entryName : localeEntryNames)
        {
            currentEntry = currentEntry[entryName];
        }

        return currentEntry;
    }

    [[nodiscard]] auto Locale::Get(const List<String>& localeEntryNames, const HashMap<String, String>& replacementMap) const -> String
    {
        JSON currentEntry = m_currentLocaleEntries;

        for (const auto& entryName : localeEntryNames)
        {
            currentEntry = currentEntry[entryName];
        }

        String localeString = currentEntry;

        for (const auto& [oldValue, newValue] : replacementMap)
        {
            localeString = string::ReplaceAll(localeString, "${" + oldValue + "}", newValue);
        }

        return localeString;
    }

    [[nodiscard]] auto Locale::LoadLocaleFile(const String& filepath) const -> Optional<JSON>
    {
        auto localeReadResult = vfs::ReadJSON(filepath);

        if (localeReadResult.is_err())
        {
            return None;
        }

        return std::move(localeReadResult).unwrap();
    }

    auto Locale::PushLocaleChangeEvent() -> void
    {
        if (s_localeChangeEventID != std::numeric_limits<u32>::max()) [[likely]]
        {
            SDL_Event localeChangeEvent{ };
            localeChangeEvent.type = s_localeChangeEventID;
            localeChangeEvent.user.code = ChangeEventCode;
            localeChangeEvent.user.data1 = m_currentLocaleName.data();
            localeChangeEvent.user.data2 = nullptr;

            SDL_PushEvent(&localeChangeEvent);
        }
    }
}
