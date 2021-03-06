#pragma once
#ifndef STARDUST_LOCALE_H
#define STARDUST_LOCALE_H

#include <nlohmann/json.hpp>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    class Locale
    {
    private:
        inline static Vector<String> s_systemPreferredLocales{ };

        String m_baseLocaleDirectory;

        nlohmann::json m_currentLocale;
        String m_currentLocaleName;

    public:
        [[nodiscard]] static const Vector<String>& GetSystemPreferredLocales();

        Locale() = default;
        ~Locale() noexcept = default;

        void Initialise(const StringView& baseLocaleDirectory);
        [[nodiscard]] Status SetLocale(const String& localeName);

        [[nodiscard]] inline const String& GetCurrentLocaleName() const noexcept { return m_currentLocaleName; }
        [[nodiscard]] inline const nlohmann::json& operator [](const StringView& localeString) const { return m_currentLocale[localeString.data()]; }

    private:
        [[nodiscard]] Optional<nlohmann::json> LoadLocaleFile(const String& filepath) const;
    };
}

#endif