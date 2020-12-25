#pragma once
#ifndef STARDUST_MOUSE_H
#define STARDUST_MOUSE_H

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/mouse/MouseButtonCodes.h"

namespace stardust
{
	class Mouse
	{
	private:
		u32 m_currentButtonStates;
		u32 m_previousButtonStates;

		f32 m_x = 0.0f;
		f32 m_y = 0.0f;

		f32 m_relativeX;
		f32 m_relativeY;

	public:
		friend class Input;

		Mouse() = default;
		~Mouse() noexcept = default;

		[[nodiscard]] inline Vec2 GetCoordinates() const noexcept { return Vec2{ m_x, m_y }; }
		[[nodiscard]] inline Vec2 GetProportionalCoordinates(const Renderer& renderer) const noexcept { return Vec2{ m_x, m_y } / renderer.GetVirtualScale(); }

		[[nodiscard]] inline Vec2 GetRelativeCoordinates() const noexcept { return Vec2{ m_relativeX, m_relativeY }; }
		[[nodiscard]] inline Vec2 GetProportionalRelativeCoordinates(const Renderer& renderer) const noexcept { return Vec2{ m_relativeX, m_relativeY } / renderer.GetVirtualScale(); }

		[[nodiscard]] bool IsButtonDown(const MouseButton button) const;
		[[nodiscard]] bool IsButtonPressed(const MouseButton button) const;
		[[nodiscard]] bool IsButtonUp(const MouseButton button) const;

		[[nodiscard]] bool IsAnyButtonDown(const Vector<MouseButton>& buttons) const;
		[[nodiscard]] bool IsAnyButtonPressed(const Vector<MouseButton>& buttons) const;
		[[nodiscard]] bool IsAnyButtonUp(const Vector<MouseButton>& buttons) const;

		[[nodiscard]] bool AreAllButtonsDown(const Vector<MouseButton>& buttons) const;
		[[nodiscard]] bool AreAllButtonsPressed(const Vector<MouseButton>& buttons) const;
		[[nodiscard]] bool AreAllButtonsUp(const Vector<MouseButton>& buttons) const;
	};
}

#endif