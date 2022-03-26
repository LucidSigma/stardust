#include "stardust/input/InputController.h"

#include <utility>

#include <SDL2/SDL.h>

#include "stardust/filesystem/vfs/VirtualFilesystem.h"

namespace stardust
{
    [[nodiscard]] auto InputController::LoadGameControllerDatabase(const StringView gameControllerDatabasePath) -> Status
    {
        auto gameControllerDatabaseResult = vfs::ReadFileBytes(gameControllerDatabasePath);

        if (gameControllerDatabaseResult.is_err())
        {
            return Status::Fail;
        }

        const List<ubyte> gameControllerDatabase = std::move(gameControllerDatabaseResult).unwrap();
        SDL_RWops* const gameControllerDatabaseRWHandle = SDL_RWFromConstMem(gameControllerDatabase.data(), static_cast<i32>(gameControllerDatabase.size()));

        if (gameControllerDatabaseRWHandle == nullptr)
        {
            return Status::Fail;
        }

        if (SDL_GameControllerAddMappingsFromRW(gameControllerDatabaseRWHandle, SDL_TRUE) == -1)
        {
            return Status::Fail;
        }

        return Status::Success;
    }

    auto InputController::GenerateTouchEventsFromMouseEvents(const bool generateTouchEvents) -> void
    {
        SDL_SetHintWithPriority(SDL_HINT_MOUSE_TOUCH_EVENTS, generateTouchEvents ? "1" : "0", SDL_HINT_OVERRIDE);
    }

    auto InputController::GenerateMouseEventsFromTouchEvents(const bool generateMouseEvents) -> void
    {
        SDL_SetHintWithPriority(SDL_HINT_TOUCH_MOUSE_EVENTS, generateMouseEvents ? "1" : "0", SDL_HINT_OVERRIDE);
    }

    auto InputController::Update() -> void
    {
        m_keyboardState.Update();
        m_mouseState.Update();
        m_gameControllerLobby.Update(*this);
        m_joystickLobby.Update(*this);
    }
}
