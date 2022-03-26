#pragma once
#ifndef STARDUST_USER_PREFS_H
#define STARDUST_USER_PREFS_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class UserPrefs final
    {
    private:
        TOML m_preferenceTable{ };
        String m_filepath{ };

    public:
        [[nodiscard]] auto Load(const StringView filepath) -> Status;
        auto Save() -> Status;

        [[nodiscard]] auto HasCategory(const StringView category) const -> bool;
        [[nodiscard]] auto HasKey(const StringView category, const StringView key) const -> bool;
        auto DeleteKey(const StringView category, const StringView key) -> void;

        template <typename T>
        [[nodiscard]] auto Get(const StringView category, const StringView key) const -> Optional<T>
        {
            if (!HasKey(category, key))
            {
                return None;
            }

            return m_preferenceTable[category][key].value<T>();
        }

        template <typename T>
        [[nodiscard]] auto GetOrDefault(const StringView category, const StringView key, const T& defaultValue) -> T
        {
            if (!HasKey(category, key))
            {
                Set(category, key, defaultValue);

                return defaultValue;
            }

            const Optional<T> tentativeValue = m_preferenceTable[category][key].value<T>();

            if (!tentativeValue.has_value())
            {
                Set(category, key, defaultValue);

                return defaultValue;
            }

            return tentativeValue.value();
        }

        template <typename T>
        auto Set(const StringView category, const StringView key, const T& value) -> void
        {
            if (!HasCategory(category))
            {
                m_preferenceTable.insert(category, TOML{ });
            }

            m_preferenceTable[category].as_table()->insert(key, value);
        }

        template <typename T>
        auto SetIfMissing(const StringView category, const StringView key, const T& value) -> void
        {
            if (!HasKey(category, key))
            {
                Set(category, key, value);
            }
        }
    };
}

#endif
