#pragma once
#ifndef STARDUST_LAYER_STACK_H
#define STARDUST_LAYER_STACK_H

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "stardust/scene/layer/Layer.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class LayerStack final
    {
    private:
        List<UniquePointer<Layer>> m_layers{ };
        usize m_layerInsertionIndex = 0u;

    public:
        LayerStack() = default;
        ~LayerStack() noexcept;

        template <std::derived_from<Layer> L, typename... Args>
            requires std::is_constructible_v<L, Args...>
        auto PushLayer(Args&&... args) -> Status
        {
            m_layers.emplace(
                std::cbegin(m_layers) + m_layerInsertionIndex,
                std::make_unique<L>(std::forward<Args>(args)...)
            );

            ++m_layerInsertionIndex;

            return m_layers[m_layerInsertionIndex - 1u]->OnAttach();
        }

        template <std::derived_from<Layer> L, typename... Args>
            requires std::is_constructible_v<L, Args...>
        auto PushOverlay(Args&&... args) -> Status
        {
            m_layers.emplace_back(
                std::make_unique<L>(std::forward<Args>(args)...)
            );

            return m_layers.back()->OnAttach();
        }

        auto PopLayer() -> void;
        auto PopOverlay() -> void;

        auto RemoveAllLayers() noexcept -> void;

        [[nodiscard]] inline auto IsEmpty() const noexcept -> bool { return m_layers.empty(); }

        [[nodiscard]] inline auto GetTotalCount() const noexcept -> u32 { return static_cast<u32>(m_layers.size()); }
        [[nodiscard]] inline auto GetLayerCount() const noexcept -> u32 { return static_cast<u32>(m_layerInsertionIndex); }
        [[nodiscard]] inline auto GetOverlayCount() const noexcept -> u32 { return GetTotalCount() - static_cast<u32>(m_layerInsertionIndex); }

        [[nodiscard]] inline auto GetLayers() noexcept -> List<UniquePointer<Layer>>& { return m_layers; }
        [[nodiscard]] inline auto GetLayers() const noexcept -> const List<UniquePointer<Layer>>& { return m_layers; }

        [[nodiscard]] auto IterateAllLayers() -> Generator<const ObserverPointer<Layer>>;
        [[nodiscard]] auto IterateAllLayers() const -> Generator<const ObserverPointer<const Layer>>;
        [[nodiscard]] auto IterateLayers() -> Generator<const ObserverPointer<Layer>>;
        [[nodiscard]] auto IterateLayers() const -> Generator<const ObserverPointer<const Layer>>;
        [[nodiscard]] auto IterateOverlays() -> Generator<const ObserverPointer<Layer>>;
        [[nodiscard]] auto IterateOverlays() const -> Generator<const ObserverPointer<const Layer>>;

        [[nodiscard]] inline auto begin() noexcept -> List<UniquePointer<Layer>>::iterator { return std::begin(m_layers); }
        [[nodiscard]] inline auto end() noexcept -> List<UniquePointer<Layer>>::iterator { return std::end(m_layers); }
        [[nodiscard]] inline auto rbegin() noexcept -> List<UniquePointer<Layer>>::reverse_iterator { return std::rbegin(m_layers); }
        [[nodiscard]] inline auto rend() noexcept -> List<UniquePointer<Layer>>::reverse_iterator { return std::rend(m_layers); }

        [[nodiscard]] inline auto cbegin() const noexcept -> List<UniquePointer<Layer>>::const_iterator { return std::cbegin(m_layers); }
        [[nodiscard]] inline auto cend() const noexcept -> List<UniquePointer<Layer>>::const_iterator { return std::cend(m_layers); }
        [[nodiscard]] inline auto crbegin() const noexcept -> List<UniquePointer<Layer>>::const_reverse_iterator { return std::crbegin(m_layers); }
        [[nodiscard]] inline auto crend() const noexcept -> List<UniquePointer<Layer>>::const_reverse_iterator { return std::crend(m_layers); }
    };
}

namespace std
{
    template <typename T>
    [[nodiscard]] inline auto begin(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.begin(); }

    template <typename T>
    [[nodiscard]] inline auto end(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.end(); }

    template <typename T>
    [[nodiscard]] inline auto rbegin(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.rbegin(); }

    template <typename T>
    [[nodiscard]] inline auto rend(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.rend(); }

    template <typename T>
    [[nodiscard]] inline auto cbegin(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.cbegin(); }

    template <typename T>
    [[nodiscard]] inline auto cend(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.cend(); }

    template <typename T>
    [[nodiscard]] inline auto crbegin(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.crbegin(); }

    template <typename T>
    [[nodiscard]] inline auto crend(stardust::LayerStack& layerStack) noexcept -> decltype(auto) { return layerStack.crend(); }
}

#endif
