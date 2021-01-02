#include "stardust/animation/Animator.h"

namespace stardust
{
	void Animator::AddAnimation(const String& animationName, Animation& animation, const bool setAsCurrent)
	{
		m_animations.insert({ animationName, &animation });

		if (setAsCurrent)
		{
			SetCurrentAnimation(animationName);
		}
	}

	void Animator::AddAnimations(const Vector<Pair<String, ObserverPtr<Animation>>>& animations)
	{
		for (const auto& [animationName, animation] : animations)
		{
			m_animations.insert({ animationName, animation });
		}
	}

	void Animator::RemoveAnimation(const String& animationName)
	{
		m_animations.erase(animationName);
	}

	void Animator::Update(const f32 deltaTime)
	{
		const f32 secondsPerFrame = m_currentAnimation->GetSecondsPerFrame();
		m_frameTimeAccumulator += deltaTime * m_speed;

		while (m_frameTimeAccumulator >= secondsPerFrame)
		{
			m_currentAnimation->Step();
			m_frameTimeAccumulator -= secondsPerFrame;
		}

		m_currentFramePercentage = m_frameTimeAccumulator / secondsPerFrame;
	}

	[[nodiscard]] const TextureCoordinatePair& Animator::GetSprite() const
	{
		return m_currentAnimation->GetSprite();
	}

	[[nodiscard]] Vec2 Animator::GetPositionOffset() const
	{
		return m_currentAnimation->GetPositionOffset(m_currentFramePercentage);
	}

	[[nodiscard]] f32 Animator::GetRotation() const
	{
		return m_currentAnimation->GetRotation(m_currentFramePercentage);
	}

	[[nodiscard]] Vec2 Animator::GetScale() const
	{
		return m_currentAnimation->GetScale(m_currentFramePercentage);
	}

	[[nodiscard]] Vec2 Animator::GetShear() const
	{
		return m_currentAnimation->GetShear(m_currentFramePercentage);
	}

	[[nodiscard]] Colour Animator::GetColour() const
	{
		return m_currentAnimation->GetColour(m_currentFramePercentage);
	}

	void Animator::SetCurrentAnimation(const String& animationName)
	{
		if (m_currentAnimation != nullptr) [[unlikely]]
		{
			m_currentAnimation->Reset();
			m_frameTimeAccumulator = 0.0f;
		}

		m_currentAnimation = m_animations[animationName];
	}
}