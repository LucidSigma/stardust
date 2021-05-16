#pragma once
#ifndef STARDUST_EVENTS_H
#define STARDUST_EVENTS_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Types.h"

namespace stardust
{
    using Event = SDL_Event;
    using WindowEvent = SDL_WindowEvent;
    using DisplayEvent = SDL_DisplayEvent;

    enum class EventType
        : std::underlying_type_t<SDL_EventType>
    {
        Quit = SDL_QUIT,
        LocaleChange = SDL_LOCALECHANGED,

        Display = SDL_DISPLAYEVENT,
        Window = SDL_WINDOWEVENT,
        InternalWindow = SDL_SYSWMEVENT,

        KeyDown = SDL_KEYDOWN,
        KeyUp = SDL_KEYUP,
        TextEdit = SDL_TEXTEDITING,
        TextInput = SDL_TEXTINPUT,
        KeymapChange = SDL_KEYMAPCHANGED,

        MouseMotion = SDL_MOUSEMOTION,
        MouseButtonDown = SDL_MOUSEBUTTONDOWN,
        MouseButtonUp = SDL_MOUSEBUTTONUP,
        MouseScroll = SDL_MOUSEWHEEL,

        JoystickAxisMotion = SDL_JOYAXISMOTION,
        JoystickBallMotion = SDL_JOYBALLMOTION,
        JoystickHatMotion = SDL_JOYHATMOTION,
        JoystickButtonDown = SDL_JOYBUTTONDOWN,
        JoystickButtonUp = SDL_JOYBUTTONUP,
        JoystickDeviceAdd = SDL_JOYDEVICEADDED,
        JoystickDeviceRemove = SDL_JOYDEVICEREMOVED,

        GameControllerAxisMotion = SDL_CONTROLLERAXISMOTION,
        GameControllerButtonDown = SDL_CONTROLLERBUTTONDOWN,
        GameControllerButtonUp = SDL_CONTROLLERBUTTONUP,
        GameControllerDeviceAdd = SDL_CONTROLLERDEVICEADDED,
        GameControllerDeviceRemove = SDL_CONTROLLERDEVICEREMOVED,
        GameControllerDeviceRemap = SDL_CONTROLLERDEVICEREMAPPED,
        GameControllerTouchpadTouch = SDL_CONTROLLERTOUCHPADDOWN,
        GameControllerTouchpadMotion = SDL_CONTROLLERTOUCHPADMOTION,
        GameControllerTouchpadRelease = SDL_CONTROLLERTOUCHPADUP,
        GameControllerSensorUpdate = SDL_CONTROLLERSENSORUPDATE,

        ClipboardUpdate = SDL_CLIPBOARDUPDATE,

        DropFile = SDL_DROPFILE,
        DropText = SDL_DROPTEXT,
        DropBegin = SDL_DROPBEGIN,
        DropComplete = SDL_DROPCOMPLETE,

        AudioDeviceAdd = SDL_AUDIODEVICEADDED,
        AudioDeviceRemove = SDL_AUDIODEVICEREMOVED,
    };

    enum class WindowEventType
        : std::underlying_type_t<SDL_WindowEventID>
    {
        Show = SDL_WINDOWEVENT_SHOWN,
        Hide = SDL_WINDOWEVENT_HIDDEN,
        Expose = SDL_WINDOWEVENT_EXPOSED,

        Moved = SDL_WINDOWEVENT_MOVED,
        ExternalResize = SDL_WINDOWEVENT_RESIZED,
        SizeChange = SDL_WINDOWEVENT_SIZE_CHANGED,

        Minimise = SDL_WINDOWEVENT_MINIMIZED,
        Maximise = SDL_WINDOWEVENT_MAXIMIZED,
        Restore = SDL_WINDOWEVENT_RESTORED,

        MouseEnter = SDL_WINDOWEVENT_ENTER,
        MouseLeave = SDL_WINDOWEVENT_LEAVE,
        KeyboardFocusGain = SDL_WINDOWEVENT_FOCUS_GAINED,
        KeyboardFocusLoss = SDL_WINDOWEVENT_FOCUS_LOST,

        Close = SDL_WINDOWEVENT_CLOSE,

        TakeFocus = SDL_WINDOWEVENT_TAKE_FOCUS,
        HitTest = SDL_WINDOWEVENT_HIT_TEST,
    };

    enum class DisplayEventType
        : std::underlying_type_t<SDL_DisplayEventID>
    {
        OrientationChange = SDL_DISPLAYEVENT_ORIENTATION,

        Connect = SDL_DISPLAYEVENT_CONNECTED,
        Disconnect = SDL_DISPLAYEVENT_DISCONNECTED,
    };

    [[nodiscard]] inline EventType GetEventType(const Event& event) noexcept { return static_cast<EventType>(event.type); }
    [[nodiscard]] inline WindowEventType GetWindowEventType(const WindowEvent& windowEvent) noexcept { return static_cast<WindowEventType>(windowEvent.event); }
    [[nodiscard]] inline DisplayEventType GetDisplayEventType(const DisplayEvent& displayEvent) noexcept { return static_cast<DisplayEventType>(displayEvent.event); }

    [[nodiscard]] extern bool IsEventEnqueued(const EventType eventType) noexcept;

    [[nodiscard]] extern bool IsEventTypeEnabled(const EventType eventType) noexcept;
    extern void SetEventState(const EventType eventType, const bool isEnabled) noexcept;

    extern Event WaitForEvent();
    extern Optional<Event> WaitForEvent(const u32 millisecondTimeout = 0u);
}

#endif