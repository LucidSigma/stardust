#include "stardust/animation/Animation.h"

#include <algorithm>
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

	void Animation::Step()
	{
		++m_currentKeyFrame;

		if (m_currentKeyFrame % m_maxKeyFrame == 0u) [[unlikely]]
		{
			m_currentKeyFrame = 0u;

			for (auto& frame : m_currentFrames)
			{
				frame = 0u;
			}
		}

		StepAttribute(KeyFrameIndex::Sprite, m_spriteFrames);
		StepAttribute(KeyFrameIndex::PositionOffset, m_positionOffsetFrames);
		StepAttribute(KeyFrameIndex::Rotation, m_rotationFrames);
		StepAttribute(KeyFrameIndex::Scale, m_scaleFrames);
		StepAttribute(KeyFrameIndex::Shear, m_shearFrames);
		StepAttribute(KeyFrameIndex::Colour, m_colourFrames);
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
			m_maxKeyFrame = std::max(m_maxKeyFrame, currentKeyFrame);

			for (const auto& [attribute, value] : frameData.items())
			{
				if (attribute == "sprite" && textureAtlas != nullptr)
				{
					m_spriteFrames.push_back({ currentKeyFrame, textureAtlas->GetSubtexture(value) });
				}
				else if (attribute == "position-offset")
				{
					m_positionOffsetFrames.push_back({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "rotation")
				{
					m_rotationFrames.push_back({ currentKeyFrame, value });
				}
				else if (attribute == "scale")
				{
					m_scaleFrames.push_back({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "shear")
				{
					m_shearFrames.push_back({
						currentKeyFrame,
						Vec2{ value["x"], value["y"] },
					});
				}
				else if (attribute == "colour")
				{
					m_colourFrames.push_back({
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

		constexpr auto SortingPredicate = [](const auto& lhs, const auto& rhs) -> bool
		{
			return lhs.first < rhs.first;
		};

		std::sort(std::begin(m_spriteFrames), std::end(m_spriteFrames), SortingPredicate);
		std::sort(std::begin(m_positionOffsetFrames), std::end(m_positionOffsetFrames), SortingPredicate);
		std::sort(std::begin(m_rotationFrames), std::end(m_rotationFrames), SortingPredicate);
		std::sort(std::begin(m_scaleFrames), std::end(m_scaleFrames), SortingPredicate);
		std::sort(std::begin(m_shearFrames), std::end(m_shearFrames), SortingPredicate);
		std::sort(std::begin(m_colourFrames), std::end(m_colourFrames), SortingPredicate);
	}

	void Animation::AddDefaultKeyFrames()
	{
		if (m_spriteFrames.empty())
		{
			m_spriteFrames.push_back({
				0u,
				TextureCoordinatePair{
					Vec2{ 0.0f, 0.0f },
					Vec2{ 1.0f, 1.0f },
				},
			});
		}

		if (m_positionOffsetFrames.empty())
		{
			m_positionOffsetFrames.push_back({ 0u, Vec2{ 0.0f, 0.0f } });
		}

		if (m_rotationFrames.empty())
		{
			m_rotationFrames.push_back({ 0u, 0.0f });
		}

		if (m_scaleFrames.empty())
		{
			m_scaleFrames.push_back({ 0u, Vec2{ 0.0f, 0.0f } });
		}

		if (m_shearFrames.empty())
		{
			m_shearFrames.push_back({ 0u, Vec2{ 0.0f, 0.0f } });
		}

		if (m_colourFrames.empty())
		{
			m_colourFrames.push_back({ 0u, colours::White });
		}
	}
}