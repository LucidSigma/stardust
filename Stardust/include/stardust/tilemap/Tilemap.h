#pragma once
#ifndef STARDUST_TILEMAP_H
#define STARDUST_TILEMAP_H

#include <functional>

#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/ecs/components/TransformComponent.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/tilemap/Tile.h"
#include "stardust/tilemap/Tileset.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Tilemap final
    {
    private:
        struct OnScreenBoundary final
        {
            i32 leftX;
            i32 rightX;

            i32 bottomY;
            i32 topY;
        };

        List<Tile> m_tiles{ };

        UVector2 m_size = UVector2Zero;
        Vector2 m_tileSize = Vector2One;

        ObserverPointer<const Tileset> m_tileset = nullptr;

        Colour m_colourMod = colours::White;

    public:
        Tilemap() = default;
        Tilemap(const List<Tile>& tiles, const u32 width, const Vector2 tileSize = Vector2One);
        explicit Tilemap(const List<List<Tile>>& tiles, const Vector2 tileSize = Vector2One);
        ~Tilemap() noexcept = default;

        auto Initialise(const List<Tile>& tiles, const u32 width, const Vector2 tileSize = Vector2One) -> void;
        auto Initialise(const List<List<Tile>>& tiles, const Vector2 tileSize = Vector2One) -> void;

        [[nodiscard]] auto GetTileset() const noexcept -> ObserverPointer<const Tileset> { return m_tileset; }
        inline auto SetTileset(const Tileset& tileset) noexcept -> void { m_tileset = &tileset; }

        [[nodiscard]] inline auto GetTiles() const noexcept -> const List<Tile>& { return m_tiles; }
        [[nodiscard]] auto GetOnScreenTiles(const Vector2 translation, const Camera2D& camera) -> const List<Pair<components::Transform, components::Sprite>>;
        [[nodiscard]] auto IterateOnScreenTiles(const Vector2 translation, const Camera2D& camera) -> Generator<const Pair<components::Transform, components::Sprite>>;

        [[nodiscard]] auto GetTile(const UVector2 coordinates) const -> Tile;
        auto SetTile(const UVector2 coordinates, const Tile tile) -> void;
        auto EraseTile(const UVector2 coordinates) -> void;
        [[nodiscard]] auto HasTile(const UVector2 coordinates) const -> bool;

        [[nodiscard]] auto ContainsTile(const Tile tile) -> bool;

        auto FillTiles(const UVector2 topLeft, const UVector2 size, const Tile tile) -> void;
        auto EraseTiles(const UVector2 topLeft, const UVector2 size) -> void;
        auto FloodFill(const UVector2 origin, const Tile tile) -> void;
        auto ClearAllTiles() -> void;

        [[nodiscard]] auto IsAnyTileInArea(const UVector2 topLeft, const UVector2 size) const -> bool;
        [[nodiscard]] auto AreAllTilesFilled(const UVector2 topLeft, const UVector2 size) const -> bool;
        [[nodiscard]] auto IsAreaFilledWithTile(const UVector2 topLeft, const UVector2 size, const Tile tile) const -> bool;
        [[nodiscard]] auto DoesAreaContainTile(const UVector2 topLeft, const UVector2 size, const Tile tile) const -> bool;
        [[nodiscard]] auto IsAreaEmpty(const UVector2 topLeft, const UVector2 size) const -> bool;

        [[nodiscard]] inline auto GetSize() const noexcept -> const UVector2 { return m_size; }
        auto Resize(const UVector2 newSize, const Tile fillerTile = EmptyTile) -> void;

        [[nodiscard]] inline auto GetTileSize() const noexcept -> const Vector2 { return m_tileSize; }
        inline auto SetTileSize(const Vector2 tileSize) noexcept -> void { m_tileSize = tileSize; }

        [[nodiscard]] inline auto GetColourMod() const noexcept -> const Colour& { return m_colourMod; }
        inline auto SetColourMod(const Colour& colourMod) noexcept -> void { m_colourMod = colourMod; }

    private:
        [[nodiscard]] auto GetOnScreenBoundaries(const Vector2 translation, const Camera2D& camera) const ->OnScreenBoundary;
    };
}

#endif
