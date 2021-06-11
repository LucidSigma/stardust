#pragma once
#ifndef STARDUST_ANIMATION_H
#define STARDUST_ANIMATION_H

#include <functional>

#include <nlohmann/json.hpp>

#include "stardust/animation/Easings.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/colour/Colour.h"

namespace stardust
{
    using KeyFrame = u32;

    class Animation
    {
    public:
        using Event = std::function<void()>;

    private:
        inline static HashMap<String, EasingFunction> s_customEasingFunctions{ };

        Vector<Pair<KeyFrame, TextureCoordinatePair>> m_spriteFrames{ };
        Vector<Pair<KeyFrame, Vec2>> m_positionOffsetFrames{ };
        Vector<Pair<KeyFrame, Quaternion>> m_rotationFrames{ };
        Vector<Pair<KeyFrame, Vec2>> m_scaleFrames{ };
        Vector<Pair<KeyFrame, Vec2>> m_shearFrames{ };
        Vector<Pair<KeyFrame, Colour>> m_colourFrames{ };

        EasingFunction m_positionOffsetEasing = easings::EaseLinear;
        EasingFunction m_rotationEasing = easings::EaseLinear;
        EasingFunction m_scaleEasing = easings::EaseLinear;
        EasingFunction m_shearEasing = easings::EaseLinear;
        EasingFunction m_colourEasing = easings::EaseLinear;

        HashMap<KeyFrame, Vector<Event>> m_eventCallbacks{ };

        KeyFrame m_currentKeyFrame = 0u;
        KeyFrame m_maxKeyFrame = 0u;

        usize m_currentSpriteIndex = 0u;
        usize m_currentPositionOffsetIndex = 0u;
        usize m_currentRotationIndex = 0u;
        usize m_currentScaleIndex = 0u;
        usize m_currentShearIndex = 0u;
        usize m_currentColourIndex = 0u;

        u32 m_fps = 0u;
        f32 m_secondsPerFrame = 0.0f;

        bool m_didLoadSuccessfully = false;

    public:
        static void AddCustomEasingFunction(const String& name, const EasingFunction& easingFunction);

        Animation() = default;
        explicit Animation(const StringView& filepath);
        Animation(const StringView& filepath, const TextureAtlas& textureAtlas);
        ~Animation() noexcept = default;

        void Initialise(const StringView& filepath);
        void Initialise(const StringView& filepath, const TextureAtlas& textureAtlas);

        void AddEvent(const KeyFrame keyFrame, const Event& event);

        void Step();

        [[nodiscard]] const TextureCoordinatePair& GetSprite() const;
        [[nodiscard]] Vec2 GetPositionOffset(const f32 frameInterpolation) const;
        [[nodiscard]] f32 GetRotation(const f32 frameInterpolation) const;
        [[nodiscard]] Vec2 GetScale(const f32 frameInterpolation) const;
        [[nodiscard]] Vec2 GetShear(const f32 frameInterpolation) const;
        [[nodiscard]] Colour GetColour(const f32 frameInterpolation) const;

        void Reset();

        [[nodiscard]] inline KeyFrame GetCurrentKeyFrame() const noexcept { return m_currentKeyFrame; }
        [[nodiscard]] inline u32 GetFrameCount() const noexcept { return m_maxKeyFrame; }
        [[nodiscard]] inline u32 GetFPS() const noexcept { return m_fps; }
        [[nodiscard]] inline f32 GetSecondsPerFrame() const noexcept { return m_secondsPerFrame; }

        [[nodiscard]] inline bool IsValid() const noexcept { return m_didLoadSuccessfully; }

    private:
        [[nodiscard]] static Optional<EasingFunction> GetEasingFunction(const String& name);

        void LoadFromFile(const StringView& filepath, const ObserverPtr<const TextureAtlas>& textureAtlas);
        void LoadAttributes(const nlohmann::json& data, const ObserverPtr<const TextureAtlas>& textureAtlas, const StringView& filepath);
        void LoadEasings(const nlohmann::json& data, const StringView& filepath);
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

        [[nodiscard]] f32 GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame, const f32 frameInterpolation, const EasingFunction& easingFunction) const;
    };
}

#endif