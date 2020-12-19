#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>

#include "stardust/data/Containers.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	class Config
	{
	private:
		StringView m_filepath;
		nlohmann::json m_data;

	public:
		Config() = default;
		~Config() noexcept = default;

		[[nodiscard]] Status Initialise(const StringView& configFilepath);
		void Save() const;

		inline auto& operator [](const StringView& configType) { return m_data[configType.data()]; }
		inline const auto& operator [](const StringView& configType) const { return m_data[configType.data()]; }
	};
}

#endif