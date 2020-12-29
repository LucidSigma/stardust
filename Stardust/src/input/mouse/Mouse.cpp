#include "stardust/input/mouse/Mouse.h"

#include <algorithm>

#include <SDL2/SDL.h>

namespace stardust
{
	[[nodiscard]] Vec2 Mouse::GetProportionalCoordinates(const Renderer& renderer) const noexcept
	{
		const auto [topLeft, size] = renderer.GetViewportRect();

		return Vec2{
			(m_x / size.x) * renderer.GetVirtualSize().x - topLeft.x / renderer.GetVirtualScale().y,
			(m_y / size.y) * renderer.GetVirtualSize().y - topLeft.y / renderer.GetVirtualScale().x,
		};
	}

	[[nodiscard]] Vec2 Mouse::GetProportionalRelativeCoordinates(const Renderer& renderer) const noexcept
	{
		const auto [topLeft, size] = renderer.GetViewportRect();

		return Vec2{
			(m_relativeX / size.x) * renderer.GetVirtualSize().x - topLeft.x / renderer.GetVirtualScale().y,
			(m_relativeY / size.y) * renderer.GetVirtualSize().y - topLeft.y / renderer.GetVirtualScale().x,
		};
	}

	[[nodiscard]] bool Mouse::IsButtonDown(const MouseButton button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

		return (m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue) && !((m_previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue));
	}

	[[nodiscard]] bool Mouse::IsButtonPressed(const MouseButton button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

		return (m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
	}

	[[nodiscard]] bool Mouse::IsButtonUp(const MouseButton button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

		return !((m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue)) && (m_previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
	}

	[[nodiscard]] bool Mouse::IsAnyButtonDown(const Vector<MouseButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Mouse::IsAnyButtonPressed(const Vector<MouseButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Mouse::IsAnyButtonUp(const Vector<MouseButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonUp(button);
		});
	}

	[[nodiscard]] bool Mouse::AreAllButtonsDown(const Vector<MouseButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Mouse::AreAllButtonsPressed(const Vector<MouseButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Mouse::AreAllButtonsUp(const Vector<MouseButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const MouseButton button)
		{
			return IsButtonUp(button);
		});
	}
}