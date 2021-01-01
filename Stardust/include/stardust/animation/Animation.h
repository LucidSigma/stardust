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
		enum class KeyFrameIndex
			: usize
		{
			Sprite = 0u,
			PositionOffset = 1u,
			Rotation = 2u,
			Scale = 3u,
			Shear = 4u,
			Colour = 5u,
		};

		Vector<Pair<KeyFrame, TextureCoordinatePair>> m_spriteFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_positionOffsetFrames{ };
		Vector<Pair<KeyFrame, f32>> m_rotationFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_scaleFrames{ };
		Vector<Pair<KeyFrame, Vec2>> m_shearFrames{ };
		Vector<Pair<KeyFrame, Colour>> m_colourFrames{ };

		Array<KeyFrame, 6u> m_currentFrames{ 0u };
		KeyFrame m_currentKeyFrame = 0u;
		KeyFrame m_maxKeyFrame = 0u;

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
		void StepAttribute(const KeyFrameIndex attributeIndex, const Vector<Pair<KeyFrame, T>>& keyFrames)
		{
			if (keyFrames.size() <= 1u)
			{
				return;
			}

			const usize currentFrameIndex = m_currentFrames[static_cast<usize>(attributeIndex)];
			const KeyFrame currentFrame = keyFrames[currentFrameIndex].first;
			const KeyFrame nextFrame = currentFrame == keyFrames.size() - 1 ? 0u : keyFrames[currentFrameIndex + 1u].first;

			if (m_currentKeyFrame == nextFrame)
			{
				++m_currentFrames[currentFrameIndex];
			}
		}
	};
}

#endif