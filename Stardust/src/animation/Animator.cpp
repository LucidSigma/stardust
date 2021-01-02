#include "stardust/animation/Animator.h"

namespace stardust
{
	Animator::Animator()
	{
		SetFPS(s_DefaultFrameRate);
	}

	void Animator::AddAnimation(const String& animationName, Animation& animation)
	{
		m_animations.insert({ animationName, &animation });
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
		m_currentFrameTimeAccumulator += deltaTime;

		while (m_currentFrameTimeAccumulator <= m_secondsPerFrame)
		{
			m_currentAnimation->Step();
			m_currentFrameTimeAccumulator -= m_secondsPerFrame;
		}
	}

	void Animator::SetCurrentAnimation(const String& animationName)
	{
		m_currentAnimation->Reset();
		m_currentAnimation = m_animations[animationName];
	}

	void Animator::SetFPS(const u32 frameRate) noexcept
	{
		m_fps = frameRate;
		m_secondsPerFrame = 1.0f / static_cast<f32>(frameRate);
	}
}