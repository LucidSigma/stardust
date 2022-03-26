#include "stardust/input/keyboard/KeyboardState.h"

#include <algorithm>
#include <utility>

namespace stardust
{
    auto KeyboardState::Update() -> void
    {
        m_previousKeyStates = std::move(m_currentKeyStates);

        const u8* const keyState = SDL_GetKeyboardState(nullptr);
        m_currentKeyStates = std::move(List<u8>(keyState, keyState + SDL_NUM_SCANCODES));
    }

    [[nodiscard]] auto KeyboardState::IsKeyDown(const KeyCode key) const -> bool
    {
        return m_currentKeyStates[static_cast<SDL_Scancode>(key)] && !m_previousKeyStates[static_cast<SDL_Scancode>(key)];
    }

    [[nodiscard]] auto KeyboardState::IsKeyPressed(const KeyCode key) const -> bool
    {
        return m_currentKeyStates[static_cast<SDL_Scancode>(key)];
    }

    [[nodiscard]] auto KeyboardState::IsKeyUp(const KeyCode key) const -> bool
    {
        return !m_currentKeyStates[static_cast<SDL_Scancode>(key)] && m_previousKeyStates[static_cast<SDL_Scancode>(key)];;
    }

    [[nodiscard]] auto KeyboardState::IsAnyKeyDown(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::any_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyDown(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::IsAnyKeyPressed(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::any_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyPressed(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::IsAnyKeyUp(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::any_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyUp(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreAllKeysDown(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::all_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyDown(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreAllKeysPressed(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::all_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyPressed(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreAllKeysUp(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::all_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyUp(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreNoKeysDown(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::none_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyDown(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreNoKeysPressed(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::none_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyPressed(keycode);
        });
    }

    [[nodiscard]] auto KeyboardState::AreNoKeysUp(const List<KeyCode>& keys) const -> bool
    {
        return std::ranges::none_of(keys, [this](const KeyCode keycode)
        {
            return IsKeyUp(keycode);
        });
    }
}
