#include "stardust/input/joystick/Joystick.h"

#include <algorithm>
#include <utility>

#include "stardust/input/InputController.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/math/Math.h"

namespace stardust
{
    Joystick::Joystick(const ID id)
        : m_id(id), m_handle(SDL_JoystickOpen(id))
    {
        if (m_handle == nullptr)
        {
            return;
        }

        m_isOwning = true;

        Initialise();
    }

    Joystick::Joystick(const GameController& gameController)
    {
        m_id = gameController.GetID();
        m_handle = gameController.GetRawJoystickHandle();
        m_isOwning = false;

        Initialise();
    }

    Joystick::Joystick(Joystick&& other) noexcept
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_playerIndex, other.m_playerIndex);

        std::swap(m_name, other.m_name);
        std::swap(m_guid, other.m_guid);

        std::swap(m_handle, other.m_handle);
        std::swap(m_isOwning, other.m_isOwning);

        std::swap(m_currentButtonStates, other.m_currentButtonStates);
        std::swap(m_previousButtonStates, other.m_previousButtonStates);

        std::swap(m_currentAxes, other.m_currentAxes);
        std::swap(m_previousAxes, other.m_previousAxes);

        std::swap(m_currentHatSwitches, other.m_currentHatSwitches);
        std::swap(m_previousHatSwitches, other.m_previousHatSwitches);

        std::swap(m_trackballs, other.m_trackballs);
    }

    auto Joystick::operator =(Joystick&& other) noexcept -> Joystick&
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_playerIndex, other.m_playerIndex);

        std::swap(m_name, other.m_name);
        std::swap(m_guid, other.m_guid);

        std::swap(m_handle, other.m_handle);
        std::swap(m_isOwning, other.m_isOwning);

        std::swap(m_currentButtonStates, other.m_currentButtonStates);
        std::swap(m_previousButtonStates, other.m_previousButtonStates);

        std::swap(m_currentAxes, other.m_currentAxes);
        std::swap(m_previousAxes, other.m_previousAxes);

        std::swap(m_currentHatSwitches, other.m_currentHatSwitches);
        std::swap(m_previousHatSwitches, other.m_previousHatSwitches);

        std::swap(m_trackballs, other.m_trackballs);

        return *this;
    }

    Joystick::~Joystick() noexcept
    {
        Destroy();
    }

    auto Joystick::Update(const InputController& inputController) -> void
    {
        UpdateButtons();
        UpdateAxes(inputController);
        UpdateHatSwitches();
        UpdateTrackballs();
    }

    [[nodiscard]] auto Joystick::IsButtonDown(const ButtonID button) const -> bool
    {
        return m_currentButtonStates[button] && !m_previousButtonStates[button];
    }

    [[nodiscard]] auto Joystick::IsButtonPressed(const ButtonID button) const -> bool
    {
        return m_currentButtonStates[button];
    }

    [[nodiscard]] auto Joystick::IsButtonUp(const ButtonID button) const -> bool
    {
        return !m_currentButtonStates[button] && m_previousButtonStates[button];
    }

    [[nodiscard]] auto Joystick::IsAnyButtonDown(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const ButtonID button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto Joystick::IsAnyButtonPressed(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const ButtonID button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto Joystick::IsAnyButtonUp(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const ButtonID button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto Joystick::AreAllButtonsDown(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const ButtonID button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto Joystick::AreAllButtonsPressed(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const ButtonID button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto Joystick::AreAllButtonsUp(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const ButtonID button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto Joystick::AreNoButtonsDown(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const ButtonID button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto Joystick::AreNoButtonsPressed(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const ButtonID button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto Joystick::AreNoButtonsUp(const List<ButtonID>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const ButtonID button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto Joystick::IsAxisDown(const AxisID axis) const -> bool
    {
        return m_currentAxes[axis] != 0.0f && m_previousAxes[axis] == 0.0f;
    }

    [[nodiscard]] auto Joystick::IsAxisPressed(const AxisID axis) const -> bool
    {
        return m_currentAxes[axis] != 0.0f;
    }

    [[nodiscard]] auto Joystick::IsAxisUp(const AxisID axis) const -> bool
    {
        return m_currentAxes[axis] == 0.0f && m_previousAxes[axis] != 0.0f;
    }

    [[nodiscard]] auto Joystick::WasHatSwitchMoved(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction) const -> bool
    {
        if (direction.has_value())
        {
            return m_currentHatSwitches[hatSwitch] == direction.value() && m_previousHatSwitches[hatSwitch] == JoystickHatSwitchDirection::Centred;
        }
        else
        {
            return m_currentHatSwitches[hatSwitch] != JoystickHatSwitchDirection::Centred && m_previousHatSwitches[hatSwitch] == JoystickHatSwitchDirection::Centred;
        }
    }

    [[nodiscard]] auto Joystick::IsHatSwitchMoved(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction) const -> bool
    {
        if (direction.has_value())
        {
            return m_currentHatSwitches[hatSwitch] == direction.value();
        }
        else
        {
            return m_currentHatSwitches[hatSwitch] != JoystickHatSwitchDirection::Centred;
        }
    }

    [[nodiscard]] auto Joystick::WasHatSwitchReturned(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction) const -> bool
    {
        if (direction.has_value())
        {
            return m_currentHatSwitches[hatSwitch] == JoystickHatSwitchDirection::Centred && m_previousHatSwitches[hatSwitch] == direction.value();
        }
        else
        {
            return m_currentHatSwitches[hatSwitch] == JoystickHatSwitchDirection::Centred && m_previousHatSwitches[hatSwitch] != JoystickHatSwitchDirection::Centred;
        }
    }

    [[nodiscard]] auto Joystick::CanRumble() const -> bool
    {
        return SDL_JoystickHasRumble(m_handle) == SDL_TRUE;
    }

    auto Joystick::Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const -> void
    {
        const u16 convertedLowFrequency = static_cast<u16>(lowFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));
        const u16 convertedHighFrequency = static_cast<u16>(highFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));

        SDL_JoystickRumble(m_handle, convertedLowFrequency, convertedHighFrequency, milliseconds);
    }

    auto Joystick::StopRumbling() const -> void
    {
        SDL_JoystickRumble(m_handle, 0u, 0u, 0u);
    }

    [[nodiscard]] auto Joystick::CanRumbleTriggers() const -> bool
    {
        return SDL_JoystickHasRumbleTriggers(m_handle) == SDL_TRUE;
    }

    auto Joystick::RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const -> void
    {
        const u16 convertedLeftIntensity = static_cast<u16>(leftIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));
        const u16 convertedRightIntensity = static_cast<u16>(rightIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));

        SDL_JoystickRumbleTriggers(m_handle, convertedLeftIntensity, convertedRightIntensity, milliseconds);
    }

    auto Joystick::StopRumblingTriggers() const -> void
    {
        SDL_JoystickRumbleTriggers(m_handle, 0u, 0u, 0u);
    }

    [[nodiscard]] auto Joystick::HasLED() const -> bool
    {
        return SDL_JoystickHasLED(m_handle) == SDL_TRUE;
    }

    auto Joystick::SetLED(const Colour& colour) const -> void
    {
        SDL_JoystickSetLED(m_handle, colour.red, colour.green, colour.blue);
    }

    [[nodiscard]] auto Joystick::GetInstanceID() const -> InstanceID
    {
        return SDL_JoystickInstanceID(m_handle);
    }

    [[nodiscard]] auto Joystick::GetType() const noexcept -> Type
    {
        return static_cast<Type>(SDL_JoystickGetType(m_handle));
    }

    auto Joystick::Initialise() -> void
    {
        m_name = SDL_JoystickName(m_handle);
        m_guid = [this]() -> String {
            const SDL_JoystickGUID guid = SDL_JoystickGetGUID(m_handle);

            String guidString(64u, '\0');
            SDL_JoystickGetGUIDString(guid, guidString.data(), static_cast<i32>(guidString.length()));

            if (const usize firstNullTerminatorLocation = guidString.find_first_of('\0');
                firstNullTerminatorLocation != String::npos)
            {
                guidString.erase(firstNullTerminatorLocation);
            }

            return guidString;
        }();

        const usize buttonCount = static_cast<usize>(SDL_JoystickNumButtons(m_handle));
        m_currentButtonStates.resize(buttonCount, false);
        m_previousButtonStates.resize(buttonCount, false);

        const usize axisCount = static_cast<usize>(SDL_JoystickNumBalls(m_handle));
        m_currentAxes.resize(axisCount, 0.0f);
        m_previousAxes.resize(axisCount, 0.0f);

        const usize hatSwitchCount = static_cast<usize>(SDL_JoystickNumHats(m_handle));
        m_currentHatSwitches.resize(hatSwitchCount, JoystickHatSwitchDirection::Centred);
        m_previousHatSwitches.resize(hatSwitchCount, JoystickHatSwitchDirection::Centred);

        const usize trackballCount = static_cast<usize>(SDL_JoystickNumBalls(m_handle));
        m_trackballs.resize(trackballCount, Trackball{ });
    }

    auto Joystick::Destroy() noexcept -> void
    {
        if (m_handle != nullptr)
        {
            if (m_isOwning)
            {
                SDL_JoystickClose(m_handle);
            }

            m_id = 0;
            m_playerIndex = InvalidPlayerIndex;

            m_handle = nullptr;
            m_isOwning = false;

            m_name.clear();
            m_guid.clear();

            m_previousButtonStates.clear();
            m_currentButtonStates.clear();

            m_currentAxes.clear();
            m_previousAxes.clear();

            m_currentHatSwitches.clear();
            m_previousHatSwitches.clear();

            m_trackballs.clear();
        }
    }

    auto Joystick::UpdateButtons() -> void
    {
        m_previousButtonStates = std::move(m_currentButtonStates);

        for (ButtonID button = 0u; button < GetButtonCount(); ++button)
        {
            m_currentButtonStates.push_back(SDL_JoystickGetButton(m_handle, static_cast<i32>(button)) != 0u);
        }
    }

    auto Joystick::UpdateAxes(const InputController& inputController) -> void
    {
        constexpr f32 MaxAxisValue = static_cast<f32>(std::numeric_limits<i16>::max());
        const f32 unnormalisedDeadzone = inputController.GetJoystickDeadzone() * MaxAxisValue;

        for (AxisID axis = 0u; axis < GetAxisCount(); ++axis)
        {
            const i16 rawAxisValue = SDL_JoystickGetAxis(m_handle, static_cast<i32>(axis));
            const f32 normalisedAxisValue = static_cast<f32>(glm::sign(rawAxisValue)) *
                ((static_cast<f32>(glm::abs(rawAxisValue)) - unnormalisedDeadzone) / (MaxAxisValue - unnormalisedDeadzone));

            m_previousAxes[axis] = m_currentAxes[axis];
            m_currentAxes[axis] = glm::clamp(normalisedAxisValue, -1.0f, 1.0f);
        }
    }

    auto Joystick::UpdateHatSwitches() -> void
    {
        for (HatSwitchID hatSwitch = 0u; hatSwitch < GetHatSwitchCount(); ++hatSwitch)
        {
            m_previousHatSwitches[hatSwitch] = m_currentHatSwitches[hatSwitch];
            m_currentHatSwitches[hatSwitch] = static_cast<JoystickHatSwitchDirection>(SDL_JoystickGetHat(m_handle, static_cast<i32>(hatSwitch)));
        }
    }

    auto Joystick::UpdateTrackballs() -> void
    {
        for (TrackballID trackball = 0u; trackball < GetTrackballCount(); ++trackball)
        {
            i32 deltaX = 0;
            i32 deltaY = 0;
            SDL_JoystickGetBall(m_handle, static_cast<i32>(trackball), &deltaX, &deltaY);

            m_trackballs[trackball].deltaX = static_cast<f32>(deltaX);
            m_trackballs[trackball].deltaY = static_cast<f32>(deltaY);
        }
    }
}
