#include "stardust/animation/Animation.h"

#include <algorithm>

#include "stardust/graphics/colour/Colours.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace animation
    {
        Animation::Animation(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events)
        {
            Initialise(scriptTable, textureAtlas, events);
        }

        Animation::Animation(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto Animation::Initialise(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events) -> void
        {
            const CreateInfo createInfo = LoadCreateInfoFromTable(scriptTable, textureAtlas, events);

            Initialise(createInfo);
        }

        auto Animation::Initialise(const CreateInfo& createInfo) -> void
        {
            m_maxKeyFrame = static_cast<KeyFrame>(createInfo.length);
            SetFPS(createInfo.fps);
            m_textureAtlas = createInfo.textureAtlas;

            for (const auto& [attribute, easing] : createInfo.attributeEasings)
            {
                SetAttributeEasing(attribute, easing);
            }

            for (const auto& [keyFrame, keyFrameData] : createInfo.keyFrames)
            {
                AddKeyFrame(keyFrame, keyFrameData);
            }

            AddDefaultKeyFrames();

            std::ranges::sort(m_textureAreaFrames.keyFrames, std::less());
            std::ranges::sort(m_positionFrames.keyFrames, std::less());
            std::ranges::sort(m_rotationFrames.keyFrames, std::less());
            std::ranges::sort(m_scaleFrames.keyFrames, std::less());
            std::ranges::sort(m_shearFrames.keyFrames, std::less());
            std::ranges::sort(m_colourFrames.keyFrames, std::less());

            for (const auto& [keyFrame, event] : createInfo.events)
            {
                AddEvent(keyFrame, event);
            }
        }

        auto Animation::AddEvent(const KeyFrame keyFrame, const Event& event) -> void
        {
            if (!m_eventCallbacks.contains(keyFrame))
            {
                m_eventCallbacks[keyFrame] = { };
            }

            m_eventCallbacks[keyFrame].push_back(event);
        }

        auto Animation::Step() -> void
        {
            ++m_currentKeyFrame;
            m_currentKeyFrame %= m_maxKeyFrame;

            m_textureAreaFrames.Step(m_currentKeyFrame);
            m_positionFrames.Step(m_currentKeyFrame);
            m_rotationFrames.Step(m_currentKeyFrame);
            m_scaleFrames.Step(m_currentKeyFrame);
            m_shearFrames.Step(m_currentKeyFrame);
            m_colourFrames.Step(m_currentKeyFrame);

            if (m_eventCallbacks.contains(m_currentKeyFrame))
            {
                for (const auto& event : m_eventCallbacks[m_currentKeyFrame])
                {
                    event();
                }
            }
        }

        auto Animation::Reset() -> void
        {
            m_currentKeyFrame = 0u;

            m_textureAreaFrames.currentIndex = 0u;
            m_positionFrames.currentIndex = 0u;
            m_rotationFrames.currentIndex = 0u;
            m_scaleFrames.currentIndex = 0u;
            m_shearFrames.currentIndex = 0u;
            m_colourFrames.currentIndex = 0u;
        }

        [[nodiscard]] auto Animation::GetTextureArea() const -> const graphics::TextureCoordinatePair&
        {
            return m_textureAreaFrames.keyFrames[m_textureAreaFrames.currentIndex].value;
        }

        [[nodiscard]] auto Animation::GetPosition(const f32 frameInterpolation) const -> Vector2
        {
            const usize positionFrameCount = m_positionFrames.keyFrames.size();

            if (positionFrameCount == 1u)
            {
                return m_positionFrames.keyFrames.front().value;
            }

            const auto [currentKeyFrame, currentPositionOffset] = m_positionFrames.keyFrames[m_positionFrames.currentIndex];
            const auto [nextKeyFrame, nextPositionOffset] = m_positionFrames.keyFrames[(m_positionFrames.currentIndex + 1u) % positionFrameCount];

            const f32 percentage = GetPercentageBetweenFrames(currentKeyFrame, nextKeyFrame, frameInterpolation, m_positionFrames.easing);

            return glm::lerp(currentPositionOffset, nextPositionOffset, percentage);
        }

        [[nodiscard]] auto Animation::GetRotation(const f32 frameInterpolation) const -> f32
        {
            const usize rotationFrameCount = m_rotationFrames.keyFrames.size();

            if (rotationFrameCount == 1u)
            {
                return glm::degrees(glm::roll(m_rotationFrames.keyFrames.front().value));
            }

            const auto& [currentKeyFrame, currentRotation] = m_rotationFrames.keyFrames[m_rotationFrames.currentIndex];
            const auto& [nextKeyFrame, nextRotation] = m_rotationFrames.keyFrames[(m_rotationFrames.currentIndex + 1u) % rotationFrameCount];

            const f32 percentage = GetPercentageBetweenFrames(currentKeyFrame, nextKeyFrame, frameInterpolation, m_rotationFrames.easing);

            return glm::degrees(glm::roll(glm::slerp(currentRotation, nextRotation, percentage)));
        }

        [[nodiscard]] auto Animation::GetScale(const f32 frameInterpolation) const -> Vector2
        {
            const usize scaleFrameCount = m_scaleFrames.keyFrames.size();

            if (scaleFrameCount == 1u)
            {
                return m_scaleFrames.keyFrames.front().value;
            }

            const auto [currentKeyFrame, currentScale] = m_scaleFrames.keyFrames[m_scaleFrames.currentIndex];
            const auto [nextKeyFrame, nextScale] = m_scaleFrames.keyFrames[(m_scaleFrames.currentIndex + 1u) % scaleFrameCount];

            const f32 percentage = GetPercentageBetweenFrames(currentKeyFrame, nextKeyFrame, frameInterpolation, m_scaleFrames.easing);

            return glm::lerp(currentScale, nextScale, percentage);
        }

        [[nodiscard]] auto Animation::GetShear(const f32 frameInterpolation) const -> Vector2
        {
            const usize shearFrameCount = m_shearFrames.keyFrames.size();

            if (shearFrameCount == 1u)
            {
                return m_shearFrames.keyFrames.front().value;
            }

            const auto [currentKeyFrame, currentShear] = m_shearFrames.keyFrames[m_shearFrames.currentIndex];
            const auto [nextKeyFrame, nextShear] = m_shearFrames.keyFrames[(m_shearFrames.currentIndex + 1u) % shearFrameCount];

            const f32 percentage = GetPercentageBetweenFrames(currentKeyFrame, nextKeyFrame, frameInterpolation, m_shearFrames.easing);

            return glm::lerp(currentShear, nextShear, percentage);
        }

        [[nodiscard]] auto Animation::GetColour(const f32 frameInterpolation) const -> Colour
        {
            const usize colourFrameCount = m_colourFrames.keyFrames.size();

            if (colourFrameCount == 1u)
            {
                return m_colourFrames.keyFrames.front().value;
            }

            const auto& [currentKeyFrame, currentColour] = m_colourFrames.keyFrames[m_colourFrames.currentIndex];
            const auto& [nextKeyFrame, nextColour] = m_colourFrames.keyFrames[(m_colourFrames.currentIndex + 1u) % colourFrameCount];

            const f32 percentage = GetPercentageBetweenFrames(currentKeyFrame, nextKeyFrame, frameInterpolation, m_colourFrames.easing);

            return Colour(glm::lerp(Vector4(currentColour), Vector4(nextColour), percentage));
        }

        auto Animation::SetFPS(const f32 fps) noexcept -> void
        {
            m_fps = fps;
            m_secondsPerFrame = 1.0f / m_fps;
        }

        auto Animation::AddKeyFrame(const KeyFrame keyFrame, const KeyFrameData& keyFrameData) -> void
        {
            if (keyFrameData.subTextureName.has_value() && m_textureAtlas != nullptr)
            {
                m_textureAreaFrames.keyFrames.push_back({
                    keyFrame,
                    m_textureAtlas->GetSubTexture(keyFrameData.subTextureName.value()),
                });
            }

            if (keyFrameData.position.has_value())
            {
                m_positionFrames.keyFrames.push_back({
                    keyFrame,
                    keyFrameData.position.value(),
                });
            }

            if (keyFrameData.rotation.has_value())
            {
                m_rotationFrames.keyFrames.push_back({
                    keyFrame,
                    glm::angleAxis(glm::radians(keyFrameData.rotation.value()), Vector3Forward),
                });
            }

            if (keyFrameData.scale.has_value())
            {
                m_scaleFrames.keyFrames.push_back({
                    keyFrame,
                    keyFrameData.scale.value(),
                });
            }

            if (keyFrameData.shear.has_value())
            {
                m_shearFrames.keyFrames.push_back({
                    keyFrame,
                    keyFrameData.shear.value(),
                });
            }

            if (keyFrameData.colour.has_value())
            {
                m_colourFrames.keyFrames.push_back({
                    keyFrame,
                    keyFrameData.colour.value(),
                });
            }
        }

        auto Animation::SetAttributeEasing(const Attribute attribute, const EasingFunction& easingFunction) -> void
        {
            switch (attribute)
            {
            case Attribute::Position:
                m_positionFrames.easing = easingFunction;

                break;

            case Attribute::Rotation:
                m_rotationFrames.easing = easingFunction;

                break;

            case Attribute::Scale:
                m_scaleFrames.easing = easingFunction;

                break;

            case Attribute::Shear:
                m_shearFrames.easing = easingFunction;

                break;

            case Attribute::Colour:
                m_colourFrames.easing = easingFunction;

                break;
            }
        }

        [[nodiscard]] auto Animation::LoadCreateInfoFromTable(const Table& scriptTable, const ObserverPointer<const graphics::TextureAtlas> textureAtlas, const HashMap<KeyFrame, Event>& events) -> CreateInfo
        {
            CreateInfo createInfo;
            createInfo.length = scriptTable["length"];
            createInfo.fps = scriptTable["fps"];
            createInfo.events = events;
            createInfo.textureAtlas = textureAtlas;

            const Table tweens = scriptTable["tweens"];

            for (const auto& [attributeKey, attributeData] : tweens)
            {
                const String attributeName = attributeKey.as<String>();
                const Table attributeTable = attributeData.as<Table>();
                const Table frameTable = attributeTable["frames"];

                const auto updateCreateInfo = [&createInfo](const KeyFrame currentKeyFrame) -> void
                {
                    createInfo.length = std::max(createInfo.length, static_cast<usize>(currentKeyFrame) + 1u);

                    if (!createInfo.keyFrames.contains(currentKeyFrame))
                    {
                        createInfo.keyFrames[currentKeyFrame] = KeyFrameData{ };
                    }
                };

                if (attributeName == "sprite")
                {
                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].subTextureName = value.as<String>();
                    }
                }
                else if (attributeName == "position")
                {
                    createInfo.attributeEasings[Attribute::Position] = attributeTable["easing"].get<EasingFunction>();

                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].position = value.as<Vector2>();
                    }
                }
                else if (attributeName == "rotation")
                {
                    createInfo.attributeEasings[Attribute::Rotation] = attributeTable["easing"].get<EasingFunction>();

                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].rotation = value.as<f32>();
                    }
                }
                else if (attributeName == "scale")
                {
                    createInfo.attributeEasings[Attribute::Scale] = attributeTable["easing"].get<EasingFunction>();

                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].scale = value.as<Vector2>();
                    }
                }
                else if (attributeName == "shear")
                {
                    createInfo.attributeEasings[Attribute::Shear] = attributeTable["easing"].get<EasingFunction>();

                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].shear = value.as<Vector2>();
                    }
                }
                else if (attributeName == "colour")
                {
                    createInfo.attributeEasings[Attribute::Colour] = attributeTable["easing"].get<EasingFunction>();

                    for (const auto& [frame, value] : frameTable)
                    {
                        const KeyFrame currentKeyFrame = frame.as<KeyFrame>();
                        updateCreateInfo(currentKeyFrame);

                        createInfo.keyFrames[currentKeyFrame].colour = value.as<Colour>();
                    }
                }
            }

            return createInfo;
        }

        auto Animation::AddDefaultKeyFrames() -> void
        {
            constexpr auto ContainsInitialFrame = []<typename T>(const List<T>& keyFrames) -> bool
            {
                return std::ranges::find_if(
                    keyFrames,
                    [](const T& frameData) -> bool { return frameData.keyFrame == 0u; }
                ) != std::cend(keyFrames);
            };

            if (m_textureAreaFrames.keyFrames.empty() || !ContainsInitialFrame(m_textureAreaFrames.keyFrames))
            {
                m_textureAreaFrames.keyFrames.push_back({
                    0u,
                    graphics::TextureCoordinatePair{
                        Vector2Zero,
                        Vector2One,
                    },
                });
            }

            if (m_positionFrames.keyFrames.empty() || !ContainsInitialFrame(m_positionFrames.keyFrames))
            {
                m_positionFrames.keyFrames.push_back({ 0u, Vector2Zero });
            }

            if (m_rotationFrames.keyFrames.empty() || !ContainsInitialFrame(m_rotationFrames.keyFrames))
            {
                m_rotationFrames.keyFrames.push_back({ 0u, glm::angleAxis(0.0f, Vector3Forward) });
            }

            if (m_scaleFrames.keyFrames.empty() || !ContainsInitialFrame(m_scaleFrames.keyFrames))
            {
                m_scaleFrames.keyFrames.push_back({ 0u, Vector2One });
            }

            if (m_shearFrames.keyFrames.empty() || !ContainsInitialFrame(m_shearFrames.keyFrames))
            {
                m_shearFrames.keyFrames.push_back({ 0u, Vector2Zero });
            }

            if (m_colourFrames.keyFrames.empty() || !ContainsInitialFrame(m_colourFrames.keyFrames))
            {
                m_colourFrames.keyFrames.push_back({ 0u, colours::White });
            }
        }

        [[nodiscard]] auto Animation::GetPercentageBetweenFrames(const KeyFrame currentFrame, KeyFrame nextFrame, const f32 frameInterpolation, const EasingFunction& easingFunction) const -> f32
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
}
