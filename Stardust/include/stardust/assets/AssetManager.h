#pragma once
#ifndef STARDUST_ASSET_MANAGER_H
#define STARDUST_ASSET_MANAGER_H

#include <type_traits>
#include <utility>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
    template <typename T>
    class AssetManager
    {
    private:
        HashMap<String, UniquePtr<T>> m_assets;

    public:
        AssetManager() = default;
        ~AssetManager() noexcept = default;

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        void Add(const String& name, Args&&... args)
        {
            m_assets[name] = std::make_unique<T>(std::forward<Args>(args)...);
        }

        inline void Remove(const String& name) { m_assets.erase(name); }
        inline void Clear() { m_assets.clear(); }

        [[nodiscard]] inline const T& Get(const String& name) const { return *m_assets.at(name); }
        [[nodiscard]] inline const T& operator [](const String& name) const { return Get(name); }

        [[nodiscard]] inline T& Get(const String& name) { return *m_assets.at(name); }
        [[nodiscard]] inline T& operator [](const String& name) { return Get(name); }

        [[nodiscard]] inline decltype(m_assets)& GetAssets() { return m_assets; }
        [[nodiscard]] inline const decltype(m_assets)& GetAssets() const { return m_assets; }
    };
}

#endif