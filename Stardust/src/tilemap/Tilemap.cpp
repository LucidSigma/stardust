#include "stardust/tilemap/Tilemap.h"

#include <algorithm>

#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/debug/logging/Log.h"

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
		const nlohmann::json tilemapData = ReadTilemapFile(filepath);

		if (tilemapData.is_discarded() || tilemapData.is_null())
		{
			return;
		}

		try
		{
			if (tilemapData["infinite"])
			{
				Log::EngineWarn("Tilemap {} is infinite - which is not supported.");

				return;
			}

			if (tilemapData["orientation"] != "orthogonal")
			{
				Log::EngineWarn("Tilemap {} is not orthogonal - which is not supported.");

				return;
			}

			m_size = UVec2{ tilemapData["width"], tilemapData["height"] };

			for (const auto& layer : tilemapData["layers"])
			{
				m_layers.emplace_back(Layer::CreateInfo{
					.id = layer["id"],
					.name = layer["name"],
					.size = Vec2{ layer["width"], layer["height"] },
					.opacity = layer["opacity"],
					.tiles = layer["data"],
				});
			}

			std::ranges::sort(m_layers);
		}
		catch (const nlohmann::json::exception& error)
		{
			Log::EngineError("Failed to load tilemap at {}. Error: {}.", filepath, error.what());

			return;
		}

		m_isValid = true;
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

	[[nodiscard]] nlohmann::json Tilemap::ReadTilemapFile(const StringView& filepath) const
	{
		const Vector<ubyte> tilemapData = vfs::ReadFileData(filepath);

		if (tilemapData.empty())
		{
			return nlohmann::json{ };
		}

		const nlohmann::json tilemapJSON = nlohmann::json::parse(
			reinterpret_cast<const unsigned char*>(tilemapData.data()),
			reinterpret_cast<const unsigned char*>(tilemapData.data()) + tilemapData.size(),
			nullptr,
			false
		);

		return tilemapJSON;
	}
}