#pragma once
#ifndef STARDUST_KEY_MOD_STATE_H
#define STARDUST_KEY_MOD_STATE_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class KeyModState
        : std::underlying_type_t<SDL_Keymod>
    {
        None = KMOD_NONE,
        LeftShift = KMOD_LSHIFT,
        RightShift = KMOD_RSHIFT,
        Shift = KMOD_SHIFT,
        LeftControl = KMOD_LCTRL,
        RightControl = KMOD_RCTRL,
        Control = KMOD_CTRL,
        LeftAlt = KMOD_LALT,
        RightAlt = KMOD_RALT,
        Alt = KMOD_ALT,
        AltGr = KMOD_MODE,
        LeftGUI = KMOD_LGUI,
        RightGUI = KMOD_RGUI,
        GUI = KMOD_GUI,
        NumLock = KMOD_NUM,
        CapsLock = KMOD_CAPS,
        ScrollLock = KMOD_SCROLL,
    };

    [[nodiscard]] extern auto IsKeyModStateActive(const KeyModState keyModState) -> bool;
    [[nodiscard]] extern auto IsAnyKeyModStateActive(const List<KeyModState>& keyModStates) -> bool;
    [[nodiscard]] extern auto AreAllKeyModStatesActive(const List<KeyModState>& keyModStates) -> bool;
    [[nodiscard]] extern auto AreNoKeyModStatesActive(const List<KeyModState>& keyModStates) -> bool;

    [[nodiscard]] extern auto ContainsKeyModState(const u32 modStateBitmap, const KeyModState keyModState) -> bool;
}

#endif