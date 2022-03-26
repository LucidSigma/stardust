#include "stardust/input/keyboard/KeyModState.h"

#include <algorithm>

namespace stardust
{
    [[nodiscard]] auto IsKeyModStateActive(const KeyModState keyModState) -> bool
    {
        return (SDL_GetModState() & static_cast<SDL_Keymod>(keyModState)) == static_cast<SDL_Keymod>(keyModState);
    }

    [[nodiscard]] auto IsAnyKeyModStateActive(const List<KeyModState>& keyModStates) -> bool
    {
        return std::ranges::any_of(
            keyModStates,
            [](const KeyModState keyModState) -> bool
            {
                return IsKeyModStateActive(keyModState);
            }
        );
    }

    [[nodiscard]] auto AreAllKeyModStatesActive(const List<KeyModState>& keyModStates) -> bool
    {
        return std::ranges::all_of(
            keyModStates,
            [](const KeyModState keyModState) -> bool
            {
                return IsKeyModStateActive(keyModState);
            }
        );
    }

    [[nodiscard]] auto AreNoKeyModStatesActive(const List<KeyModState>& keyModStates) -> bool
    {
        return std::ranges::none_of(
            keyModStates,
            [](const KeyModState keyModState) -> bool
            {
                return IsKeyModStateActive(keyModState);
            }
        );
    }

    [[nodiscard]] auto ContainsKeyModState(const u32 modStateBitmap, const KeyModState keyModState) -> bool
    {
        return (modStateBitmap & static_cast<u32>(keyModState)) == static_cast<u32>(keyModState);
    }
}
