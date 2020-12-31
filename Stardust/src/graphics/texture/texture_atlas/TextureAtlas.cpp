#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"

#include <utility>

#include <nlohmann/json.hpp>

#include "stardust/filesystem/Filesystem.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	TextureAtlas::TextureAtlas(const StringView& filepath)
	{
		Initialise(filepath);
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
		: m_texture(std::move(other.m_texture)), m_subtextures(other.m_subtextures)
	{
		other.m_texture = Texture();
		other.m_subtextures = { };
	}

	TextureAtlas& TextureAtlas::operator =(TextureAtlas&& other) noexcept
	{
		m_texture = std::exchange(other.m_texture, Texture());
		m_subtextures = std::exchange(other.m_subtextures, { });

		return *this;
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		Destroy();
	}

	void TextureAtlas::Initialise(const StringView& filepath)
	{
		const Vector<ubyte> textureAtlasData = vfs::ReadFileData(filepath);

		if (textureAtlasData.empty())
		{
			return;
		}

		const nlohmann::json textureAtlasJSON = nlohmann::json::parse(
			reinterpret_cast<const u8*>(textureAtlasData.data()),
			reinterpret_cast<const u8*>(textureAtlasData.data()) + textureAtlasData.size(),
			nullptr,
			false
		);

		if (textureAtlasJSON.is_discarded())
		{
			return;
		}

		const String textureFilename = filesystem::GetParentFilepath(filepath) + "/" + String(textureAtlasJSON["texture_file"]);
		m_texture.Initialise(textureFilename);

		if (!m_texture.IsValid())
		{
			return;
		}

		const f32 textureWidth = static_cast<f32>(m_texture.GetSize().x);
		const f32 textureHeight = static_cast<f32>(m_texture.GetSize().y);

		for (const auto& subtexture : textureAtlasJSON["subtextures"])
		{
			const f32 x = subtexture["rect"]["x"];
			const f32 y = subtexture["rect"]["y"];
			const f32 width = subtexture["rect"]["width"];
			const f32 height = subtexture["rect"]["height"];

			const Pair<Vec2, Vec2> normalisedCoordinates{
				Vec2{
					x / textureWidth,
					(textureHeight - (y + height)) / textureHeight,
				},
				Vec2{
					(x + width) / textureWidth,
					(textureHeight - y) / textureHeight,
				},
			};

			m_subtextures[subtexture["name"]] = normalisedCoordinates;
		}
	}

	void TextureAtlas::Destroy() noexcept
	{
		if (m_texture.IsValid())
		{
			m_texture.Destroy();
			m_subtextures.clear();
		}
	}
}