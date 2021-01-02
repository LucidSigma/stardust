#include "stardust/animation/Animation.h"

#include <algorithm>
#include <string>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/math/Math.h"

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
		m_currentKeyFrame %= (m_maxKeyFrame + 1u);

		StepSprite();
		StepPositionOffset();
		StepRotation();
		StepScale();
		StepShear();
		StepColour();
	}

	[[nodiscard]] const TextureCoordinatePair& Animation::GetSprite() const
	{
		return m_spriteFrames[m_currentSpriteIndex].second;
	}

	[[nodiscard]] Vec2 Animation::GetPositionOffset() const
	{
		const Vec2& currentPositionOffset = m_positionOffsetFrames[m_currentPositionOffsetIndex].second;
		const Vec2& nextPositionOffset = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].second;

		const KeyFrame currentFrame = m_positionOffsetFrames[m_currentPositionOffsetIndex].first;
		KeyFrame nextFrame = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].first;

		if (nextFrame < currentFrame)
		{
			nextFrame += m_maxKeyFrame + 1u;
		}

		const f32 frameDifference = static_cast<f32>(nextFrame) - static_cast<f32>(currentFrame);
		const f32 shiftedFrame = static_cast<f32>(m_currentKeyFrame) - static_cast<f32>(currentFrame);

		const f32 percentage = shiftedFrame / frameDifference;

		return glm::lerp(currentPositionOffset, nextPositionOffset, percentage);
	}
	
	//[[nodiscard]] f32 Animation::GetRotation() const
	//{
	//
	//}
	//
	//[[nodiscard]] Vec2 Animation::GetScale() const
	//{
	//
	//}
	//
	//[[nodiscard]] Vec2 Animation::GetShear() const
	//{
	//
	//}
	
	[[nodiscard]] Colour Animation::GetColour() const
	{
		const Colour& currentColour = m_colourFrames[m_currentColourIndex].second;
		const Colour& nextColour = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].second;

		const KeyFrame currentFrame = m_colourFrames[m_currentColourIndex].first;
		KeyFrame nextFrame = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].first;

		if (nextFrame < currentFrame)
		{
			nextFrame += m_maxKeyFrame + 1u;
		}

		const f32 frameDifference = static_cast<f32>(nextFrame) - static_cast<f32>(currentFrame);
		const f32 shiftedFrame = static_cast<f32>(m_currentKeyFrame) - static_cast<f32>(currentFrame);

		const f32 percentage = shiftedFrame / frameDifference;

		return Vec4ToColour(glm::lerp(ColourToVec4(currentColour), ColourToVec4(nextColour), percentage));
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
		m_maxKeyFrame = data["length"];

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

	void Animation::StepSprite()
	{
		if (m_spriteFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentSpriteIndex == m_spriteFrames.size() - 1u
			? 0u
			: m_spriteFrames[m_currentSpriteIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentSpriteIndex;
			m_currentSpriteIndex %= m_spriteFrames.size();
		}
	}

	void Animation::StepPositionOffset()
	{
		if (m_positionOffsetFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentPositionOffsetIndex == m_positionOffsetFrames.size() - 1u
			? 0u
			: m_positionOffsetFrames[m_currentPositionOffsetIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentPositionOffsetIndex;
			m_currentPositionOffsetIndex %= m_positionOffsetFrames.size();
		}
	}

	void Animation::StepRotation()
	{
		if (m_rotationFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentRotationIndex == m_rotationFrames.size() - 1u
			? 0u
			: m_rotationFrames[m_currentRotationIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentRotationIndex;
			m_currentRotationIndex %= m_rotationFrames.size();
		}
	}

	void Animation::StepScale()
	{
		if (m_scaleFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentScaleIndex == m_scaleFrames.size() - 1u
			? 0u
			: m_scaleFrames[m_currentScaleIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentScaleIndex;
			m_currentScaleIndex %= m_scaleFrames.size();
		}
	}

	void Animation::StepShear()
	{
		if (m_shearFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentShearIndex == m_shearFrames.size() - 1u
			? 0u
			: m_shearFrames[m_currentShearIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentShearIndex;
			m_currentShearIndex %= m_shearFrames.size();
		}
	}

	void Animation::StepColour()
	{
		if (m_colourFrames.size() <= 1u)
		{
			return;
		}

		const KeyFrame nextFrame = m_currentColourIndex == m_colourFrames.size() - 1u
			? 0u
			: m_colourFrames[m_currentColourIndex + 1u].first;

		if (m_currentKeyFrame == nextFrame)
		{
			++m_currentColourIndex;
			m_currentColourIndex %= m_colourFrames.size();
		}
	}
}