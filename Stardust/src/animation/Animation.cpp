#include "stardust/animation/Animation.h"

#include <algorithm>
#include <string>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/math/Math.h"

namespace stardust
{
    void Animation::AddCustomEasingFunction(const String& name, const EasingFunction& easingFunction)
    {
        s_customEasingFunctions[name] = easingFunction;
    }

    Animation::Animation(const StringView& filepath)
    {
        Initialise(filepath);
    }

    Animation::Animation(const StringView& filepath, const TextureAtlas& textureAtlas)
    {
        Initialise(filepath, textureAtlas);
    }

    void Animation::Initialise(const StringView& filepath)
    {
        LoadFromFile(filepath, nullptr);
    }

    void Animation::Initialise(const StringView& filepath, const TextureAtlas& textureAtlas)
    {
        LoadFromFile(filepath, &textureAtlas);
    }

    void Animation::AddEvent(const KeyFrame keyFrame, const Event& event)
    {
        if (!m_eventCallbacks.contains(keyFrame))
        {
            m_eventCallbacks[keyFrame] = { };
        }

        m_eventCallbacks[keyFrame].push_back(event);
    }

    void Animation::Step()
    {
        ++m_currentKeyFrame;
        m_currentKeyFrame %= m_maxKeyFrame;

        StepAttribute(m_spriteFrames, m_currentSpriteIndex);
        StepAttribute(m_positionOffsetFrames, m_currentPositionOffsetIndex);
        StepAttribute(m_rotationFrames, m_currentRotationIndex);
        StepAttribute(m_scaleFrames, m_currentScaleIndex);
        StepAttribute(m_shearFrames, m_currentShearIndex);
        StepAttribute(m_colourFrames, m_currentColourIndex);

        if (m_eventCallbacks.contains(m_currentKeyFrame))
        {
            for (const auto& event : m_eventCallbacks[m_currentKeyFrame])
            {
                event();
            }
        }
    }

    [[nodiscard]] const TextureCoordinatePair& Animation::GetSprite() const
    {
        return m_spriteFrames[m_currentSpriteIndex].second;
    }

    [[nodiscard]] Vec2 Animation::GetPositionOffset(const f32 frameInterpolation) const
    {
        if (m_positionOffsetFrames.size() == 1u)
        {
            return m_positionOffsetFrames.front().second;
        }

        const Vec2& currentPositionOffset = m_positionOffsetFrames[m_currentPositionOffsetIndex].second;
        const Vec2& nextPositionOffset = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].second;

        const KeyFrame currentFrame = m_positionOffsetFrames[m_currentPositionOffsetIndex].first;
        const KeyFrame nextFrame = m_positionOffsetFrames[(m_currentPositionOffsetIndex + 1u) % m_positionOffsetFrames.size()].first;

        const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation, m_positionOffsetEasing);

        return glm::lerp(currentPositionOffset, nextPositionOffset, percentage);
    }
    
    [[nodiscard]] f32 Animation::GetRotation(const f32 frameInterpolation) const
    {
        if (m_rotationFrames.size() == 1u)
        {
            return glm::degrees(glm::roll(m_rotationFrames.front().second));
        }

        const Quaternion& currentRotation = m_rotationFrames[m_currentRotationIndex].second;
        const Quaternion& nextRotation = m_rotationFrames[(m_currentRotationIndex + 1u) % m_rotationFrames.size()].second;

        const KeyFrame currentFrame = m_rotationFrames[m_currentRotationIndex].first;
        const KeyFrame nextFrame = m_rotationFrames[(m_currentRotationIndex + 1u) % m_rotationFrames.size()].first;

        const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation, m_rotationEasing);

        return glm::degrees(glm::roll(glm::slerp(currentRotation, nextRotation, percentage)));
    }
    
    [[nodiscard]] Vec2 Animation::GetScale(const f32 frameInterpolation) const
    {
        if (m_scaleFrames.size() == 1u)
        {
            return m_scaleFrames.front().second;
        }

        const Vec2& currentScale = m_scaleFrames[m_currentScaleIndex].second;
        const Vec2& nextScale = m_scaleFrames[(m_currentScaleIndex + 1u) % m_scaleFrames.size()].second;

        const KeyFrame currentFrame = m_scaleFrames[m_currentScaleIndex].first;
        const KeyFrame nextFrame = m_scaleFrames[(m_currentScaleIndex + 1u) % m_scaleFrames.size()].first;

        const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation, m_scaleEasing);

        return glm::lerp(currentScale, nextScale, percentage);
    }
    
    [[nodiscard]] Vec2 Animation::GetShear(const f32 frameInterpolation) const
    {
        if (m_shearFrames.size() == 1u)
        {
            return m_shearFrames.front().second;
        }

        const Vec2& currentShear = m_shearFrames[m_currentShearIndex].second;
        const Vec2& nextShear = m_shearFrames[(m_currentShearIndex + 1u) % m_shearFrames.size()].second;

        const KeyFrame currentFrame = m_shearFrames[m_currentShearIndex].first;
        const KeyFrame nextFrame = m_shearFrames[(m_currentShearIndex + 1u) % m_shearFrames.size()].first;

        const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation, m_shearEasing);

        return glm::lerp(currentShear, nextShear, percentage);
    }
    
    [[nodiscard]] Colour Animation::GetColour(const f32 frameInterpolation) const
    {
        if (m_colourFrames.size() == 1u)
        {
            return m_colourFrames.front().second;
        }

        const Colour& currentColour = m_colourFrames[m_currentColourIndex].second;
        const Colour& nextColour = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].second;

        const KeyFrame currentFrame = m_colourFrames[m_currentColourIndex].first;
        const KeyFrame nextFrame = m_colourFrames[(m_currentColourIndex + 1u) % m_colourFrames.size()].first;

        const f32 percentage = GetPercentageBetweenFrames(currentFrame, nextFrame, frameInterpolation, m_colourEasing);

        return Colour(glm::lerp(Vec4(currentColour), Vec4(nextColour), percentage));
    }

    void Animation::Reset()
    {
        m_currentKeyFrame = 0u;
        
        m_currentSpriteIndex = 0u;
        m_currentPositionOffsetIndex = 0u;
        m_currentRotationIndex = 0u;
        m_currentScaleIndex = 0u;
        m_currentShearIndex = 0u;
        m_currentColourIndex = 0u;
    }

    [[nodiscard]] Optional<EasingFunction> Animation::GetEasingFunction(const String& name)
    {
        const Optional<EasingFunction> easingFunction = GetEasingFunctionFromString(name);

        if (easingFunction.has_value())
        {
            return easingFunction;
        }
        
        if (const auto easingFunctionLocation = s_customEasingFunctions.find(name);
            easingFunctionLocation != std::cend(s_customEasingFunctions))
        {
            return easingFunctionLocation->second;
        }
        else
        {
            return NullOpt;
        }
    }

    void Animation::LoadFromFile(const StringView& filepath, const ObserverPtr<const TextureAtlas>& textureAtlas)
    {
        const nlohmann::json animationJSON = vfs::ReadJSON(filepath);

        if (animationJSON.is_discarded())
        {
            return;
        }
        
        try
        {
            LoadAttributes(animationJSON, textureAtlas, filepath);
        }
        catch (const nlohmann::json::exception& error)
        {
            Log::EngineError("Animation file {} is invalid: {}.", filepath, error.what());

            return;
        }

        AddDefaultKeyFrames();
        m_didLoadSuccessfully = true;
    }

    void Animation::LoadAttributes(const nlohmann::json& data, const ObserverPtr<const TextureAtlas>& textureAtlas, const StringView& filepath)
    {
        m_maxKeyFrame = data["length"];
        m_fps = data["fps"];
        m_secondsPerFrame = 1.0f / static_cast<f32>(m_fps);

        LoadEasings(data, filepath);

        for (const auto& [frameNumber, frameData] : data["frames"].items())
        {
            const KeyFrame currentKeyFrame = std::stoi(frameNumber);
            m_maxKeyFrame = std::max(m_maxKeyFrame, currentKeyFrame + 1);

            for (const auto& [attribute, value] : frameData.items())
            {
                if (attribute == "sprite" && textureAtlas != nullptr)
                {
                    m_spriteFrames.push_back({ currentKeyFrame, textureAtlas->GetSubtexture(value) });
                }
                else if (attribute == "position-offset")
                {
                    m_positionOffsetFrames.push_back({
                        currentKeyFrame,
                        Vec2{ value["x"], value["y"] },
                    });
                }
                else if (attribute == "rotation")
                {
                    m_rotationFrames.push_back({
                        currentKeyFrame,
                        glm::angleAxis(glm::radians(static_cast<f32>(value)), Vec3Forward),
                    });
                }
                else if (attribute == "scale")
                {
                    m_scaleFrames.push_back({
                        currentKeyFrame,
                        Vec2{ value["x"], value["y"] },
                    });
                }
                else if (attribute == "shear")
                {
                    m_shearFrames.push_back({
                        currentKeyFrame,
                        Vec2{ value["x"], value["y"] },
                    });
                }
                else if (attribute == "colour")
                {
                    m_colourFrames.push_back({
                        currentKeyFrame,
                        Colour(
                            static_cast<u8>(value["r"]),
                            static_cast<u8>(value["g"]),
                            static_cast<u8>(value["b"]),
                            static_cast<u8>(value["a"])
                        ),
                    });
                }
            }
        }

        constexpr auto SortingPredicate = [](const auto& lhs, const auto& rhs) -> bool
        {
            return lhs.first < rhs.first;
        };

        std::ranges::sort(m_spriteFrames, SortingPredicate);
        std::ranges::sort(m_positionOffsetFrames, SortingPredicate);
        std::ranges::sort(m_rotationFrames, SortingPredicate);
        std::ranges::sort(m_scaleFrames, SortingPredicate);
        std::ranges::sort(m_shearFrames, SortingPredicate);
        std::ranges::sort(m_colourFrames, SortingPredicate);
    }

    void Animation::LoadEasings(const nlohmann::json& data, const StringView& filepath)
    {
        if (data.contains("easings"))
        {
            if (data["easings"].is_string())
            {
                const Optional<EasingFunction> easingFunction = GetEasingFunction(data["easings"]);

                if (!easingFunction.has_value())
                {
                    Log::EngineWarn("Animation {} has easing {}, but this easing does not exist; defaulting to linear easing.", filepath, data["easings"]);
                }
                else
                {
                    m_positionOffsetEasing = easingFunction.value();
                    m_rotationEasing = easingFunction.value();
                    m_scaleEasing = easingFunction.value();
                    m_shearEasing = easingFunction.value();
                    m_colourEasing = easingFunction.value();
                }
            }
            else
            {
                for (const auto& [attribute, easingFunctionName] : data["easings"].items())
                {
                    const Optional<EasingFunction> easingFunction = GetEasingFunction(easingFunctionName);

                    if (!easingFunction.has_value())
                    {
                        Log::EngineWarn("Animation {} has easing {}, but this easing does not exist; defaulting to linear easing.", filepath, easingFunctionName);

                        continue;
                    }

                    if (attribute == "position-offset")
                    {
                        m_positionOffsetEasing = easingFunction.value();
                    }
                    else if (attribute == "rotation")
                    {
                        m_rotationEasing = easingFunction.value();
                    }
                    else if (attribute == "scale")
                    {
                        m_scaleEasing = easingFunction.value();
                    }
                    else if (attribute == "shear")
                    {
                        m_shearEasing = easingFunction.value();
                    }
                    else if (attribute == "colour")
                    {
                        m_colourEasing = easingFunction.value();
                    }
                }
            }
        }
    }

    void Animation::AddDefaultKeyFrames()
    {
        if (m_spriteFrames.empty())
        {
            m_spriteFrames.push_back({
                0u,
                TextureCoordinatePair{
                    Vec2Zero,
                    Vec2One,
                },
            });
        }

        if (m_positionOffsetFrames.empty())
        {
            m_positionOffsetFrames.push_back({ 0u, Vec2Zero });
        }

        if (m_rotationFrames.empty())
        {
            m_rotationFrames.push_back({ 0u, glm::angleAxis(0.0f, Vec3Forward) });
        }

        if (m_scaleFrames.empty())
        {
            m_scaleFrames.push_back({ 0u, Vec2One });
        }

        if (m_shearFrames.empty())
        {
            m_shearFrames.push_back({ 0u, Vec2Zero });
        }

        if (m_colourFrames.empty())
        {
            m_colourFrames.push_back({ 0u, colours::White });
        }
    }

    [[nodiscard]] f32 Animation::GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame, const f32 frameInterpolation, const EasingFunction& easingFunction) const
    {
        if (nextFrame < currentFrame)
        {
            nextFrame += m_maxKeyFrame;
        }

        const f32 frameDifference = static_cast<f32>(nextFrame) - static_cast<f32>(currentFrame);
        const f32 shiftedCurrentFrame = static_cast<f32>(m_currentKeyFrame) - static_cast<f32>(currentFrame);

        return easingFunction((shiftedCurrentFrame + frameInterpolation) / frameDifference);
    }
}