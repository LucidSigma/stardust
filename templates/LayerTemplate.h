#pragma once
#ifndef LAYER_TEMPLATE_H
#define LAYER_TEMPLATE_H

#include <stardust/Stardust.h>

class LayerTemplate final
    : public sd::Layer
{
public:
    LayerTemplate(sd::Scene& scene, const sd::StringView name);
    virtual ~LayerTemplate() noexcept override = default;

    [[nodiscard]] virtual auto OnAttach() -> sd::Status override { return sd::Status::Success; }
    [[nodiscard]] virtual auto OnPostAttach() -> sd::Status override { return sd::Status::Success; }
    virtual auto OnDetach() noexcept -> void override { }

    virtual auto FixedUpdate(const sd::f32 fixedDeltaTime) -> void override { }
    virtual auto ProcessInput(const sd::InputController& inputController, const sd::InputManager& inputManager) -> void override { }
    virtual auto PreUpdate(const sd::f32 deltaTime) -> void override { }
    virtual auto Update(const sd::f32 deltaTime) -> void override { }
    virtual auto PostUpdate(const sd::f32 deltaTime) -> void override { }

    virtual auto Render(sd::graphics::Renderer& renderer) -> void override { }

    virtual auto OnKeyDown(const sd::events::KeyDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnKeyUp(const sd::events::KeyUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnTextInput(const sd::events::TextInput& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnMouseButtonDown(const sd::events::MouseButtonDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnMouseButtonUp(const sd::events::MouseButtonUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnMouseMotion(const sd::events::MouseMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnMouseScroll(const sd::events::MouseScroll& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnGameControllerAdded(const sd::events::GameControllerAdded& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerRemoved(const sd::events::GameControllerRemoved& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerButtonDown(const sd::events::GameControllerButtonDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerButtonUp(const sd::events::GameControllerButtonUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerAxisMotion(const sd::events::GameControllerAxisMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerTouchpadFingerDown(const sd::events::GameControllerTouchpadFingerDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerTouchpadFingerUp(const sd::events::GameControllerTouchpadFingerUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerTouchpadFingerMotion(const sd::events::GameControllerTouchpadFingerMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerAccelerometerUpdate(const sd::events::GameControllerAccelerometerUpdate& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnGameControllerGyroscopeUpdate(const sd::events::GameControllerGyroscopeUpdate& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnJoystickAdded(const sd::events::JoystickAdded& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickRemoved(const sd::events::JoystickRemoved& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickButtonDown(const sd::events::JoystickButtonDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickButtonUp(const sd::events::JoystickButtonUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickAxisMotion(const sd::events::JoystickAxisMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickHatSwitchMotion(const sd::events::JoystickHatSwitchMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnJoystickTrackballMotion(const sd::events::JoystickTrackballMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnTouchFingerDown(const sd::events::TouchFingerDown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnTouchFingerUp(const sd::events::TouchFingerUp& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnTouchFingerMotion(const sd::events::TouchFingerMotion& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnMultiTouchFingerGesture(const sd::events::MultiTouchFingerGesture& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnDollarGesturePerformed(const sd::events::DollarGesturePerformed& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnDollarGestureRecorded(const sd::events::DollarGestureRecorded& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnWindowShown(const sd::events::WindowShown& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowHidden(const sd::events::WindowHidden& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowMoved(const sd::events::WindowMoved& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowResized(const sd::events::WindowResized& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowMinimised(const sd::events::WindowMinimised& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowMaximised(const sd::events::WindowMaximised& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowRestored(const sd::events::WindowRestored& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowMouseEnter(const sd::events::WindowMouseEnter& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowMouseExit(const sd::events::WindowMouseExit& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowFocusGained(const sd::events::WindowFocusGained& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
    virtual auto OnWindowFocusLost(const sd::events::WindowFocusLost& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnLocaleChanged(const sd::events::LocaleChanged& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }

    virtual auto OnUserEvent(const sd::events::UserEvent& event) -> sd::EventStatus override { return sd::EventStatus::NotHandled; }
};

#endif
