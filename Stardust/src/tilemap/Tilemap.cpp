#include "stardust/tilemap/Tilemap.h"

#include <algorithm>
#include <utility>

#include "stardust/debug/logging/Log.h"
#include "stardust/scene/components/Components.h"

namespace stardust
{
    Tilemap::Tilemap(const Vector<Tile>& tiles, const u32 width, const Vec2& tileSize)
    {
        Initialise(tiles, width, tileSize);
    }

    Tilemap::Tilemap(const Vector<Vector<Tile>>& tiles, const Vec2& tileSize)
    {
        Initialise(tiles, tileSize);
    }

    void Tilemap::Initialise(const Vector<Tile>& tiles, const u32 width, const Vec2& tileSize)
    {
        for (usize y = 0u; y < tiles.size(); y += static_cast<usize>(width))
        {
            Vector<Tile> tileRow(width, EmptyTile);

            for (usize x = 0u; x < width; ++x)
            {
                if (x + y >= tiles.size()) [[unlikely]]
                {
                    break;
                }

                tileRow[x] = tiles[x + y];
            }

            m_tiles.push_back(std::move(tileRow));
        }

        m_size = UVec2{ width, m_tiles.size() };
        m_tileSize = tileSize;
    }

    void Tilemap::Initialise(const Vector<Vector<Tile>>& tiles, const Vec2& tileSize)
    {
        m_tiles = tiles;
        m_size = UVec2{ m_tiles.front().size(), m_tiles.size() };
        m_tileSize = tileSize;
    }

    void Tilemap::AddTileTextures(const TextureAtlas& textureAtlas)
    {
        const usize originalSize = m_tileTextureCoordinates.size();

        for (const auto& [name, textureCoordinates] : textureAtlas.GetSubtextures())
        {
            const Tile tileID = textureAtlas.GetSubtextureID(name) + originalSize + 1u;

            m_tileTextureCoordinates.insert({
                tileID,
                textureCoordinates,
            });

            m_tileTextureLookup.insert({
                tileID,
                &textureAtlas.GetTexture(),
            });

        #ifndef NDEBUG
            if (m_tileNames.contains(name))
            {
                Log::EngineWarn("Tilemap contains two tiles with the name {}. Consider renaming one in the texture atlas file.", name);
            }
        #endif

            m_tileNames.insert({
                name,
                tileID,
            });
        }
    }

    [[nodiscard]] Optional<Tile> Tilemap::GetTileID(const String& name) const
    {
        const auto tileIDLocation = m_tileNames.find(name);

        if (tileIDLocation != std::cend(m_tileNames))
        {
            return tileIDLocation->second;
        }
        else
        {
            return NullOpt;
        }
    }

    void Tilemap::Render(Renderer& renderer, const Camera2D& camera) const
    {
        const f32 longestScaledDimension = camera.GetAspectRatio() >= 1.0f
            ? camera.GetHalfSize() / glm::abs(camera.GetZoom())
            : camera.GetHalfSize() / camera.GetAspectRatio() / glm::abs(camera.GetZoom());

        const f32 zoomFactor = std::max(1.0f, 1.0f / glm::abs(camera.GetZoom()));
        const f32 cameraRotationCosine = glm::abs(glm::cos(glm::radians(camera.GetRotation())));

        const f32 tilesWidth = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
        const f32 tilesHeight = 2.0f * (longestScaledDimension / cameraRotationCosine * zoomFactor);
        const f32 leftmostPossibleTile = ((camera.GetPosition().x - m_position.x) - (tilesWidth / 2.0f)) / m_tileSize.x;
        const f32 bottommostPossibleTile = ((m_position.y - camera.GetPosition().y) + (tilesHeight / 2.0f)) / m_tileSize.y;

        components::Transform tileTransform(m_position, 0.0f, NullOpt, m_tileSize);

        const i32 topY = static_cast<f32>(glm::floor(std::max(bottommostPossibleTile - tilesHeight, 0.0f)));
        const i32 bottomY = static_cast<f32>(glm::ceil(std::min(bottommostPossibleTile + 1.0f, static_cast<f32>(m_size.y))));

        const i32 leftX = static_cast<i32>(glm::floor(std::max(leftmostPossibleTile, 0.0f)));
        const i32 rightX = static_cast<i32>(glm::ceil(std::min(leftmostPossibleTile + tilesWidth + 1.0f, static_cast<f32>(m_size.x))));

        for (i32 y = topY; y < bottomY; ++y)
        {
            for (i32 x = leftX; x < rightX; ++x)
            {
                const Tile tile = GetTile(UVec2{ static_cast<u32>(x), static_cast<u32>(y) });

                if (tile != EmptyTile)
                {
                    tileTransform.position = m_position + Vec2{ x, -y } * m_tileSize;

                    renderer.DrawWorldRect(
                        tileTransform,
                        components::Sprite(m_tileTextureLookup.at(tile), m_tileTextureCoordinates.at(tile), m_colourMod),
                        camera
                    );
                }
            }
        }
    }

    [[nodiscard]] Tile Tilemap::GetTile(const UVec2& coordinates) const
    {
        return m_tiles[coordinates.y][coordinates.x];
    }

    void Tilemap::SetTile(const UVec2& coordinates, const Tile tile)
    {
        m_tiles[coordinates.y][coordinates.x] = tile;
    }
}