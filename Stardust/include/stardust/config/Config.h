#pragma once
#ifndef STARDUST_CONFIG_H
#define STARDUST_CONFIG_H

#include <nlohmann/json.hpp>

#include "stardust/data/Containers.h"
#include "stardust/utility/status/Status.h"

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

        [[nodiscard]] Status Initialise(const StringView& preferenceDirectory, const StringView& defaultConfigFilepath);
        [[nodiscard]] Status Save() const;

        [[nodiscard]] inline auto& operator [](const StringView& configType) { return m_data[configType.data()]; }
        [[nodiscard]] inline const auto& operator [](const StringView& configType) const { return m_data[configType.data()]; }
    };
}

#endif