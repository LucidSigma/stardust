#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"

#include <utility>

#include <nlohmann/json.hpp>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/Filesystem.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	TextureAtlas::TextureAtlas(const StringView& filepath)
	{
		Initialise(filepath);
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
		: m_texture(std::move(other.m_texture)), m_subtextures(other.m_subtextures), m_subtextureIDs(other.m_subtextureIDs), m_isValid(other.m_isValid)
	{
		other.m_texture = Texture();
		other.m_subtextures = { };
		other.m_subtextureIDs = { };
		other.m_isValid = false;
	}

	TextureAtlas& TextureAtlas::operator =(TextureAtlas&& other) noexcept
	{
		m_texture = std::exchange(other.m_texture, Texture());
		m_subtextures = std::exchange(other.m_subtextures, { });
		m_subtextureIDs = std::exchange(other.m_subtextureIDs, { });
		m_isValid = std::exchange(other.m_isValid, false);

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

		try
		{
			const f32 textureWidth = static_cast<f32>(m_texture.GetSize().x);
			const f32 textureHeight = static_cast<f32>(m_texture.GetSize().y);

			for (const auto& subtexture : textureAtlasJSON["subtextures"])
			{
				const f32 x = subtexture["rect"]["x"];
				const f32 y = subtexture["rect"]["y"];
				const f32 width = subtexture["rect"]["width"];
				const f32 height = subtexture["rect"]["height"];

				const TextureCoordinatePair normalisedCoordinates{
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
				m_subtextureIDs[subtexture["name"]] = subtexture["id"];
			}
		}
		catch (const nlohmann::json::exception& error)
		{
			Log::EngineError("Failed to load texture atlas at {}. Error: {}.", filepath, error.what());

			return;
		}

		m_isValid = true;
	}

	void TextureAtlas::Destroy() noexcept
	{
		if (m_isValid)
		{
			m_texture.Destroy();
			m_subtextures.clear();
			m_subtextureIDs.clear();

			m_isValid = false;
		}
	}
}