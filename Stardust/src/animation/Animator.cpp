#include "stardust/animation/Animator.h"

namespace stardust
{
    namespace animation
    {
        auto Animator::AddAnimation(const String& animationName, Animation& animation, const bool setAsCurrent) -> void
        {
            m_animations.insert({ animationName, &animation });

            if (setAsCurrent)
            {
                SetCurrentAnimation(animationName);
            }
        }

        auto Animator::AddAnimations(const List<Pair<String, ObserverPointer<Animation>>>& animations) -> void
        {
            for (const auto& [animationName, animation] : animations)
            {
                m_animations.insert({ animationName, animation });
            }
        }

        auto Animator::RemoveAnimation(const String& animationName) -> void
        {
            m_animations.erase(animationName);
        }

        [[nodiscard]] auto Animator::HasAnimation(const String& animationName) -> bool
        {
            return m_animations.contains(animationName);
        }

        auto Animator::Update(const f32 deltaTime) -> void
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

        [[nodiscard]] auto Animator::GetTextureArea() const -> const graphics::TextureCoordinatePair&
        {
            return m_currentAnimation->GetTextureArea();
        }

        [[nodiscard]] auto Animator::GetPosition() const -> Vector2
        {
            return m_currentAnimation->GetPosition(m_currentFramePercentage);
        }

        [[nodiscard]] auto Animator::GetRotation() const -> f32
        {
            return m_currentAnimation->GetRotation(m_currentFramePercentage);
        }

        [[nodiscard]] auto Animator::GetScale() const -> Vector2
        {
            return m_currentAnimation->GetScale(m_currentFramePercentage);
        }

        [[nodiscard]] auto Animator::GetShear() const -> Vector2
        {
            return m_currentAnimation->GetShear(m_currentFramePercentage);
        }

        [[nodiscard]] auto Animator::GetColour() const -> Colour
        {
            return m_currentAnimation->GetColour(m_currentFramePercentage);
        }

        [[nodiscard]] auto Animator::GetTransform(const Vector2 worldPosition, const Optional<Vector2>& pivot) const -> components::Transform
        {
            const Vector2 scale = GetScale();
            graphics::Reflection reflection = graphics::Reflection::None;

            if (scale.x < 0.0f && scale.y < 0.0f)
            {
                reflection = graphics::Reflection::Both;
            }
            else if (scale.x < 0.0f)
            {
                reflection = graphics::Reflection::Horizontal;
            }
            else if (scale.y < 0.0f)
            {
                reflection = graphics::Reflection::Vertical;
            }

            return components::Transform{
                .translation = worldPosition + GetPosition(),
                .scale = glm::abs(scale),
                .reflection = reflection,
                .rotation = GetRotation(),
                .pivot = pivot,
                .shear = GetShear(),
            };
        }

        [[nodiscard]] auto Animator::GetScreenTransform(const Optional<IVector2>& pivot) const -> components::ScreenTransform
        {
            const Vector2 scale = GetScale();
            graphics::Reflection reflection = graphics::Reflection::None;

            if (scale.x < 0.0f && scale.y < 0.0f)
            {
                reflection = graphics::Reflection::Both;
            }
            else if (scale.x < 0.0f)
            {
                reflection = graphics::Reflection::Horizontal;
            }
            else if (scale.y < 0.0f)
            {
                reflection = graphics::Reflection::Vertical;
            }

            return components::ScreenTransform{
                .translation = IVector2(GetPosition()),
                .reflection = reflection,
                .rotation = GetRotation(),
                .pivot = pivot,
                .shear = GetShear(),
            };
        }

        [[nodiscard]] auto Animator::GetSprite() const -> components::Sprite
        {
            if (m_currentAnimation->GetTextureAtlas() == nullptr)
            {
                return components::Sprite{
                    .texture = nullptr,
                    .subTextureArea = graphics::TextureCoordinatePair{ },
                    .colourMod = GetColour(),
                };
            }
            else
            {
                return components::Sprite{
                    .texture = &m_currentAnimation->GetTextureAtlas()->GetTexture(),
                    .subTextureArea = GetTextureArea(),
                    .colourMod = GetColour(),
                };
            }
        }

        auto Animator::SkipToFrame(Animation::KeyFrame frame) const -> void
        {
            frame %= m_currentAnimation->GetFrameCount();

            while (m_currentAnimation->GetCurrentKeyFrame() != frame)
            {
                m_currentAnimation->Step();
            }
        }

        auto Animator::SetCurrentAnimation(const String& animationName) -> void
        {
            if (m_currentAnimation != nullptr) [[unlikely]]
            {
                m_currentAnimation->Reset();
                m_frameTimeAccumulator = 0.0f;
            }

            m_currentAnimation = m_animations[animationName];
        }
    }
}
