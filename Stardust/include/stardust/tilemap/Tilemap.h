#pragma once
#ifndef STARDUST_TILEMAP_H
#define STARDUST_TILEMAP_H

#include <compare>

#include <nlohmann/json.hpp>

#include "stardust//camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"

namespace stardust
{
	using Tile = u32;

	class Tilemap
	{
	public:
		class Layer
		{
		public:
			struct CreateInfo
			{
				ObserverPtr<const Tilemap> owner;

				u32 id;
				StringView name;

				UVec2 size;
				f32 opacity = 1.0f;

				Vector<Tile> tiles;
				bool isVisible;
			};

		private:
			ObserverPtr<const Tilemap> m_ownerTilemap = nullptr;

			u32 m_id = 0u;
			String m_name;

			UVec2 m_size{ 0u, 0u };
			f32 m_opacity = 0.0f;

			Vector<Tile> m_tileData{ };
			bool m_isVisible = true;

		public:
			Layer() = default;
			Layer(const CreateInfo& createInfo);
			~Layer() noexcept = default;

			void Initialise(const CreateInfo& createInfo);

			void Render(Renderer& renderer, const Camera2D& camera) const;

			inline u32 GetID() const noexcept { return m_id; }
			inline const String& GetName() const noexcept { return m_name; }

			inline const UVec2& GetSize() const noexcept { return m_size; }

			inline f32 GetOpacity() const noexcept { return m_opacity; }
			void SetOpacity(const f32 opacity) noexcept;

			[[nodiscard]] Tile GetTile(const u32 x, const u32 y) const;
			void SetTile(const u32 x, const u32 y, const Tile tile);

			[[nodiscard]] inline bool IsVisible() const noexcept { return m_isVisible; }
			inline void SetVisibility(const bool isVisible) noexcept { m_isVisible = isVisible; }

			bool operator ==(const Layer&) const noexcept = default;
			bool operator !=(const Layer&) const noexcept = default;

			inline bool operator <(const Layer& other) const noexcept { return m_id < other.m_id; }
			inline bool operator <=(const Layer& other) const noexcept { return m_id <= other.m_id; }
			inline bool operator >(const Layer& other) const noexcept { return m_id > other.m_id; }
			inline bool operator >=(const Layer& other) const noexcept { return m_id >= other.m_id; }

			inline std::strong_ordering operator <=>(const Layer& other) const noexcept { return m_id <=> other.m_id; }
		};

	private:
		static constexpr Tile s_EmptyTile = 0u;

		Vec2 m_position{ 0.0f, 0.0f };

		UVec2 m_size{ 0u, 0u };
		Vec2 m_tileSize{ 1.0f, 1.0f };

		Vector<Layer> m_layers{ };

		HashMap<Tile, TextureCoordinatePair> m_tiles{ };
		HashMap<Tile, ObserverPtr<const Texture>> m_tileTextureLookup{ };
		HashMap<String, Tile> m_tileNameLookup{ };

		bool m_isValid = false;

	public:
		friend class Layer;

		Tilemap() = default;
		Tilemap(const StringView& filepath);
		~Tilemap() noexcept = default;

		void Initialise(const StringView& filepath);

		void Render(Renderer& renderer, const Camera2D& camera) const;

		void AddTiles(const TextureAtlas& textureAtlas);
		[[nodiscard]] Optional<Tile> GetTileID(const String& name) const;

		inline const Vec2& GetPosition() const noexcept { return m_position; }
		inline void SetPosition(const Vec2& position) noexcept { m_position = position; }

		inline const UVec2& GetSize() const noexcept { return m_size; }
		inline void SetSize(const UVec2& size) noexcept { m_size = size; }
		inline const Vec2& GetTileSize() const noexcept { return m_tileSize; }
		inline void SetTileSize(const Vec2& tileSize) noexcept { m_tileSize = tileSize; }

		inline Vector<Layer>& GetLayers() noexcept { return m_layers; }
		inline const Vector<Layer>& GetLayers() const noexcept { return m_layers; }
		[[nodiscard]] ObserverPtr<Layer> GetLayerByID(const u32 layerID) noexcept;
		[[nodiscard]] ObserverPtr<const Layer> GetLayerByID(const u32 layerID) const noexcept;
		[[nodiscard]] ObserverPtr<Layer> GetLayerByName(const String layerName) noexcept;
		[[nodiscard]] ObserverPtr<const Layer> GetLayerByName(const String layerName) const noexcept;

		inline bool IsValid() const noexcept { return m_isValid; }

	private:
		[[nodiscard]] nlohmann::json ReadTilemapFile(const StringView& filepath) const;
	};
}

#endif