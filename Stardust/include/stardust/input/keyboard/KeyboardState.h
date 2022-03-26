#pragma once
#ifndef STARDUST_KEYBOARD_STATE_H
#define STARDUST_KEYBOARD_STATE_H

#include <SDL2/SDL.h>

#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class KeyboardState final
    {
    private:
        List<u8> m_currentKeyStates{ List<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };
        List<u8> m_previousKeyStates{ List<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };

    public:
        auto Update() -> void;

        [[nodiscard]] auto IsKeyDown(const KeyCode key) const -> bool;
        [[nodiscard]] auto IsKeyPressed(const KeyCode key) const -> bool;
        [[nodiscard]] auto IsKeyUp(const KeyCode key) const -> bool;

        [[nodiscard]] auto IsAnyKeyDown(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto IsAnyKeyPressed(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto IsAnyKeyUp(const List<KeyCode>& keys) const -> bool;

        [[nodiscard]] auto AreAllKeysDown(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto AreAllKeysPressed(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto AreAllKeysUp(const List<KeyCode>& keys) const -> bool;

        [[nodiscard]] auto AreNoKeysDown(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto AreNoKeysPressed(const List<KeyCode>& keys) const -> bool;
        [[nodiscard]] auto AreNoKeysUp(const List<KeyCode>& keys) const -> bool;
    };
}

#endif
