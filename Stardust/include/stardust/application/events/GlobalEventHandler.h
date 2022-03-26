#pragma once
#ifndef STARDUST_GLOBAL_EVENT_HANDLER_H
#define STARDUST_GLOBAL_EVENT_HANDLER_H

#include "stardust/application/events/Events.h"

namespace stardust
{
    class GlobalEventHandler
    {
    public:
        GlobalEventHandler() = default;
        virtual ~GlobalEventHandler() noexcept = default;

        inline virtual auto OnKeyDown(class Application& application, const events::KeyDown& event) -> void { }
        inline virtual auto OnKeyUp(class Application& application, const events::KeyUp& event) -> void { }
        inline virtual auto OnTextInput(class Application& application, const events::TextInput& event) -> void { }

        inline virtual auto OnMouseButtonDown(class Application& application, const events::MouseButtonDown& event) -> void { }
        inline virtual auto OnMouseButtonUp(class Application& application, const events::MouseButtonUp& event) -> void { }
        inline virtual auto OnMouseMotion(class Application& application, const events::MouseMotion& event) -> void { }
        inline virtual auto OnMouseScroll(class Application& application, const events::MouseScroll& event) -> void { }

        inline virtual auto OnGameControllerAdded(class Application& application, const events::GameControllerAdded& event) -> void { }
        inline virtual auto OnGameControllerRemoved(class Application& application, const events::GameControllerRemoved& event) -> void { }
        inline virtual auto OnGameControllerButtonDown(class Application& application, const events::GameControllerButtonDown& event) -> void { }
        inline virtual auto OnGameControllerButtonUp(class Application& application, const events::GameControllerButtonUp& event) -> void { }
        inline virtual auto OnGameControllerAxisMotion(class Application& application, const events::GameControllerAxisMotion& event) -> void { }
        inline virtual auto OnGameControllerTouchpadFingerDown(class Application& application, const events::GameControllerTouchpadFingerDown& event) -> void { }
        inline virtual auto OnGameControllerTouchpadFingerUp(class Application& application, const events::GameControllerTouchpadFingerUp& event) -> void { }
        inline virtual auto OnGameControllerTouchpadFingerMotion(class Application& application, const events::GameControllerTouchpadFingerMotion& event) -> void { }
        inline virtual auto OnGameControllerAccelerometerUpdate(class Application& application, const events::GameControllerAccelerometerUpdate& event) -> void { }
        inline virtual auto OnGameControllerGyroscopeUpdate(class Application& application, const events::GameControllerGyroscopeUpdate& event) -> void { }

        inline virtual auto OnJoystickAdded(class Application& application, const events::JoystickAdded& event) -> void { }
        inline virtual auto OnJoystickRemoved(class Application& application, const events::JoystickRemoved& event) -> void { }
        inline virtual auto OnJoystickButtonDown(class Application& application, const events::JoystickButtonDown& event) -> void { }
        inline virtual auto OnJoystickButtonUp(class Application& application, const events::JoystickButtonUp& event) -> void { }
        inline virtual auto OnJoystickAxisMotion(class Application& application, const events::JoystickAxisMotion& event) -> void { }
        inline virtual auto OnJoystickHatSwitchMotion(class Application& application, const events::JoystickHatSwitchMotion& event) -> void { }
        inline virtual auto OnJoystickTrackballMotion(class Application& application, const events::JoystickTrackballMotion& event) -> void { }

        inline virtual auto OnTouchFingerDown(class Application& application, const events::TouchFingerDown& event) -> void { }
        inline virtual auto OnTouchFingerUp(class Application& application, const events::TouchFingerUp& event) -> void { }
        inline virtual auto OnTouchFingerMotion(class Application& application, const events::TouchFingerMotion& event) -> void { }
        inline virtual auto OnMultiTouchFingerGesture(class Application& application, const events::MultiTouchFingerGesture& event) -> void { }
        inline virtual auto OnDollarGesturePerformed(class Application& application, const events::DollarGesturePerformed& event) -> void { }
        inline virtual auto OnDollarGestureRecorded(class Application& application, const events::DollarGestureRecorded& event) -> void { }

        inline virtual auto OnWindowShown(class Application& application, const events::WindowShown& event) -> void { }
        inline virtual auto OnWindowHidden(class Application& application, const events::WindowHidden& event) -> void { }
        inline virtual auto OnWindowMoved(class Application& application, const events::WindowMoved& event) -> void { }
        inline virtual auto OnWindowResized(class Application& application, const events::WindowResized& event) -> void { }
        inline virtual auto OnWindowMinimised(class Application& application, const events::WindowMinimised& event) -> void { }
        inline virtual auto OnWindowMaximised(class Application& application, const events::WindowMaximised& event) -> void { }
        inline virtual auto OnWindowRestored(class Application& application, const events::WindowRestored& event) -> void { }
        inline virtual auto OnWindowMouseEnter(class Application& application, const events::WindowMouseEnter& event) -> void { }
        inline virtual auto OnWindowMouseExit(class Application& application, const events::WindowMouseExit& event) -> void { }
        inline virtual auto OnWindowFocusGained(class Application& application, const events::WindowFocusGained& event) -> void { }
        inline virtual auto OnWindowFocusLost(class Application& application, const events::WindowFocusLost& event) -> void { }

        inline virtual auto OnLocaleChanged(class Application& application, const events::LocaleChanged& event) -> void { }

        inline virtual auto OnUserEvent(class Application& application, const events::UserEvent& event) -> void { }
    };
}

#endif
