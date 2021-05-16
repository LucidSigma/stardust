#pragma once
#ifndef STARDUST_TEXT_INPUT_H
#define STARDUST_TEXT_INPUT_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"

namespace stardust
{
    namespace text_input
    {
        enum class ModState
            : std::underlying_type_t<SDL_Keymod>
        {
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
        };

        extern void Start();
        extern void Stop();

        [[nodiscard]] extern bool IsActive();

        [[nodiscard]] extern bool IsModStateActive(const ModState modState);
        [[nodiscard]] extern bool IsAnyModStateActive(const Vector<ModState>& modStates);
        [[nodiscard]] extern bool AreAllModStatesActive(const Vector<ModState>& modStates);
    }

    namespace ti = text_input;
}

#endif