#pragma once
#ifndef STARDUST_TEXTURE_ATLAS_H
#define STARDUST_TEXTURE_ATLAS_H

#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    namespace graphics
    {
        class TextureAtlas final
        {
        public:
            using SubTextureID = u32;

            struct SubTextureData final
            {
                SubTextureID id;
                String name;

                UVector2 topLeftCoordinates;
                UVector2 size;
            };

        private:
            ObserverPointer<const Texture> m_texture = nullptr;

            HashMap<String, TextureCoordinatePair> m_subTextureCoordinates{ };
            HashMap<String, SubTextureID> m_subTextureIDs{ };

            bool m_isValid = false;

        public:
            TextureAtlas() = default;
            TextureAtlas(const Texture& texture, const StringView jsonFilepath);
            TextureAtlas(const Texture& texture, const List<SubTextureData>& subTextures);

            auto Initialise(const Texture& texture, const StringView jsonFilepath) -> void;
            auto Initialise(const Texture& texture, const List<SubTextureData>& subTextures) -> void;

            auto AddSubTexture(const SubTextureData& subTextureData) -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_isValid && m_texture->IsValid(); }

            [[nodiscard]] inline auto GetSubTexture(const String& name) const -> const TextureCoordinatePair& { return m_subTextureCoordinates.at(name); }
            [[nodiscard]] inline auto GetSubTextureID(const String& name) const -> SubTextureID { return m_subTextureIDs.at(name); }
            [[nodiscard]] inline auto operator [](const String& name) const -> const TextureCoordinatePair& { return GetSubTexture(name); }

            [[nodiscard]] inline auto GetTexture() const noexcept -> const Texture& { return *m_texture; }
            [[nodiscard]] inline auto GetAllSubTextures() const noexcept -> const decltype(m_subTextureCoordinates)& { return m_subTextureCoordinates; }
            [[nodiscard]] inline auto GetSubTextureIDs() const noexcept -> const decltype(m_subTextureIDs)& { return m_subTextureIDs; }

            [[nodiscard]] inline auto GetSubTextureCount() const noexcept -> u32 { return static_cast<u32>(m_subTextureIDs.size()); }

            [[nodiscard]] auto GetSubTextureSize(const String& subTextureName) const noexcept -> UVector2;
            [[nodiscard]] auto GetSubTextureWidth(const String& subTextureName) const noexcept -> u32;
            [[nodiscard]] auto GetSubTextureHeight(const String& subTextureName) const noexcept -> u32;
        };
    }
}

#endif
