#pragma once
#ifndef LOCALE_H
#define LOCALE_H

#include <nlohmann/json.hpp>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	class Locale
	{
	private:
		String m_baseLocaleDirectory;

		nlohmann::json m_currentLocale;
		String m_currentLocaleName;

	public:
		Locale() = default;
		~Locale() noexcept = default;

		void Initialise(const String& baseLocaleDirectory);
		[[nodiscard]] Status SetLocale(const String& localeName);

		inline const String& GetCurrentLocaleName() const noexcept { return m_currentLocaleName; }
		inline const nlohmann::json& operator [](const String& localeString) const { return m_currentLocale[localeString.data()]; }

	private:
		[[nodiscard]] Optional<nlohmann::json> LoadLocaleFile(const String& filepath) const;
	};
}

#endif