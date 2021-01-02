#pragma once
#ifndef STARDUST_ANIMATOR_H
#define STARDUST_ANIMATOR_H

#include "stardust/animation/Animation.h"
#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Texture.h"

namespace stardust
{
	class Animator
	{
	private:
		static constexpr u32 s_DefaultFrameRate = 24u;

		HashMap<String, ObserverPtr<Animation>> m_animations{ };
		ObserverPtr<Animation> m_currentAnimation = nullptr;

		u32 m_fps = 0u;
		f32 m_millisecondsPerFrame = 0.0f;
		f32 m_currentFrameTimeAccumulator = 0.0f;

	public:
		Animator();
		~Animator() noexcept = default;

		void AddAnimation(const String& animationName, Animation& animation);
		void AddAnimations(const Vector<Pair<String, ObserverPtr<Animation>>>& animations);
		void RemoveAnimation(const String& animationName);

		void Update(const f32 deltaTime);

		inline ObserverPtr<const Animation> GetCurrentAnimation() const noexcept { return m_currentAnimation; }
		void SetCurrentAnimation(const String& animationName);

		inline f32 GetFPS() const noexcept { return m_fps; }
		void SetFPS(const u32 frameRate) noexcept;
	};
}

#endif