#include "stardust/tilemap/Tilemap.h"

#include <algorithm>

#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/Components.h"

namespace stardust
{
	Tilemap::Layer::Layer(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	void Tilemap::Layer::Initialise(const CreateInfo& createInfo)
	{
		m_ownerTilemap = createInfo.owner;

		m_id = createInfo.id;
		m_name = createInfo.name;

		m_size = createInfo.size;
		SetOpacity(createInfo.opacity);

		m_tileData = createInfo.tiles;
		m_isVisible = createInfo.isVisible;
	}

	void Tilemap::Layer::Render(Renderer& renderer, const Camera2D& camera) const
	{
		if (!m_isVisible)
		{
			return;
		}

		const auto& tilemapPosition = m_ownerTilemap->GetPosition();
		const auto& tilemapTileSize = m_ownerTilemap->GetTileSize();

		const f32 longestScaledDimension = camera.GetAspectRatio() >= 1.0f
			? camera.GetHalfSize() / glm::abs(camera.GetZoom())
			: camera.GetHalfSize() / camera.GetAspectRatio() / glm::abs(camera.GetZoom());

		const f32 zoomFactor = std::max(1.0f, 1.0f / glm::abs(camera.GetZoom()));
		const f32 cameraRotationCosine = glm::abs(glm::cos(glm::radians(camera.GetRotation())));

		const f32 tilesWidth = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
		const f32 tilesHeight = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
		const f32 leftmostPossibleTile = ((camera.GetPosition().x - tilemapPosition.x) - (tilesWidth / 2.0f)) / tilemapTileSize.x;
		const f32 bottommostPossibleTile = ((tilemapPosition.y - camera.GetPosition().y) + (tilesHeight / 2.0f)) / tilemapTileSize.y;

		components::Transform tileTransform(tilemapPosition, 0.0f, NullOpt, tilemapTileSize);

		const i32 topY = static_cast<f32>(glm::floor(std::max(bottommostPossibleTile - tilesHeight, 0.0f)));
		const i32 bottomY = static_cast<f32>(glm::ceil(std::min(bottommostPossibleTile + 1.0f, static_cast<f32>(m_size.y))));

		const i32 leftX = static_cast<i32>(glm::floor(std::max(leftmostPossibleTile, 0.0f)));
		const i32 rightX = static_cast<i32>(glm::ceil(std::min(leftmostPossibleTile + tilesWidth + 1.0f, static_cast<f32>(m_size.x))));

		for (i32 y = topY; y < bottomY; ++y)
		{
			for (i32 x = leftX; x < rightX; ++x)
			{
				const Tile tile = GetTile(static_cast<u32>(x), static_cast<u32>(y));

				if (tile != s_EmptyTile)
				{
					tileTransform.position = tilemapPosition + Vec2{ x, -y } * tilemapTileSize;

					renderer.DrawWorldRect(
						tileTransform,
						components::Sprite(*m_ownerTilemap->m_tileTextureLookup.at(tile), m_ownerTilemap->m_tiles.at(tile)),
						camera
					);
				}
			}
		}
	}

	void Tilemap::Layer::SetOpacity(const f32 opacity) noexcept
	{
		m_opacity = std::clamp(opacity, 0.0f, 1.0f);
	}

	[[nodiscard]] Tile Tilemap::Layer::GetTile(const u32 x, const u32 y) const
	{
		return m_tileData[static_cast<usize>(m_size.x) * y + x];
	}

	void Tilemap::Layer::SetTile(const u32 x, const u32 y, const Tile tile)
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
			const Vec2 tilePixelSize{ tilemapData["tilewidth"], tilemapData["tileheight"] };

			for (const auto& layer : tilemapData["layers"])
			{
				if (layer.contains("objects"))
				{
					ParseColliders(layer["objects"], tilePixelSize, filepath);
				}
				else
				{
					m_layers.emplace_back(Layer::CreateInfo{
						.owner = this,
						.id = layer["id"],
						.name = layer["name"],
						.size = Vec2{ layer["width"], layer["height"] },
						.opacity = layer["opacity"],
						.tiles = layer["data"],
						.isVisible = layer["visible"],
					});
				}
			}
		}
		catch (const nlohmann::json::exception& error)
		{
			Log::EngineError("Failed to load tilemap at {}. Error: {}.", filepath, error.what());

			return;
		}

		std::ranges::sort(m_layers);
		m_isValid = true;
	}

	void Tilemap::Render(Renderer& renderer, const Camera2D& camera) const
	{
		for (const auto& layer : m_layers)
		{
			layer.Render(renderer, camera);
		}
	}

	void Tilemap::AddTiles(const TextureAtlas& textureAtlas)
	{
		const usize originalSize = m_tiles.size();

		for (const auto& [name, textureCoordinates] : textureAtlas.GetSubtextures())
		{
			const Tile tileID = textureAtlas.GetSubtextureID(name) + originalSize + 1u;

			m_tiles.insert({
				tileID,
				textureCoordinates,
			});

			m_tileTextureLookup.insert({
				tileID,
				&textureAtlas.GetTexture(),
			});

		#ifndef NDEBUG
			if (m_tileNameLookup.contains(name))
			{
				Log::EngineWarn("Tilemap contains two tiles with the name {}. Consider renaming one in the texture atlas file.", name);
			}
		#endif

			m_tileNameLookup.insert({
				name,
				tileID,
			});
		}
	}

	[[nodiscard]] Optional<Tile> Tilemap::GetTileID(const String& name) const
	{
		const auto tileIDLocation = m_tileNameLookup.find(name);

		if (tileIDLocation != std::cend(m_tileNameLookup))
		{
			return tileIDLocation->second;
		}
		else
		{
			return NullOpt;
		}
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

	void Tilemap::ParseColliders(const nlohmann::json& objects, const Vec2& tilePixelSize, const StringView& filepath)
	{
		for (const auto& object : objects)
		{
			const f32 x = (object["x"] / tilePixelSize.x);
			const f32 y = (object["y"] / tilePixelSize.y);

			if (object.contains("polygon"))
			{
				if (object["polygon"].size() > b2_maxPolygonVertices)
				{
					Log::EngineWarn("Polygon object in tilemap {} has too many vertices; skipping.", filepath);

					continue;
				}

				Vector<physics::Point> points{ };
				points.reserve(object["polygon"].size());

				for (const auto& vertex : object["polygon"])
				{
					const f32 xOffset = vertex["x"] / tilePixelSize.x;
					const f32 yOffset = vertex["y"] / tilePixelSize.y;

					points.push_back(physics::Point{ x + xOffset, -y - yOffset });
				}

				physics::Polygon polygon{ };
				polygon.Set(points.data(), static_cast<i32>(points.size()));

				m_colliders.push_back(polygon);
			}
			else
			{
				const f32 halfWidth = object["width"] / tilePixelSize.x / 2.0f;
				const f32 halfHeight = object["height"] / tilePixelSize.y / 2.0f;

				physics::Polygon rectangle{ };
				rectangle.SetAsBox(halfWidth, halfHeight, physics::Point{ x + halfWidth - 0.5f, -y - halfHeight + 0.5f }, 0.0f);

				m_colliders.push_back(rectangle);
			}
		}
	}
}