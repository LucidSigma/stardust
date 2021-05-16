#include "stardust/input/keyboard/Keyboard.h"

#include <algorithm>

#include <SDL2/SDL.h>

namespace stardust
{
    Keyboard::Keyboard(const u8* currentKeys, const u8* previousKeys)
        : m_currentKeys(currentKeys), m_previousKeys(previousKeys)
    { }

    [[nodiscard]] bool Keyboard::IsKeyDown(const KeyCode key) const
    {
        return m_currentKeys[static_cast<SDL_Scancode>(key)] && !m_previousKeys[static_cast<SDL_Scancode>(key)];
    }

    [[nodiscard]] bool Keyboard::IsKeyPressed(const KeyCode key) const
    {
        return m_currentKeys[static_cast<SDL_Scancode>(key)];
    }

    [[nodiscard]] bool Keyboard::IsKeyUp(const KeyCode key) const
    {
        return !m_currentKeys[static_cast<SDL_Scancode>(key)] && m_previousKeys[static_cast<SDL_Scancode>(key)];;
    }

    [[nodiscard]] bool Keyboard::IsAnyKeyDown(const Vector<KeyCode>& keys) const
    {
        return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyDown(keycode);
        });
    }

    [[nodiscard]] bool Keyboard::IsAnyKeyPressed(const Vector<KeyCode>& keys) const
    {
        return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyPressed(keycode);
        });
    }

    [[nodiscard]] bool Keyboard::IsAnyKeyUp(const Vector<KeyCode>& keys) const
    {
        return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyUp(keycode);
        });
    }

    [[nodiscard]] bool Keyboard::AreAllKeysDown(const Vector<KeyCode>& keys) const
    {
        return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyDown(keycode);
        });
    }

    [[nodiscard]] bool Keyboard::AreAllKeysPressed(const Vector<KeyCode>& keys) const
    {
        return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyPressed(keycode);
        });
    }

    [[nodiscard]] bool Keyboard::AreAllKeysUp(const Vector<KeyCode>& keys) const
    {
        return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
        {
            return IsKeyUp(keycode);
        });
    }
}