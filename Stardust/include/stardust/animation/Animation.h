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
		HashMap<KeyFrame, TextureCoordinatePair> m_spriteFrames{ };
		HashMap<KeyFrame, Vec2> m_positionOffsetFrames{ };
		HashMap<KeyFrame, f32> m_rotationFrames{ };
		HashMap<KeyFrame, Vec2> m_scaleFrames{ };
		HashMap<KeyFrame, Vec2> m_shearFrames{ };
		HashMap<KeyFrame, Colour> m_colourFrames{ };

		bool m_didLoadSuccessfully = false;

	public:
		Animation() = default;
		Animation(const StringView& filepath);
		Animation(const StringView& filepath, const TextureAtlas& textureAtlas);
		~Animation() noexcept = default;

		void Initialise(const StringView& filepath);
		void Initialise(const StringView& filepath, const TextureAtlas& textureAtlas);

		inline bool IsValid() const noexcept { return m_didLoadSuccessfully; }

	private:
		void LoadFromFile(const StringView& filepath, const ObserverPtr<const TextureAtlas>& textureAtlas);
		void LoadAttributes(const nlohmann::json& data, const ObserverPtr<const TextureAtlas>& textureAtlas);
		void AddDefaultKeyFrames();
	};
}

#endif