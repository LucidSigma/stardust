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
		String m_filepath;
		nlohmann::json m_data;

	public:
		Config() = default;
		~Config() noexcept = default;

		[[nodiscard]] Status Initialise(const String& configFilepath);
		void Save() const;

		inline auto& operator [](const String& configType) { return m_data[configType.data()]; }
	};
}

#endif