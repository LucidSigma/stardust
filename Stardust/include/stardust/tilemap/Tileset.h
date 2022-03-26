#pragma once
#ifndef STARDUST_TILESET_H
#define STARDUST_TILESET_H

#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/tilemap/Tile.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class Tileset final
    {
    public:
        struct TileTextureInfo final
        {
            ObserverPointer<const graphics::Texture> texture = nullptr;
            graphics::TextureCoordinatePair subTextureArea{ };
        };

    private:
        HashMap<Tile, TileTextureInfo> m_tileTextures{ };

    public:
        auto Set(const Tile tile, const graphics::Texture& texture, const graphics::TextureCoordinatePair& textureCoordinates = graphics::TextureCoordinatePair{ }) -> void;
        auto Set(const Tile tile, const graphics::TextureAtlas& textureAtlas, const String& subTextureName) -> void;

        [[nodiscard]] inline auto Has(const Tile tile) const noexcept -> bool { return m_tileTextures.contains(tile); }
        
        [[nodiscard]] inline auto Get(const Tile tile) const -> const TileTextureInfo& { return m_tileTextures.at(tile); }
        [[nodiscard]] inline auto operator [](const Tile tile) const -> const TileTextureInfo& { return Get(tile); }
    };
}

#endif
