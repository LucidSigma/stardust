#include "stardust/input/mouse/MouseState.h"

#include <algorithm>
#include <type_traits>

#include <SDL2/SDL.h>

namespace stardust
{
    auto MouseState::Update() -> void
    {
        m_previousScreenCoordinates = m_currentScreenCoordinates;

        m_previousButtonState = m_currentButtonState;
        m_currentButtonState = SDL_GetMouseState(&m_currentScreenCoordinates.x, &m_currentScreenCoordinates.y);
    }

    auto MouseState::ResetScrollState() noexcept -> void
    {
        m_yScrollAmount = 0;
        m_preciseYScrollAmount = 0.0f;
    }

    auto MouseState::UpdateScrollState(const i32 scrollAmount, const f32 preciseScrollAmount) noexcept -> void
    {
        m_yScrollAmount = scrollAmount;
        m_preciseYScrollAmount = preciseScrollAmount;
    }

    [[nodiscard]] auto MouseState::GetVirtualCoordintes(const Window& window, const Camera2D& camera) const noexcept -> IVector2
    {
        const Vector2 windowSize = static_cast<Vector2>(window.GetSize());

        return IVector2{
            static_cast<i32>((static_cast<f32>(m_currentScreenCoordinates.x) / windowSize.x) * static_cast<f32>(camera.GetVirtualScreenSize().x)),
            static_cast<i32>((static_cast<f32>(m_currentScreenCoordinates.y) / windowSize.y) * static_cast<f32>(camera.GetVirtualScreenSize().y)),
        };
    }

    [[nodiscard]] auto MouseState::GetPreviousVirtualCoordinates(const Window& window, const Camera2D& camera) const noexcept -> IVector2
    {
        const Vector2 windowSize = static_cast<Vector2>(window.GetSize());

        return IVector2{
            static_cast<i32>((static_cast<f32>(m_previousScreenCoordinates.x) / windowSize.x) * static_cast<f32>(camera.GetVirtualScreenSize().x)),
            static_cast<i32>((static_cast<f32>(m_previousScreenCoordinates.y) / windowSize.y) * static_cast<f32>(camera.GetVirtualScreenSize().y)),
        };
    }

    [[nodiscard]] auto MouseState::GetRelativeVirtualCoordinates(const Window& window, const Camera2D& camera) const noexcept -> IVector2
    {
        const Vector2 windowSize = static_cast<Vector2>(window.GetSize());

        return IVector2{
            static_cast<i32>((static_cast<f32>(m_currentScreenCoordinates.x - m_previousScreenCoordinates.x) / windowSize.x) * static_cast<f32>(camera.GetVirtualScreenSize().x)),
            static_cast<i32>((static_cast<f32>(m_currentScreenCoordinates.y - m_previousScreenCoordinates.y) / windowSize.y) * static_cast<f32>(camera.GetVirtualScreenSize().y)),
        };
    }

    [[nodiscard]] auto MouseState::IsButtonDown(const MouseButton button) const -> bool
    {
        const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

        return (m_currentButtonState & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue) &&
            !((m_previousButtonState & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue));
    }

    [[nodiscard]] auto MouseState::IsButtonPressed(const MouseButton button) const -> bool
    {
        const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

        return (m_currentButtonState & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
    }

    [[nodiscard]] auto MouseState::IsButtonUp(const MouseButton button) const -> bool
    {
        const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

        return !((m_currentButtonState & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue)) &&
            (m_previousButtonState & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
    }

    [[nodiscard]] auto MouseState::IsAnyButtonDown(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const MouseButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto MouseState::IsAnyButtonPressed(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const MouseButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto MouseState::IsAnyButtonUp(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const MouseButton button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto MouseState::AreAllButtonsDown(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const MouseButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto MouseState::AreAllButtonsPressed(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const MouseButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto MouseState::AreAllButtonsUp(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const MouseButton button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto MouseState::AreNoButtonsDown(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const MouseButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto MouseState::AreNoButtonsPressed(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const MouseButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto MouseState::AreNoButtonsUp(const List<MouseButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const MouseButton button)
        {
            return IsButtonUp(button);
        });
    }
}
