#include "stardust/scene/layer/LayerStack.h"

#include <ranges>

namespace stardust
{
    LayerStack::~LayerStack() noexcept
    {
        RemoveAllLayers();
    }

    auto LayerStack::PopLayer() -> void
    {
        if (m_layerInsertionIndex == 0u) [[unlikely]]
        {
            return;
        }

        m_layers[m_layerInsertionIndex]->OnDetach();

        m_layers.erase(std::cbegin(m_layers) + m_layerInsertionIndex - 1u);
        --m_layerInsertionIndex;
    }

    auto LayerStack::PopOverlay() -> void
    {
        if (m_layerInsertionIndex == m_layers.size()) [[unlikely]]
        {
            return;
        }

        m_layers.back()->OnDetach();
        m_layers.pop_back();
    }

    auto LayerStack::RemoveAllLayers() noexcept -> void
    {
        if (m_layers.empty())
        {
            return;
        }

        for (auto& layer : std::views::reverse(m_layers))
        {
            layer->OnDetach();
        }

        m_layers.clear();
    }

    [[nodiscard]] auto LayerStack::IterateAllLayers() -> Generator<const ObserverPointer<Layer>>
    {
        for (const auto& layer : m_layers)
        {
            co_yield layer.get();
        }
    }

    [[nodiscard]] auto LayerStack::IterateAllLayers() const -> Generator<const ObserverPointer<const Layer>>
    {
        for (const auto& layer : m_layers)
        {
            co_yield layer.get();
        }
    }
    
    [[nodiscard]] auto LayerStack::IterateLayers() -> Generator<const ObserverPointer<Layer>>
    {
        for (usize i = 0u; i < m_layerInsertionIndex; ++i)
        {
            co_yield m_layers[i].get();
        }
    }

    [[nodiscard]] auto LayerStack::IterateLayers() const -> Generator<const ObserverPointer<const Layer>>
    {
        for (usize i = 0u; i < m_layerInsertionIndex; ++i)
        {
            co_yield m_layers[i].get();
        }
    }

    [[nodiscard]] auto LayerStack::IterateOverlays() -> Generator<const ObserverPointer<Layer>>
    {
        for (usize i = m_layerInsertionIndex; i < m_layers.size(); ++i)
        {
            co_yield m_layers[i].get();
        }
    }

    [[nodiscard]] auto LayerStack::IterateOverlays() const -> Generator<const ObserverPointer<const Layer>>
    {
        for (usize i = m_layerInsertionIndex; i < m_layers.size(); ++i)
        {
            co_yield m_layers[i].get();
        }
    }
}
