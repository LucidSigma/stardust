#include "stardust/tilemap/Tilemap.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include "stardust/graphics/Graphics.h"

namespace stardust
{
    Tilemap::Tilemap(const List<Tile>& tiles, const u32 width, const Vector2 tileSize)
    {
        Initialise(tiles, width, tileSize);
    }

    Tilemap::Tilemap(const List<List<Tile>>& tiles, const Vector2 tileSize)
    {
        Initialise(tiles, tileSize);
    }

    auto Tilemap::Initialise(const List<Tile>& tiles, const u32 width, const Vector2 tileSize) -> void
    {
        m_tiles = tiles;

        m_size = UVector2{ width, m_tiles.size() / width };
        m_tileSize = tileSize;
    }

    auto Tilemap::Initialise(const List<List<Tile>>& tiles, const Vector2 tileSize) -> void
    {
        m_tiles.reserve(tiles.size() * tiles.front().size());

        for (const auto& row : tiles)
        {
            for (const auto tile : row)
            {
                m_tiles.push_back(tile);
            }
        }

        m_size = UVector2{ tiles.front().size(), tiles.size() };
        m_tileSize = tileSize;
    }

    [[nodiscard]] auto Tilemap::GetOnScreenTiles(const Vector2 translation, const Camera2D& camera) -> const List<Pair<components::Transform, components::Sprite>>
    {
        const auto [leftX, rightX, bottomY, topY] = GetOnScreenBoundaries(translation, camera);
        List<Pair< components::Transform, components::Sprite>> onScreenTiles{ };

        for (i32 y = topY; y < bottomY; ++y)
        {
            for (i32 x = leftX; x < rightX; ++x)
            {
                const Tile tile = GetTile(UVector2{ static_cast<u32>(x), static_cast<u32>(y) });

                if (tile == EmptyTile)
                {
                    continue;
                }

                const components::Transform currentTileTransform{
                    .translation = translation + (Vector2{ x, -y } * m_tileSize),
                    .scale = m_tileSize,
                    .reflection = graphics::Reflection::None,
                    .rotation = 0.0f,
                    .pivot = None,
                    .shear = None,
                };

                const components::Sprite currentTileSprite{
                    .texture = m_tileset->Get(tile).texture,
                    .subTextureArea = m_tileset->Get(tile).subTextureArea,
                    .colourMod = m_colourMod,
                };

                onScreenTiles.emplace_back(currentTileTransform, currentTileSprite);
            }
        }

        return onScreenTiles;
    }

    [[nodiscard]] auto Tilemap::IterateOnScreenTiles(const Vector2 translation, const Camera2D& camera) -> Generator<const Pair<components::Transform, components::Sprite>>
    {
        const auto [leftX, rightX, bottomY, topY] = GetOnScreenBoundaries(translation, camera);

        for (i32 y = topY; y < bottomY; ++y)
        {
            for (i32 x = leftX; x < rightX; ++x)
            {
                const Tile tile = GetTile(UVector2{ static_cast<u32>(x), static_cast<u32>(y) });

                if (tile == EmptyTile)
                {
                    continue;
                }

                co_yield {
                    components::Transform{
                        .translation = translation + (Vector2{ x, -y } * m_tileSize),
                        .scale = m_tileSize,
                        .reflection = graphics::Reflection::None,
                        .rotation = 0.0f,
                        .pivot = None,
                        .shear = None,
                    },
                    components::Sprite{
                        .texture = m_tileset->Get(tile).texture,
                        .subTextureArea = m_tileset->Get(tile).subTextureArea,
                        .colourMod = m_colourMod,
                    },
                };
            }
        }
    }

    [[nodiscard]] auto Tilemap::GetTile(const UVector2 coordinates) const -> Tile
    {
        return m_tiles[static_cast<usize>(coordinates.y * m_size.y) + static_cast<usize>(coordinates.x)];
    }
    
    auto Tilemap::SetTile(const UVector2 coordinates, const Tile tile) -> void
    {
        m_tiles[static_cast<usize>(coordinates.y * m_size.y) + static_cast<usize>(coordinates.x)] = tile;
    }

    auto Tilemap::EraseTile(const UVector2 coordinates) -> void
    {
        SetTile(coordinates, EmptyTile);
    }

    auto Tilemap::HasTile(const UVector2 coordinates) const -> bool
    {
        return GetTile(coordinates) != EmptyTile;
    }

    auto Tilemap::ContainsTile(const Tile tile) -> bool
    {
        if (std::ranges::find(m_tiles, tile) != std::cend(m_tiles))
        {
            return true;
        }

        return false;
    }

    auto Tilemap::FillTiles(const UVector2 topLeft, const UVector2 size, const Tile tile) -> void
    {
        for (usize y = static_cast<usize>(topLeft.y); y <= static_cast<usize>(topLeft.y) + static_cast<usize>(size.y); ++y)
        {
            for (usize x = static_cast<usize>(topLeft.x); x <= static_cast<usize>(topLeft.x) + static_cast<usize>(size.x); ++x)
            {
                SetTile(UVector2{ x, y }, tile);
            }
        }
    }

    auto Tilemap::EraseTiles(const UVector2 topLeft, const UVector2 size) -> void
    {
        FillTiles(topLeft, size, EmptyTile);
    }

    auto Tilemap::FloodFill(const UVector2 origin, const Tile tile) -> void
    {
        const Tile tileToReplace = GetTile(origin);

        HashSet<UVector2> visitedCoordinates{ };
        Queue<UVector2> coordinatesToFill{ };
        coordinatesToFill.push(origin);

        while (!coordinatesToFill.empty())
        {
            const UVector2 currentCoordinate = coordinatesToFill.front();
            coordinatesToFill.pop();

            visitedCoordinates.insert(currentCoordinate);

            if (GetTile(currentCoordinate) == tileToReplace)
            {
                SetTile(currentCoordinate, tile);

                if (currentCoordinate.y > 0u && !visitedCoordinates.contains(UVector2{ currentCoordinate.x, currentCoordinate.y - 1u }))
                {
                    coordinatesToFill.push(UVector2{ currentCoordinate.x, currentCoordinate.y - 1u });
                }

                if (currentCoordinate.y < m_size.y - 1u && !visitedCoordinates.contains(UVector2{ currentCoordinate.x, currentCoordinate.y + 1u }))
                {
                    coordinatesToFill.push(UVector2{ currentCoordinate.x, currentCoordinate.y + 1u });
                }

                if (currentCoordinate.x > 0u && !visitedCoordinates.contains(UVector2{ currentCoordinate.x - 1u, currentCoordinate.y }))
                {
                    coordinatesToFill.push(UVector2{ currentCoordinate.x - 1u, currentCoordinate.y });
                }

                if (currentCoordinate.x < m_size.x - 1u && !visitedCoordinates.contains(UVector2{ currentCoordinate.x + 1u, currentCoordinate.y }))
                {
                    coordinatesToFill.push(UVector2{ currentCoordinate.x + 1u, currentCoordinate.y });
                }
            }
        }
    }

    auto Tilemap::ClearAllTiles() -> void
    {
        FillTiles(UVector2Zero, m_size, EmptyTile);
    }

    [[nodiscard]] auto Tilemap::IsAnyTileInArea(const UVector2 topLeft, const UVector2 size) const -> bool
    {
        for (usize y = static_cast<usize>(topLeft.y); y <= static_cast<usize>(topLeft.y) + static_cast<usize>(size.y); ++y)
        {
            for (usize x = static_cast<usize>(topLeft.x); x <= static_cast<usize>(topLeft.x) + static_cast<usize>(size.x); ++x)
            {
                if (GetTile(UVector2{ x, y }) != EmptyTile)
                {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]] auto Tilemap::AreAllTilesFilled(const UVector2 topLeft, const UVector2 size) const -> bool
    {
        for (usize y = static_cast<usize>(topLeft.y); y <= static_cast<usize>(topLeft.y) + static_cast<usize>(size.y); ++y)
        {
            for (usize x = static_cast<usize>(topLeft.x); x <= static_cast<usize>(topLeft.x) + static_cast<usize>(size.x); ++x)
            {
                if (GetTile(UVector2{ x, y }) == EmptyTile)
                {
                    return false;
                }
            }
        }

        return true;
    }

    [[nodiscard]] auto Tilemap::IsAreaFilledWithTile(const UVector2 topLeft, const UVector2 size, const Tile tile) const -> bool
    {
        for (usize y = static_cast<usize>(topLeft.y); y <= static_cast<usize>(topLeft.y) + static_cast<usize>(size.y); ++y)
        {
            for (usize x = static_cast<usize>(topLeft.x); x <= static_cast<usize>(topLeft.x) + static_cast<usize>(size.x); ++x)
            {
                if (GetTile(UVector2{ x, y }) != tile)
                {
                    return false;
                }
            }
        }

        return true;
    }

    [[nodiscard]] auto Tilemap::DoesAreaContainTile(const UVector2 topLeft, const UVector2 size, const Tile tile) const -> bool
    {
        for (usize y = static_cast<usize>(topLeft.y); y <= static_cast<usize>(topLeft.y) + static_cast<usize>(size.y); ++y)
        {
            for (usize x = static_cast<usize>(topLeft.x); x <= static_cast<usize>(topLeft.x) + static_cast<usize>(size.x); ++x)
            {
                if (GetTile(UVector2{ x, y }) == tile)
                {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]] auto Tilemap::IsAreaEmpty(const UVector2 topLeft, const UVector2 size) const -> bool
    {
        return IsAreaFilledWithTile(topLeft, size, EmptyTile);
    }

    auto Tilemap::Resize(const UVector2 newSize, const Tile fillerTile) -> void
    {
        const List<Tile> oldTiles = std::move(m_tiles);
        m_tiles.clear();
        m_tiles.reserve(static_cast<usize>(newSize.x) * static_cast<usize>(newSize.y));

        for (auto iterator = std::cbegin(oldTiles); iterator != std::cend(oldTiles); iterator += m_size.x)
        {
            List<Tile> currentRow(iterator, iterator + m_size.x);
            currentRow.resize(newSize.x, fillerTile);

            m_tiles.insert(
                std::cend(m_tiles),
                std::make_move_iterator(std::begin(currentRow)),
                std::make_move_iterator(std::end(currentRow))
            );
        }

        m_size = newSize;
    }

    [[nodiscard]] auto Tilemap::GetOnScreenBoundaries(const Vector2 translation, const Camera2D& camera) const -> OnScreenBoundary
    {
        const f32 longestScaledDimension = camera.GetAspectRatio() >= 1.0f
            ? camera.GetHalfSize() / glm::abs(camera.GetZoom())
            : camera.GetHalfSize() / camera.GetAspectRatio() / glm::abs(camera.GetZoom());

        const f32 zoomFactor = std::max(1.0f, 1.0f / glm::abs(camera.GetZoom()));
        const f32 cameraRotationCosine = glm::abs(glm::cos(glm::radians(camera.GetRotation())));

        const f32 tilesWidth = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
        const f32 tilesHeight = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
        const f32 leftmostPossibleTile = ((camera.GetPosition().x - translation.x) - (tilesWidth / 2.0f)) / m_tileSize.x;
        const f32 bottommostPossibleTile = ((translation.y - camera.GetPosition().y) + (tilesHeight / 2.0f)) / m_tileSize.y;

        const i32 topY = static_cast<i32>(glm::floor(std::max(bottommostPossibleTile - tilesHeight, 0.0f)));
        const i32 bottomY = static_cast<i32>(glm::ceil(std::min(bottommostPossibleTile + 1.0f, static_cast<f32>(m_size.y))));

        const i32 leftX = static_cast<i32>(glm::floor(std::max(leftmostPossibleTile, 0.0f)));
        const i32 rightX = static_cast<i32>(glm::ceil(std::min(leftmostPossibleTile + tilesWidth + 1.0f, static_cast<f32>(m_size.x))));

        return OnScreenBoundary{
            .leftX = leftX,
            .rightX = rightX,
            .bottomY = bottomY,
            .topY = topY,
        };
    }
}
