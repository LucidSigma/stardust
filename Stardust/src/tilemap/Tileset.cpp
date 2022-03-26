#include "stardust/tilemap/Tileset.h"

namespace stardust
{
    auto Tileset::Set(const Tile tile, const graphics::Texture& texture, const graphics::TextureCoordinatePair& textureCoordinates) -> void
    {
        m_tileTextures[tile] = TileTextureInfo{
            .texture = &texture,
            .subTextureArea = textureCoordinates,
        };
    }

    auto Tileset::Set(const Tile tile, const graphics::TextureAtlas& textureAtlas, const String& subTextureName) -> void
    {
        m_tileTextures[tile] = TileTextureInfo{
            .texture = &textureAtlas.GetTexture(),
            .subTextureArea = textureAtlas.GetSubTexture(subTextureName),
        };
    }
}
