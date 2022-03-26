#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"

#include <utility>

#include <nlohmann/json.hpp>

#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"

namespace stardust
{
    namespace graphics
    {
        TextureAtlas::TextureAtlas(const Texture& texture, const StringView jsonFilepath)
        {
            Initialise(texture, jsonFilepath);
        }
        
        TextureAtlas::TextureAtlas(const Texture& texture, const List<SubTextureData>& subTextures)
        {
            Initialise(texture, subTextures);
        }
        
        auto TextureAtlas::Initialise(const Texture& texture, const StringView jsonFilepath) -> void
        {
            auto textureAtlasJSONResult = vfs::ReadJSON(jsonFilepath);

            if (textureAtlasJSONResult.is_err())
            {
                return;
            }

            const JSON textureAtlasJSON = std::move(textureAtlasJSONResult).unwrap();
            List<SubTextureData> subTextures{ };

            try
            {
                if (!textureAtlasJSON["sub-textures"].is_array())
                {
                    return;
                }

                subTextures.reserve(textureAtlasJSON["sub-textures"].size());

                for (const auto& subTexture : textureAtlasJSON["sub-textures"])
                {
                    const u32 x = subTexture["area"]["x"];
                    const u32 y = subTexture["area"]["y"];
                    const u32 width = subTexture["area"]["width"];
                    const u32 height = subTexture["area"]["height"];

                    subTextures.push_back(
                        SubTextureData{
                            .id = subTexture["id"],
                            .name = subTexture["name"],
                            .topLeftCoordinates = UVector2{ x, y },
                            .size = UVector2{ width, height },
                        }
                    );
                }
            }
            catch (const nlohmann::json::exception& error)
            {
                Log::EngineError("Failed to load texture atlas at {}. Error: {}.", jsonFilepath, error.what());

                return;
            }

            Initialise(texture, subTextures);
        }
        
        auto TextureAtlas::Initialise(const Texture& texture, const List<SubTextureData>& subTextures) -> void
        {
            m_texture = &texture;

            for (const auto& subTexture : subTextures)
            {
                AddSubTexture(subTexture);
            }

            m_isValid = true;
        }

        auto TextureAtlas::AddSubTexture(const SubTextureData& subTextureData) -> void
        {
            const f32 textureWidth = static_cast<f32>(m_texture->GetSize().x);
            const f32 textureHeight = static_cast<f32>(m_texture->GetSize().y);

            const f32 x = static_cast<f32>(subTextureData.topLeftCoordinates.x);
            const f32 y = static_cast<f32>(subTextureData.topLeftCoordinates.y);
            const f32 width = static_cast<f32>(subTextureData.size.x);
            const f32 height = static_cast<f32>(subTextureData.size.y);

            TextureCoordinatePair normalisedCoordinates{
                .lowerLeft = Vector2{
                    x / textureWidth,
                    (textureHeight - (y + height)) / textureHeight,
                },
                .upperRight = Vector2{
                    (x + width) / textureWidth,
                    (textureHeight - y) / textureHeight,
                },
            };

            m_subTextureCoordinates[subTextureData.name] = normalisedCoordinates;
            m_subTextureIDs[subTextureData.name] = subTextureData.id;
        }

        [[nodiscard]] auto TextureAtlas::GetSubTextureSize(const String& subTextureName) const noexcept -> UVector2
        {
            return m_texture->GetSizeFromCoordinates(GetSubTexture(subTextureName));
        }

        [[nodiscard]] auto TextureAtlas::GetSubTextureWidth(const String& subTextureName) const noexcept -> u32
        {
            return m_texture->GetWidthFromCoordinates(GetSubTexture(subTextureName));
        }

        [[nodiscard]] auto TextureAtlas::GetSubTextureHeight(const String& subTextureName) const noexcept -> u32
        {
            return m_texture->GetHeightFromCoordinates(GetSubTexture(subTextureName));
        }
    }
}
