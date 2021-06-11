#pragma once
#ifndef STARDUST_TILEMAP_H
#define STARDUST_TILEMAP_H

#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/math/Math.h"

namespace stardust
{
    using Tile = u32;

    constexpr Tile EmptyTile = 0u;

    class Tilemap
    {
    private:
        Vector<Vector<Tile>> m_tiles{ };
        
        Vec2 m_position = Vec2Zero;
        UVec2 m_size = UVec2Zero;
        Vec2 m_tileSize = Vec2One;

        HashMap<Tile, TextureCoordinatePair> m_tileTextureCoordinates{ };
        HashMap<Tile, ObserverPtr<const Texture>> m_tileTextureLookup{ };
        HashMap<String, Tile> m_tileNames{ };

        Colour m_colourMod = colours::White;

    public:
        Tilemap() = default;
        Tilemap(const Vector<Tile>& tiles, const u32 width, const Vec2& tileSize = Vec2One);
        explicit Tilemap(const Vector<Vector<Tile>>& tiles, const Vec2& tileSize = Vec2One);
        ~Tilemap() noexcept = default;

        void Initialise(const Vector<Tile>& tiles, const u32 width, const Vec2& tileSize = Vec2One);
        void Initialise(const Vector<Vector<Tile>>& tiles, const Vec2& tileSize = Vec2One);

        void AddTileTextures(const TextureAtlas& textureAtlas);
        [[nodiscard]] Optional<Tile> GetTileID(const String& name) const;

        void Render(Renderer& renderer, const Camera2D& camera) const;

        [[nodiscard]] Tile GetTile(const UVec2& coordinates) const;
        void SetTile(const UVec2& coordinates, const Tile tile);
        void EraseTile(const UVec2& coordinates);
        [[nodiscard]] bool HasTile(const UVec2& coordinates) const;

        [[nodiscard]] bool ContainsTile(const Tile tile) const;

        void FillTiles(const UVec2& topLeft, const UVec2& size, const Tile tile);
        void EraseTiles(const UVec2& topLeft, const UVec2& size);
        void FloodFill(const UVec2& origin, const Tile tile);
        void ClearAllTiles();

        [[nodiscard]] bool IsAnyTileInArea(const UVec2& topLeft, const UVec2& size) const;
        [[nodiscard]] bool AreAllTilesFilled(const UVec2& topLeft, const UVec2& size) const;
        [[nodiscard]] bool IsAreaFilledWithTile(const UVec2& topLeft, const UVec2& size, const Tile tile) const;
        [[nodiscard]] bool IsAreaEmpty(const UVec2& topLeft, const UVec2& size) const;

        [[nodiscard]] inline const Vector<Vector<Tile>>& GetTiles() const noexcept { return m_tiles; }

        [[nodiscard]] inline const UVec2& GetSize() const noexcept { return m_size; }
        void Resize(const UVec2& newSize, const Tile fillerTile = EmptyTile);

        [[nodiscard]] inline const Vec2& GetPosition() const noexcept { return m_position; }
        inline void SetPosition(const Vec2& position) noexcept { m_position = position; }
        [[nodiscard]] inline const Vec2& GetTileSize() const noexcept { return m_tileSize; }
        inline void SetTileSize(const Vec2& tileSize) noexcept { m_tileSize = tileSize; }

        [[nodiscard]] inline const Colour& GetColourMod() const noexcept { return m_colourMod; }
        inline void SetColourMod(const Colour& colourMod) noexcept { m_colourMod = colourMod; }
    };
}

#endif