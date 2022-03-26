#pragma once
#ifndef STARDUST_MOUSE_STATE_H
#define STARDUST_MOUSE_STATE_H

#include "stardust/camera/Camera2D.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"
#include "stardust/window/Window.h"

namespace stardust
{
    class MouseState final
    {
    private:
        u32 m_currentButtonState = 0u;
        u32 m_previousButtonState = 0u;

        IVector2 m_currentScreenCoordinates = IVector2Zero;
        IVector2 m_previousScreenCoordinates = IVector2Zero;

        i32 m_yScrollAmount = 0;
        f32 m_preciseYScrollAmount = 0.0f;

    public:
        auto Update() -> void;
        auto ResetScrollState() noexcept -> void;
        auto UpdateScrollState(const i32 scrollAmount, const f32 preciseScrollAmount) noexcept -> void;

        [[nodiscard]] inline auto GetCoordinates() const noexcept -> IVector2 { return m_currentScreenCoordinates; }
        [[nodiscard]] auto GetVirtualCoordintes(const Window& window, const Camera2D& camera) const noexcept -> IVector2;

        [[nodiscard]] inline auto GetPreviousCoordinates() const noexcept -> IVector2 { return m_previousScreenCoordinates; }
        [[nodiscard]] auto GetPreviousVirtualCoordinates(const Window& window, const Camera2D& camera) const noexcept -> IVector2;

        [[nodiscard]] inline auto GetRelativeCoordinates() const noexcept -> IVector2 { return m_currentScreenCoordinates - m_previousScreenCoordinates; }
        [[nodiscard]] auto GetRelativeVirtualCoordinates(const Window& window, const Camera2D& camera) const noexcept -> IVector2;

        [[nodiscard]] inline auto HasMoved() const noexcept -> bool { return GetRelativeCoordinates() != IVector2Zero; }

        [[nodiscard]] auto IsButtonDown(const MouseButton button) const -> bool;
        [[nodiscard]] auto IsButtonPressed(const MouseButton button) const -> bool;
        [[nodiscard]] auto IsButtonUp(const MouseButton button) const -> bool;

        [[nodiscard]] auto IsAnyButtonDown(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonPressed(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonUp(const List<MouseButton>& buttons) const -> bool;

        [[nodiscard]] auto AreAllButtonsDown(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsPressed(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsUp(const List<MouseButton>& buttons) const -> bool;

        [[nodiscard]] auto AreNoButtonsDown(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsPressed(const List<MouseButton>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsUp(const List<MouseButton>& buttons) const -> bool;

        [[nodiscard]] inline auto GetScrollAmount() const noexcept -> i32 { return m_yScrollAmount; }
        [[nodiscard]] inline auto GetPreciseScrollAmount() const noexcept -> f32 { return m_preciseYScrollAmount; }
    };
}

#endif
