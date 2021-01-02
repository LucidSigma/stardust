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

	void Animator::SetCurrentAnimation(const String& animationName)
	{
		m_currentAnimation = m_animations[animationName];
	}

	void Animator::SetFPS(const u32 frameRate) noexcept
	{
		m_fps = frameRate;
		m_millisecondsPerFrame = 1'000.0f / static_cast<f32>(frameRate);
	}
}