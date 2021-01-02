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

	void Animation::AddEvent(const KeyFrame keyFrame, const Event& event)
	{
		if (!m_eventCallbacks.contains(keyFrame))
		{
			m_eventCallbacks[keyFrame] = { };
		}

		m_eventCallbacks[keyFrame].push_back(event);
	}

	void Animation::Step()
	{
		++m_currentKeyFrame;
		m_currentKeyFrame %= (m_maxKeyFrame + 1u);

		StepAttribute(m_spriteFrames, m_currentSpriteIndex);
		StepAttribute(m_positionOffsetFrames, m_currentPositionOffsetIndex);
		StepAttribute(m_rotationFrames, m_currentRotationIndex);
		StepAttribute(m_scaleFrames, m_currentScaleIndex);
		StepAttribute(m_shearFrames, m_currentShearIndex);
		StepAttribute(m_colourFrames, m_currentColourIndex);

		if (m_eventCallbacks.contains(m_currentKeyFrame))
		{
			for (const auto& event : m_eventCallbacks[m_currentKeyFrame])
			{
				event();
			}
		}
	}

	[[nodiscard]] const TextureCoordinatePair& Animation::GetSprite() const
	{
		return m_spriteFrames[m_currentSpriteIndex].second;
	}

	[[nodiscard]] Vec2 Animation::GetPositionOffset(const f32 frameInterpolation) const
	{
		if (m_positionOffsetFrames.size() == 1u)
		{
			return m_positionOffsetFrames.front().second;
		}

		const Vec2& currentPositionOffset = m_positionOffsetFrames[m_currentPositionOffsetIndex].second;
		const Vec2& nextPositionOffset = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].second;

		const KeyFrame currentFrame = m_positionOffsetFrames[m_currentPositionOffsetIndex].first;
		const KeyFrame nextFrame = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].first;

		const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation);

		return glm::lerp(currentPositionOffset, nextPositionOffset, percentage);
	}
	
	[[nodiscard]] f32 Animation::GetRotation(const f32 frameInterpolation) const
	{
		if (m_rotationFrames.size() == 1u)
		{
			return glm::degrees(glm::roll(m_rotationFrames.front().second));
		}

		const Quaternion& currentRotation = m_rotationFrames[m_currentRotationIndex].second;
		const Quaternion& nextRotation = m_rotationFrames[(m_currentRotationIndex + 1u) % m_rotationFrames.size()].second;

		const KeyFrame currentFrame = m_rotationFrames[m_currentRotationIndex].first;
		const KeyFrame nextFrame = m_rotationFrames[(m_currentRotationIndex + 1u) % m_rotationFrames.size()].first;

		const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation);

		return glm::degrees(glm::roll(glm::slerp(currentRotation, nextRotation, percentage)));
	}
	
	[[nodiscard]] Vec2 Animation::GetScale(const f32 frameInterpolation) const
	{
		if (m_scaleFrames.size() == 1u)
		{
			return m_scaleFrames.front().second;
		}

		const Vec2& currentScale = m_scaleFrames[m_currentScaleIndex].second;
		const Vec2& nextScale = m_scaleFrames[(m_currentScaleIndex + 1u) % m_scaleFrames.size()].second;

		const KeyFrame currentFrame = m_scaleFrames[m_currentScaleIndex].first;
		const KeyFrame nextFrame = m_scaleFrames[(m_currentScaleIndex + 1u) % m_scaleFrames.size()].first;

		const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation);

		return glm::lerp(currentScale, nextScale, percentage);
	}
	
	[[nodiscard]] Vec2 Animation::GetShear(const f32 frameInterpolation) const
	{
		if (m_shearFrames.size() == 1u)
		{
			return m_shearFrames.front().second;
		}

		const Vec2& currentShear = m_shearFrames[m_currentShearIndex].second;
		const Vec2& nextShear = m_shearFrames[(m_currentShearIndex + 1u) % m_shearFrames.size()].second;

		const KeyFrame currentFrame = m_shearFrames[m_currentShearIndex].first;
		const KeyFrame nextFrame = m_shearFrames[(m_currentShearIndex + 1u) % m_shearFrames.size()].first;

		const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation);

		return glm::lerp(currentShear, nextShear, percentage);
	}
	
	[[nodiscard]] Colour Animation::GetColour(const f32 frameInterpolation) const
	{
		if (m_colourFrames.size() == 1u)
		{
			return m_colourFrames.front().second;
		}

		const Colour& currentColour = m_colourFrames[m_currentColourIndex].second;
		const Colour& nextColour = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].second;

		const KeyFrame currentFrame = m_colourFrames[m_currentColourIndex].first;
		const KeyFrame nextFrame = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].first;

		const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation);

		return Vec4ToColour(glm::lerp(ColourToVec4(currentColour), ColourToVec4(nextColour), percentage));
	}

	void Animation::Reset()
	{
		m_currentKeyFrame = 0u;
		
		m_currentSpriteIndex = 0u;
		m_currentPositionOffsetIndex = 0u;
		m_currentRotationIndex = 0u;
		m_currentScaleIndex = 0u;
		m_currentShearIndex = 0u;
		m_currentColourIndex = 0u;
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
					m_rotationFrames.push_back({
						currentKeyFrame,
						glm::angleAxis(glm::radians(static_cast<f32>(value)), Vec3{ 0.0f, 0.0f, 1.0f }),
					});
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
			m_rotationFrames.push_back({ 0u, glm::angleAxis(0.0f, Vec3{ 0.0f, 0.0f, 1.0f }) });
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

	[[nodiscard]] f32 Animation::GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame, const f32 frameInterpolation) const
	{
		if (nextFrame < currentFrame)
		{
			nextFrame += m_maxKeyFrame + 1u;
		}

		const f32 frameDifference = static_cast<f32>(nextFrame) - static_cast<f32>(currentFrame);
		const f32 shiftedCurrentFrame = static_cast<f32>(m_currentKeyFrame) - static_cast<f32>(currentFrame);

		return (shiftedCurrentFrame + frameInterpolation) / frameDifference;
	}
}