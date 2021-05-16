#pragma once
#ifndef STARDUST_INPUT_H
#define STARDUST_INPUT_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <limits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/keyboard/Keyboard.h"
#include "stardust/input/mouse/Mouse.h"
#include "stardust/utility/status/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    class Input
        : private INoncopyable, private INonmovable
    {
    private:
        inline static Vector<u8> s_currentKeys{ Vector<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };
        inline static Vector<u8> s_previousKeys{ Vector<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };

        inline static u32 s_currentMouseButtonStates = 0u;
        inline static u32 s_previousMouseButtonStates = 0u;

        inline static Keyboard s_keyboardState{ s_currentKeys.data(), s_previousKeys.data() };
        inline static Mouse s_mouseState{ };

        inline static bool s_isMouseCaptured = false;
        inline static bool s_isMouseInRelativeMode = false;

        inline static u32 s_maxGameControllers = std::numeric_limits<u32>::max();
        inline static HashMap<SDL_JoystickID, GameController> s_gameControllers{ };
        inline static u32 s_gameControllerDeadzone = 0u;

    public:
        [[nodiscard]] static Status InitialiseGameControllerDatabase(const StringView& controllerDatabaseFilepath);

        static bool IsMouseCaptured();
        static void CaptureMouse(const bool captureMouse);

        static bool IsMouseInRelativeMode();
        static void SetRelativeMouseMode(const bool isMouseRelative);
        static void ClearRelativeMouseState();

        static bool IsCursorShown();
        static void ShowCursor(const bool showCursor);
        static void WarpMouse(const class Window& window, const i32 x, const i32 y);

        static u32 GetMaxGameControllers() noexcept;
        static void SetMaxGameControllers(const u32 maxGameControllers) noexcept;
        static u32 GetGameControllerDeadzone() noexcept;
        static void SetGameControllerDeadzone(const u32 gameControllerDeadzone) noexcept;

        static ObserverPtr<GameController> AddGameController(const i32 id, const class Locale& locale);
        static void RemoveGameController(const SDL_JoystickID instanceID);
        static void RemoveAllGameControllers();
        static usize GetGameControllerCount();

        static void UpdateKeyboardState();
        static void UpdateMouseState();
        static void ResetScrollState() noexcept;
        static void UpdateScrollState(const i32 scrollAmount) noexcept;
        static void UpdateGameControllers();

        static const Keyboard& GetKeyboardState();
        static const Mouse& GetMouseState();

        [[nodiscard]] static bool DoesGameControllerExist(const SDL_JoystickID instanceID);
        [[nodiscard]] static ObserverPtr<GameController> GetGameController(const SDL_JoystickID instanceID);
        [[nodiscard]] static ObserverPtr<GameController> GetGameControllerByPlayerIndex(const u32 playerIndex);
        [[nodiscard]] static const HashMap<SDL_JoystickID, GameController>& GetGameControllers();

        Input() = delete;
        ~Input() noexcept = delete;
    };
}

#endif