#pragma once
#ifndef STARDUST_ANIMATOR_H
#define STARDUST_ANIMATOR_H

#include "stardust/animation/Animation.h"
#include "stardust/ecs/components/ScreenTransformComponent.h"
#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/ecs/components/TransformComponent.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace animation
    {
        class Animator final
        {
        private:
            HashMap<String, ObserverPointer<Animation>> m_animations{ };
            ObserverPointer<Animation> m_currentAnimation = nullptr;

            f32 m_speed = 1.0f;
            f32 m_frameTimeAccumulator = 0.0f;
            f32 m_currentFramePercentage = 0.0f;

        public:
            auto AddAnimation(const String& animationName, Animation& animation, const bool setAsCurrent = false) -> void;
            auto AddAnimations(const List<Pair<String, ObserverPointer<Animation>>>& animations) -> void;
            auto RemoveAnimation(const String& animationName) -> void;
            [[nodiscard]] auto HasAnimation(const String& animationName) -> bool;

            auto Update(const f32 deltaTime) -> void;

            [[nodiscard]] auto GetTextureArea() const -> const graphics::TextureCoordinatePair&;
            [[nodiscard]] auto GetPosition() const->Vector2;
            [[nodiscard]] auto GetRotation() const->f32;
            [[nodiscard]] auto GetScale() const->Vector2;
            [[nodiscard]] auto GetShear() const->Vector2;
            [[nodiscard]] auto GetColour() const->Colour;

            [[nodiscard]] auto GetTransform(const Vector2 worldPosition = Vector2Zero, const Optional<Vector2>& pivot = None) const -> components::Transform;
            [[nodiscard]] auto GetScreenTransform(const Optional<IVector2>& pivot = None) const -> components::ScreenTransform;
            [[nodiscard]] auto GetSprite() const -> components::Sprite;

            auto SkipToFrame(Animation::KeyFrame frame) const -> void;

            [[nodiscard]] inline auto GetCurrentAnimation() const noexcept -> ObserverPointer<const Animation> { return m_currentAnimation; }
            auto SetCurrentAnimation(const String& animationName) -> void;

            [[nodiscard]] inline auto GetSpeed() const noexcept -> f32 { return m_speed; }
            inline auto SetSpeed(const f32 speed) noexcept -> void { m_speed = speed; }
        };
    }
}

#endif
