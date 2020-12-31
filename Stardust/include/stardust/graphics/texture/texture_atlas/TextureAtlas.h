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

		HashMap<String, CoordinatePair> m_subtextures{ };

	public:
		TextureAtlas() = default;
		TextureAtlas(const StringView& filepath);

		TextureAtlas(TextureAtlas&& other) noexcept;
		TextureAtlas& operator =(TextureAtlas&& other) noexcept;

		~TextureAtlas() noexcept;

		void Initialise(const StringView& filepath);
		void Destroy() noexcept;

		inline bool IsTextureValid() const noexcept { return m_texture.IsValid(); }

		inline const CoordinatePair& GetSubtexture(const String& name) const { return m_subtextures.at(name); }
		inline const CoordinatePair& operator [](const String& name) const { return GetSubtexture(name); }

		inline Texture& GetTexture() noexcept { return m_texture; }
		inline const Texture& GetTexture() const noexcept { return m_texture; }
		inline const decltype(m_subtextures)& GetSubtextures() const noexcept { return m_subtextures; }

		inline const usize GetSubtextureCount() const noexcept { return m_subtextures.size(); }
	};
}

#endif