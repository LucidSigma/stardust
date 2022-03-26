#pragma once
#ifndef STARDUST_LOCALE_H
#define STARDUST_LOCALE_H

#include <limits>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class Locale final
    {
    public:
        static constexpr i32 ChangeEventCode = 0;

    private:
        inline static u32 s_localeChangeEventID = std::numeric_limits<u32>::max();

        String m_baseLocaleDirectory;

        JSON m_currentLocaleEntries;
        String m_currentLocaleName;

    public:
        static auto SetLocaleChangeEventID(const u32 eventID) noexcept -> void;

        auto Initialise(const StringView baseLocaleDirectory) -> void;
        [[nodiscard]] auto SetLocale(const String& localeName, const bool triggerEvent = true) -> Status;

        [[nodiscard]] auto GetCurrentLocaleName() const noexcept -> const String& { return m_currentLocaleName; }

        [[nodiscard]] inline auto Get(const StringView localeEntryName) const -> JSON { return m_currentLocaleEntries[localeEntryName.data()]; }
        [[nodiscard]] auto Get(const StringView localeEntryName, const HashMap<String, String>& replacementMap) const -> String;
        [[nodiscard]] auto Get(const List<String>& localeEntryNames) const -> String;
        [[nodiscard]] auto Get(const List<String>& localeEntryNames, const HashMap<String, String>& replacementMap) const -> String;

        [[nodiscard]] inline auto operator [](const StringView localeEntryName) const -> JSON { return Get(localeEntryName); }
        [[nodiscard]] inline auto operator ()(const StringView localeEntryName) const -> JSON { return Get(localeEntryName); }
        [[nodiscard]] inline auto operator ()(const StringView localeEntryName, const HashMap<String, String>& replacementMap) const -> String { return Get(localeEntryName, replacementMap); }
        [[nodiscard]] inline auto operator [](const List<String>& localeEntryNames) const -> String { return Get(localeEntryNames); }
        [[nodiscard]] inline auto operator ()(const List<String>& localeEntryNames) const -> String { return Get(localeEntryNames); }
        [[nodiscard]] inline auto operator ()(const List<String>& localeEntryNames, const HashMap<String, String>& replacementMap) const -> String { return Get(localeEntryNames, replacementMap); }

    private:
        [[nodiscard]] auto LoadLocaleFile(const String& filepath) const -> Optional<JSON>;

        auto PushLocaleChangeEvent() -> void;
    };
}

#endif
