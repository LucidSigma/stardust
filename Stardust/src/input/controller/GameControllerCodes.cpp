#include "stardust/input/controller/GameControllerCodes.h"

namespace stardust
{
    [[nodiscard]] auto GetInternalGameControllerButtonName(const GameControllerButton button) -> String
    {
        return SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(button));
    }

    [[nodiscard]] auto GetInternalGameControllerTriggerName(const GameControllerTrigger trigger) -> String
    {
        return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(trigger));
    }

    [[nodiscard]] auto GetInternalGameControllerAxisName(const GameControllerAxis axis) -> String
    {
        return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
    }
}
