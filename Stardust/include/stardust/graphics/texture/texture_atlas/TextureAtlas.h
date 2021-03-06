#pragma once
#ifndef STARDUST_TEXTURE_ATLAS_H
#define STARDUST_TEXTURE_ATLAS_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"

namespace stardust
{
    class TextureAtlas
        : private INoncopyable
    {
    private:
        Texture m_texture;

        HashMap<String, TextureCoordinatePair> m_subtextures{ };
        HashMap<String, u32> m_subtextureIDs{ };

        bool m_isValid = false;

    public:
        TextureAtlas() = default;
        explicit TextureAtlas(const StringView& filepath);

        TextureAtlas(TextureAtlas&& other) noexcept;
        TextureAtlas& operator =(TextureAtlas&& other) noexcept;

        ~TextureAtlas() noexcept;

        void Initialise(const StringView& filepath);
        void Destroy() noexcept;

        [[nodiscard]] inline bool IsValid() const noexcept { return m_isValid && IsTextureValid(); }
        [[nodiscard]] inline bool IsTextureValid() const noexcept { return m_texture.IsValid(); }

        [[nodiscard]] inline const TextureCoordinatePair& GetSubtexture(const String& name) const { return m_subtextures.at(name); }
        [[nodiscard]] inline u32 GetSubtextureID(const String& name) const { return m_subtextureIDs.at(name); }
        [[nodiscard]] inline const TextureCoordinatePair& operator [](const String& name) const { return GetSubtexture(name); }

        [[nodiscard]] inline Texture& GetTexture() noexcept { return m_texture; }
        [[nodiscard]] inline const Texture& GetTexture() const noexcept { return m_texture; }
        [[nodiscard]] inline const decltype(m_subtextures)& GetSubtextures() const noexcept { return m_subtextures; }
        [[nodiscard]] inline const decltype(m_subtextureIDs)& GetSubtextureIDs() const noexcept { return m_subtextureIDs; }

        [[nodiscard]] inline const usize GetSubtextureCount() const noexcept { return m_subtextures.size(); }
    };
}

#endif