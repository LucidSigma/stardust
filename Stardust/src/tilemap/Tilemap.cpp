#include "stardust/tilemap/Tilemap.h"

#include <algorithm>

namespace stardust
{
	Tilemap::Layer::Layer(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	void Tilemap::Layer::Initialise(const CreateInfo& createInfo)
	{
		m_id = createInfo.id;
		m_name = createInfo.name;

		m_size = createInfo.size;
		m_offset = createInfo.offset;
		SetOpacity(createInfo.opacity);

		m_tileData = createInfo.tiles;
	}

	void Tilemap::Layer::SetOpacity(const f32 opacity) noexcept
	{
		m_opacity = std::clamp(opacity, 0.0f, 1.0f);
	}

	[[nodiscard]] u32 Tilemap::Layer::GetTile(const u32 x, const u32 y) const
	{
		return m_tileData[static_cast<usize>(m_size.x) * y + x];
	}

	void Tilemap::Layer::SetTile(const u32 x, const u32 y, const u32 tile)
	{
		m_tileData[static_cast<usize>(m_size.x) * y + x] = tile;
	}

	Tilemap::Tilemap(const StringView& filepath)
	{
		Initialise(filepath);
	}

	void Tilemap::Initialise(const StringView& filepath)
	{
		
	}

	[[nodiscard]] ObserverPtr<Tilemap::Layer> Tilemap::GetLayerByID(const u32 layerID) noexcept
	{
		for (auto& layer : m_layers)
		{
			if (layer.GetID() == layerID)
			{
				return &layer;
			}
		}

		return nullptr;
	}

	[[nodiscard]] ObserverPtr<const Tilemap::Layer> Tilemap::GetLayerByID(const u32 layerID) const noexcept
	{
		for (const auto& layer : m_layers)
		{
			if (layer.GetID() == layerID)
			{
				return &layer;
			}
		}

		return nullptr;
	}

	[[nodiscard]] ObserverPtr<Tilemap::Layer> Tilemap::GetLayerByName(const String layerName) noexcept
	{
		for (auto& layer : m_layers)
		{
			if (layer.GetName() == layerName)
			{
				return &layer;
			}
		}

		return nullptr;
	}

	[[nodiscard]] ObserverPtr<const Tilemap::Layer> Tilemap::GetLayerByName(const String layerName) const noexcept
	{
		for (const auto& layer : m_layers)
		{
			if (layer.GetName() == layerName)
			{
				return &layer;
			}
		}

		return nullptr;
	}
}