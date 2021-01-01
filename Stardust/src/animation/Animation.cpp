#include "stardust/animation/Animation.h"

#include <string>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	Animation::Animation(const StringView& filepath)
	{
		Initialise(filepath);
	}

	Animation::Animation(const StringView& filepath, const TextureAtlas& textureAtlas)
	{
		Initialise(filepath, textureAtlas);
	}

	void Animation::Initialise(const StringView& filepath)
	{
		LoadFromFile(filepath, nullptr);
	}

	void Animation::Initialise(const StringView& filepath, const TextureAtlas& textureAtlas)
	{
		LoadFromFile(filepath, &textureAtlas);
	}

	void Animation::LoadFromFile(const StringView& filepath, const ObserverPtr<const TextureAtlas>& textureAtlas)
	{
		const Vector<ubyte> animationData = vfs::ReadFileData(filepath);

		if (animationData.empty())
		{
			return;
		}

		const nlohmann::json animationJSON = nlohmann::json::parse(
			reinterpret_cast<const u8*>(animationData.data()),
			reinterpret_cast<const u8*>(animationData.data()) + animationData.size(),
			nullptr,
			false
		);

		if (animationJSON.is_discarded())
		{
			return;
		}
		
		try
		{
			LoadAttributes(animationJSON, textureAtlas);
		}
		catch (const nlohmann::json::exception& error)
		{
			Log::EngineError("Animation file {} is invalid: {}.", filepath, error.what());

			return;
		}

		AddDefaultKeyFrames();
		m_didLoadSuccessfully = true;
	}

	void Animation::LoadAttributes(const nlohmann::json& data, const ObserverPtr<const TextureAtlas>& textureAtlas)
	{
		for (const auto& [frameNumber, frameData] : data["frames"].items())
		{
			const KeyFrame currentKeyFrame = std::stoi(frameNumber);

			for (const auto& [attribute, value] : frameData.items())
			{
				if (attribute == "sprite" && textureAtlas != nullptr)
				{
					m_spriteFrames.insert({ currentKeyFrame, textureAtlas->GetSubtexture(value) });
				}
				else if (attribute == "position-offset")
				{
					m_positionOffsetFrames.insert({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "rotation")
				{
					m_rotationFrames.insert({ currentKeyFrame, value });
				}
				else if (attribute == "scale")
				{
					m_scaleFrames.insert({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "shear")
				{
					m_shearFrames.insert({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "colour")
				{
					m_colourFrames.insert({
						currentKeyFrame,
						CreateColour(
							static_cast<u8>(value["r"]),
							static_cast<u8>(value["g"]),
							static_cast<u8>(value["b"]),
							static_cast<u8>(value["a"])
						),
					});
				}
			}
		}
	}

	void Animation::AddDefaultKeyFrames()
	{
		if (m_spriteFrames.empty())
		{
			m_spriteFrames.insert({
				0,
				TextureCoordinatePair{
					Vec2{ 0.0f, 0.0f },
					Vec2{ 1.0f, 1.0f },
				},
			});
		}

		if (m_positionOffsetFrames.empty())
		{
			m_positionOffsetFrames.insert({ 0, Vec2{ 0.0f, 0.0f } });
		}

		if (m_rotationFrames.empty())
		{
			m_rotationFrames.insert({ 0, 0.0f });
		}

		if (m_scaleFrames.empty())
		{
			m_scaleFrames.insert({ 0, Vec2{ 0.0f, 0.0f } });
		}

		if (m_shearFrames.empty())
		{
			m_shearFrames.insert({ 0, Vec2{ 0.0f, 0.0f } });
		}

		if (m_colourFrames.empty())
		{
			m_colourFrames.insert({ 0, colours::White });
		}
	}
}