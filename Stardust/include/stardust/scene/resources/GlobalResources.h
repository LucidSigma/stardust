#pragma once
#ifndef STARDUST_GLOBAL_RESOURCES_H
#define STARDUST_GLOBAL_RESOURCES_H

#include <any>

#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class GlobalResources final
    {
    private:
        HashMap<String, Any> m_data{ };

    public:
        template <typename T>
        [[nodiscard]] inline auto Get(const StringView key) const -> T
        {
            return std::any_cast<T>(m_data.at(key.data()));
        }

        template <typename T>
        [[nodiscard]] inline auto GetReference(const StringView key) -> T&
        {
            return std::any_cast<T&>(m_data[key.data()]);
        }

        template <typename T>
        [[nodiscard]] inline auto GetConstReference(const StringView key) const -> const T&
        {
            return std::any_cast<const T&>(m_data.at(key.data()));
        }

        template <typename T>
        [[nodiscard]] inline auto GetPointer(const StringView key) -> ObserverPointer<T>
        {
            return std::any_cast<T>(&m_data[key.data()]);
        }

        template <typename T>
        [[nodiscard]] inline auto GetConstPointer(const StringView key) const -> ObserverPointer<const T>
        {
            return std::any_cast<const T>(&m_data.at(key.data()));
        }

        template <typename T>
        inline auto Set(const StringView key, const T& data) -> void
        {
            m_data[key.data()] = data;
        }

        [[nodiscard]] auto HasKey(const StringView key) -> bool;
        
        auto Remove(const StringView key) -> void;
        auto Clear() -> void;
    };
}

#endif