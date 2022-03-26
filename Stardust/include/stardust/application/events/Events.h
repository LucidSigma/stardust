#pragma once
#ifndef STARDUST_EVENTS_H
#define STARDUST_EVENTS_H

#include "stardust/input/controller/GameController.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/keyboard/VirtualKeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/input/touch/TouchDevice.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/window/Window.h"

namespace stardust
{
    enum class EventStatus
    {
        NotHandled,
        Handled,
    };

    namespace events
    {
        struct KeyDown final
        {
            KeyCode keyCode;
            VirtualKeyCode virtualKeyCode;

            u32 modState;

            bool isRepeat;
        };

        struct KeyUp final
        {
            KeyCode keyCode;
            VirtualKeyCode virtualKeyCode;

            u32 modState;

            bool isRepeat;
        };

        struct TextInput final
        {
            String text;
        };

        struct MouseButtonDown final
        {
            MouseButton mouseButton;
            IVector2 coordinates;

            u32 clickCount;
        };

        struct MouseButtonUp final
        {
            MouseButton mouseButton;
            IVector2 coordinates;

            u32 clickCount;
        };

        struct MouseMotion final
        {
            IVector2 coordinates;
            IVector2 relativeCoordinates;
        };

        struct MouseScroll final
        {
            i32 scrollAmount;
            f32 preciseScrollAmount;
        };

        struct GameControllerAdded final
        {
            ObserverPointer<GameController> gameController;
        };

        struct GameControllerRemoved final
        {
            ObserverPointer<const GameController> gameController;
        };

        struct GameControllerButtonDown final
        {
            ObserverPointer<const GameController> gameController;
            GameControllerButton button;
        };

        struct GameControllerButtonUp final
        {
            ObserverPointer<const GameController> gameController;
            GameControllerButton button;
        };

        struct GameControllerAxisMotion final
        {
            ObserverPointer<const GameController> gameController;

            GameControllerAxis axis;
            f32 value;
        };

        struct GameControllerTouchpadFingerDown final
        {
            ObserverPointer<const GameController> gameController;

            usize fingerIndex;
            Vector2 position;
            f32 pressure;
        };

        struct GameControllerTouchpadFingerUp final
        {
            ObserverPointer<const GameController> gameController;

            usize fingerIndex;
            Vector2 position;
        };

        struct GameControllerTouchpadFingerMotion final
        {
            ObserverPointer<const GameController> gameController;

            usize fingerIndex;
            Vector2 position;
            f32 pressure;
        };

        struct GameControllerAccelerometerUpdate final
        {
            ObserverPointer<const GameController> gameController;
            Vector3 acceleration;
        };

        struct GameControllerGyroscopeUpdate final
        {
            ObserverPointer<const GameController> gameController;
            Vector3 rotation;
        };

        struct JoystickAdded final
        {
            ObserverPointer<Joystick> joystick;
        };

        struct JoystickRemoved final
        {
            ObserverPointer<const Joystick> joystick;
        };

        struct JoystickButtonDown final
        {
            ObserverPointer<const Joystick> joystick;

            Joystick::ButtonID button;
        };

        struct JoystickButtonUp final
        {
            ObserverPointer<const Joystick> joystick;

            Joystick::ButtonID button;
        };

        struct JoystickAxisMotion final
        {
            ObserverPointer<const Joystick> joystick;

            Joystick::AxisID axis;
            f32 value;
        };

        struct JoystickHatSwitchMotion final
        {
            ObserverPointer<const Joystick> joystick;

            Joystick::HatSwitchID hatSwitch;
            JoystickHatSwitchDirection direction;
        };

        struct JoystickTrackballMotion final
        {
            ObserverPointer<const Joystick> joystick;

            Joystick::TrackballID trackball;
            Vector2 relativeMotion;
        };

        struct TouchFingerDown final
        {
            TouchDevice touchDevice;
            TouchDevice::FingerID fingerID;

            Vector2 position;
            f32 pressure;

            bool isOnGameWindow;
        };

        struct TouchFingerUp final
        {
            TouchDevice touchDevice;
            TouchDevice::FingerID fingerID;

            Vector2 position;

            bool isOnGameWindow;
        };

        struct TouchFingerMotion final
        {
            TouchDevice touchDevice;
            TouchDevice::FingerID fingerID;

            Vector2 position;
            Vector2 deltaPosition;
            f32 pressure;

            bool isOnGameWindow;
        };

        struct MultiTouchFingerGesture final
        {
            TouchDevice touchDevice;

            u32 fingerCount;
            Vector2 normalisedCentre;

            f32 pinchAmount;
            f32 rotationAmount;
        };

        struct DollarGesturePerformed final
        {
            TouchDevice touchDevice;
            TouchDevice::GestureID gestureID;

            u32 fingerCount;
            Vector2 normalisedCentre;

            f32 errorAmount;
        };

        struct DollarGestureRecorded final
        {
            TouchDevice touchDevice;
            TouchDevice::GestureID gestureID;
        };

        struct WindowShown final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowHidden final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowMoved final
        {
            ObserverPointer<const Window> window;
            IVector2 newPosition;
        };

        struct WindowResized final
        {
            ObserverPointer<const Window> window;
            UVector2 newSize;
        };

        struct WindowMinimised final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowMaximised final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowRestored final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowMouseEnter final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowMouseExit final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowFocusGained final
        {
            ObserverPointer<const Window> window;
        };

        struct WindowFocusLost final
        {
            ObserverPointer<const Window> window;
        };

        struct LocaleChanged final
        {
            String newLocaleName;
        };

        struct UserEvent final
        {
            i32 code;
            Optional<Any> userData;
        };
    }
}

#endif
