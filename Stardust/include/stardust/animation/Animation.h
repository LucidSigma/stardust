#pragma once
#ifndef STARDUST_ANIMATION_H
#define STARDUST_ANIMATION_H

#include <nlohmann/json.hpp>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Colour.h"

namespace stardust
{
	using KeyFrame = u32;

	class Animation
	{
	private:
		Vector<Pair<KeyFrame, TextureCoordinatePair>> m_spriteFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_positionOffsetFrames{ };
		Vector<Pair<KeyFrame, Quaternion>> m_rotationFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_scaleFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_shearFrames{ };
		Vector<Pair<KeyFrame, Colour>> m_colourFrames{ };

		KeyFrame m_currentKeyFrame = 0u;
		KeyFrame m_maxKeyFrame = 0u;

		usize m_currentSpriteIndex = 0u;
		usize m_currentPositionOffsetIndex = 0u;
		usize m_currentRotationIndex = 0u;
		usize m_currentScaleIndex = 0u;
		usize m_currentShearIndex = 0u;
		usize m_currentColourIndex = 0u;

		bool m_didLoadSuccessfully = false;

	public:
		Animation() = default;
		Animation(const StringView& filepath);
		Animation(const StringView& filepath, const TextureAtlas& textureAtlas);
		~Animation() noexcept = default;

		void Initialise(const StringView& filepath);
		void Initialise(const StringView& filepath, const TextureAtlas& textureAtlas);

		void Step();

		[[nodiscard]] const TextureCoordinatePair& GetSprite() const;
		[[nodiscard]] Vec2 GetPositionOffset() const;
		[[nodiscard]] f32 GetRotation() const;
		[[nodiscard]] Vec2 GetScale() const;
		[[nodiscard]] Vec2 GetShear() const;
		[[nodiscard]] Colour GetColour() const;

		inline bool IsValid() const noexcept { return m_didLoadSuccessfully; }

	private:
		void LoadFromFile(const StringView& filepath, const ObserverPtr<const TextureAtlas>& textureAtlas);
		void LoadAttributes(const nlohmann::json& data, const ObserverPtr<const TextureAtlas>& textureAtlas);
		void AddDefaultKeyFrames();

		template <typename T>
		void StepAttribute(const Vector<Pair<KeyFrame, T>> keyFrames, usize& currentIndex)
		{
			if (keyFrames.size() <= 1u)
			{
				return;
			}

			const KeyFrame nextFrame = currentIndex == keyFrames.size() - 1u
				? 0u
				: keyFrames[currentIndex + 1u].first;

			if (m_currentKeyFrame == nextFrame)
			{
				++currentIndex;
				currentIndex %= keyFrames.size();
			}
		}

		[[nodiscard]] f32 GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame) const;
	};
}

#endif