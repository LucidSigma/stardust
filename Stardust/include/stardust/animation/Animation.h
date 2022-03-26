#pragma once
#ifndef STARDUST_ANIMATION_H
#define STARDUST_ANIMATION_H

#include <functional>

#include "stardust/animation/easings/Easings.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace animation
    {
        class Animation final
        {
        public:
            using KeyFrame = u32;
            using Event = std::function<auto() -> void>;

            enum class Attribute
            {
                Position,
                Rotation,
                Scale,
                Shear,
                Colour,
            };

            struct KeyFrameData final
            {
                Optional<String> subTextureName;
                Optional<Vector2> position;
                Optional<f32> rotation;
                Optional<Vector2> scale;
                Optional<Vector2> shear;
                Optional<Colour> colour;
            };

            struct CreateInfo final
            {
                f32 fps = 0.0f;
                usize length = 0u;

                HashMap<Attribute, EasingFunction> attributeEasings{ };
                HashMap<KeyFrame, KeyFrameData> keyFrames{ };
                HashMap<KeyFrame, Event> events{ };

                ObserverPointer<const graphics::TextureAtlas> textureAtlas = nullptr;
            };

        private:
            template <typename A>
            struct AttributeData final
            {
                struct KeyFrameData final
                {
                    KeyFrame keyFrame;
                    A value;

                    [[nodiscard]] friend inline auto operator <(const KeyFrameData& lhs, const KeyFrameData& rhs) noexcept -> bool
                    {
                        return lhs.keyFrame < rhs.keyFrame;
                    }
                };

                List<KeyFrameData> keyFrames{ };
                usize currentIndex = 0u;

                EasingFunction easing = easings::EaseLinear;

                auto Step(const KeyFrame currentKeyFrame) -> void
                {
                    if (keyFrames.size() <= 1u)
                    {
                        return;
                    }

                    const KeyFrame nextFrame = currentIndex == keyFrames.size() - 1u
                        ? 0u
                        : keyFrames[currentIndex + 1u].keyFrame;

                    if (currentKeyFrame == nextFrame)
                    {
                        ++currentIndex;
                        currentIndex %= keyFrames.size();
                    }
                }
            };

            inline static HashMap<String, EasingFunction> s_customEasingFunctions{ };

            KeyFrame m_currentKeyFrame = 0u;
            KeyFrame m_maxKeyFrame = 0u;

            f32 m_fps = 0u;
            f32 m_secondsPerFrame = 0.0f;

            AttributeData<graphics::TextureCoordinatePair> m_textureAreaFrames{ };
            AttributeData<Vector2> m_positionFrames{ };
            AttributeData<Quaternion> m_rotationFrames{ };

            AttributeData<Vector2> m_scaleFrames{ };
            AttributeData<Vector2> m_shearFrames{ };
            AttributeData<Colour> m_colourFrames{ };

            HashMap<KeyFrame, List<Event>> m_eventCallbacks{ };
            ObserverPointer<const graphics::TextureAtlas> m_textureAtlas = nullptr;

        public:
            Animation() = default;
            Animation(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events = { });
            explicit Animation(const CreateInfo& createInfo);

            auto Initialise(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events = { }) -> void;
            auto Initialise(const CreateInfo& createInfo) -> void;

            auto AddEvent(const KeyFrame keyFrame, const Event& event) -> void;

            auto Step() -> void;
            auto Reset() -> void;

            [[nodiscard]] auto GetTextureArea() const -> const graphics::TextureCoordinatePair&;
            [[nodiscard]] auto GetPosition(const f32 frameInterpolation) const -> Vector2;
            [[nodiscard]] auto GetRotation(const f32 frameInterpolation) const -> f32;
            [[nodiscard]] auto GetScale(const f32 frameInterpolation) const -> Vector2;
            [[nodiscard]] auto GetShear(const f32 frameInterpolation) const -> Vector2;
            [[nodiscard]] auto GetColour(const f32 frameInterpolation) const -> Colour;

            [[nodiscard]] inline auto GetCurrentKeyFrame() const noexcept -> KeyFrame { return m_currentKeyFrame; }
            [[nodiscard]] inline auto GetFrameCount() const noexcept -> u32 { return m_maxKeyFrame; }

            [[nodiscard]] inline auto GetFPS() const noexcept -> f32 { return m_fps; }
            auto SetFPS(const f32 fps) noexcept -> void;
            [[nodiscard]] inline auto GetSecondsPerFrame() const noexcept -> f32 { return m_secondsPerFrame; }

            [[nodiscard]] inline auto GetTextureAtlas() const noexcept -> ObserverPointer<const graphics::TextureAtlas> { return m_textureAtlas; }

        private:
            auto AddKeyFrame(const KeyFrame keyFrame, const KeyFrameData& keyFrameData) -> void;
            auto SetAttributeEasing(const Attribute attribute, const EasingFunction& easingFunction) -> void;

            [[nodiscard]] auto LoadCreateInfoFromTable(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events) -> CreateInfo;
            auto AddDefaultKeyFrames() -> void;

            [[nodiscard]] auto GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame, const f32 frameInterpolation, const EasingFunction& easingFunction) const -> f32;
        };
    }

    namespace anim = animation;
}

#endif
