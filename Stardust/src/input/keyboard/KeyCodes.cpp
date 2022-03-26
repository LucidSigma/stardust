#include "stardust/input/keyboard/KeyCodes.h"

namespace stardust
{
    [[nodiscard]] auto ConvertKeyCodeToVirtualKeyCode(const KeyCode keyCode) -> VirtualKeyCode
    {
        const SDL_Keycode virtualKeyCode = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(keyCode));

        return static_cast<VirtualKeyCode>(virtualKeyCode);
    }

    [[nodiscard]] auto GetInternalKeyCodeName(const KeyCode keyCode) -> String
    {
        return SDL_GetScancodeName(static_cast<SDL_Scancode>(keyCode));
    }
}
