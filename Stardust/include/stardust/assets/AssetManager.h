#pragma once
#ifndef STARDUST_ASSET_MANAGER_H
#define STARDUST_ASSET_MANAGER_H

#include <memory>
#include <type_traits>
#include <utility>

#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    template <typename T>
    class AssetManager final
    {
    private:
        HashMap<String, UniquePointer<T>> m_assets{ };
        Optional<UniquePointer<T>> m_defaultAsset = None;

    public:
        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        inline auto Add(const String& name, Args&&... args) -> void
        {
            m_assets[name] = std::make_unique<T>(std::forward<Args>(args)...);
        }

        [[nodiscard]] inline auto Has(const String& name) -> bool { return m_assets.contains(name); }

        inline auto Remove(const String& name) -> void { m_assets.erase(name); }
        inline auto Clear() -> void { m_assets.clear(); }

        [[nodiscard]] inline auto GetAssetCount() const noexcept -> u32 { return static_cast<u32>(m_assets.size()); }

        [[nodiscard]] auto Get(const String& name) -> T&
        {
            const auto assetLocation = m_assets.find(name);

            if (assetLocation == std::cend(m_assets))
            {
                return *m_defaultAsset.value();
            }

            return *assetLocation->second;
        }

        [[nodiscard]] auto Get(const String& name) const -> const T&
        {
            const auto assetLocation = m_assets.find(name);

            if (assetLocation == std::cend(m_assets))
            {
                return *m_defaultAsset.value();
            }

            return *assetLocation->second;
        }

        [[nodiscard]] inline auto operator [](const String& name) -> T& { return Get(name); }
        [[nodiscard]] inline auto operator [](const String& name) const -> const T& { return Get(name); }

        [[nodiscard]] inline auto HasDefault() const noexcept -> bool { return m_defaultAsset.has_value(); }
        [[nodiscard]] inline auto GetDefault() -> Optional<UniquePointer<T>>& { return m_defaultAsset; }
        [[nodiscard]] inline auto GetDefault() const -> const Optional<UniquePointer<T>>& { return m_defaultAsset; }

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        inline auto SetDefault(Args&&... args) -> void
        {
            m_defaultAsset = std::make_unique<T>(std::forward<Args>(args)...);
        }

        [[nodiscard]] inline auto GetAssets() -> decltype(m_assets)& { return m_assets; }
        [[nodiscard]] inline auto GetAssets() const -> const decltype(m_assets)& { return m_assets; }

        [[nodiscard]] inline auto begin() noexcept -> HashMap<String, UniquePointer<T>>::iterator { return std::begin(m_assets); }
        [[nodiscard]] inline auto end() noexcept -> HashMap<String, UniquePointer<T>>::iterator { return std::end(m_assets); }

        [[nodiscard]] inline auto cbegin() const noexcept -> HashMap<String, UniquePointer<T>>::const_iterator { return std::cbegin(m_assets); }
        [[nodiscard]] inline auto cend() const noexcept -> HashMap<String, UniquePointer<T>>::const_iterator { return std::cend(m_assets); }

        [[nodiscard]] auto Iterate() const -> Generator<const T&>
        {
            for (const auto& [key, asset] : m_assets)
            {
                co_yield *asset;
            }
        }
    };
}

namespace std
{
    template <typename T>
    [[nodiscard]] inline auto begin(stardust::AssetManager<T>& assetManager) noexcept -> decltype(auto) { return assetManager.begin(); }

    template <typename T>
    [[nodiscard]] inline auto end(stardust::AssetManager<T>& assetManager) noexcept -> decltype(auto) { return assetManager.end(); }

    template <typename T>
    [[nodiscard]] inline auto cbegin(stardust::AssetManager<T>& assetManager) noexcept -> decltype(auto) { return assetManager.cbegin(); }

    template <typename T>
    [[nodiscard]] inline auto cend(stardust::AssetManager<T>& assetManager) noexcept -> decltype(auto) { return assetManager.cend(); }
}

#endif
