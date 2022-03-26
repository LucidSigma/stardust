#include "stardust/input/keyboard/VirtualKeyCodes.h"

#include "stardust/input/keyboard/KeyCodes.h"

namespace stardust
{
    [[nodiscard]] auto ConvertVirtualKeyCodeToKeyCode(const VirtualKeyCode virtualKeyCode) -> KeyCode
    {
        const SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(virtualKeyCode));

        return static_cast<KeyCode>(scancode);
    }

    [[nodiscard]] auto GetInternalVirtualKeyCodeName(const VirtualKeyCode virtualKeyCode) -> String
    {
        return SDL_GetKeyName(static_cast<SDL_Keycode>(virtualKeyCode));
    }
}
