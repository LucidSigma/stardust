#pragma once
#ifndef STARDUST_KEY_CODES_H
#define STARDUST_KEY_CODES_H

#include <type_traits>

; // Temporary compiler bug workaround.
#include <magic_enum/magic_enum.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    enum class KeyCode
        : std::underlying_type_t<SDL_Scancode>
    {
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,

        One = SDL_SCANCODE_1,
        Two = SDL_SCANCODE_2,
        Three = SDL_SCANCODE_3,
        Four = SDL_SCANCODE_4,
        Five = SDL_SCANCODE_5,
        Six = SDL_SCANCODE_6,
        Seven = SDL_SCANCODE_7,
        Eight = SDL_SCANCODE_8,
        Nine = SDL_SCANCODE_9,
        Zero = SDL_SCANCODE_0,

        Return = SDL_SCANCODE_RETURN,
        Enter = SDL_SCANCODE_RETURN,
        Escape = SDL_SCANCODE_ESCAPE,
        Backspace = SDL_SCANCODE_BACKSPACE,
        Tab = SDL_SCANCODE_TAB,
        Space = SDL_SCANCODE_SPACE,

        Minus = SDL_SCANCODE_MINUS,
        Equals = SDL_SCANCODE_EQUALS,
        LeftBracket = SDL_SCANCODE_LEFTBRACKET,
        RightBracket = SDL_SCANCODE_RIGHTBRACKET,
        Backslash = SDL_SCANCODE_BACKSLASH,
        NonUSHash = SDL_SCANCODE_NONUSHASH,
        Semicolon = SDL_SCANCODE_SEMICOLON,
        Apostrophe = SDL_SCANCODE_APOSTROPHE,
        Grave = SDL_SCANCODE_GRAVE,
        Comma = SDL_SCANCODE_COMMA,
        Period = SDL_SCANCODE_PERIOD,
        FullStop = SDL_SCANCODE_PERIOD,
        ForwardSlash = SDL_SCANCODE_SLASH,

        CapsLock = SDL_SCANCODE_CAPSLOCK,

        F1 = SDL_SCANCODE_F1,
        F2 = SDL_SCANCODE_F2,
        F3 = SDL_SCANCODE_F3,
        F4 = SDL_SCANCODE_F4,
        F5 = SDL_SCANCODE_F5,
        F6 = SDL_SCANCODE_F6,
        F7 = SDL_SCANCODE_F7,
        F8 = SDL_SCANCODE_F8,
        F9 = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,

        PrintScreen = SDL_SCANCODE_PRINTSCREEN,
        ScrollLock = SDL_SCANCODE_SCROLLLOCK,
        Pause = SDL_SCANCODE_PAUSE,
        Insert = SDL_SCANCODE_INSERT,
        Home = SDL_SCANCODE_HOME,
        PageUp = SDL_SCANCODE_PAGEUP,
        Delete = SDL_SCANCODE_DELETE,
        End = SDL_SCANCODE_END,
        PageDown = SDL_SCANCODE_PAGEDOWN,
        Right = SDL_SCANCODE_RIGHT,
        Left = SDL_SCANCODE_LEFT,
        Down = SDL_SCANCODE_DOWN,
        Up = SDL_SCANCODE_UP,

        NumLock = SDL_SCANCODE_NUMLOCKCLEAR,
        Clear = SDL_SCANCODE_NUMLOCKCLEAR,
        KeypadDivide = SDL_SCANCODE_KP_DIVIDE,
        KeypadMultiply = SDL_SCANCODE_KP_MULTIPLY,
        KeypadMinus = SDL_SCANCODE_KP_MINUS,
        KeypadPlus = SDL_SCANCODE_KP_PLUS,
        KeypadEnter = SDL_SCANCODE_KP_ENTER,
        Keypad1 = SDL_SCANCODE_KP_1,
        Keypad2 = SDL_SCANCODE_KP_2,
        Keypad3 = SDL_SCANCODE_KP_3,
        Keypad4 = SDL_SCANCODE_KP_4,
        Keypad5 = SDL_SCANCODE_KP_5,
        Keypad6 = SDL_SCANCODE_KP_6,
        Keypad7 = SDL_SCANCODE_KP_7,
        Keypad8 = SDL_SCANCODE_KP_8,
        Keypad9 = SDL_SCANCODE_KP_9,
        Keypad0 = SDL_SCANCODE_KP_0,
        KeypadPeriod = SDL_SCANCODE_KP_PERIOD,

        NonUSBackslash = SDL_SCANCODE_NONUSBACKSLASH,
        KeypadEquals = SDL_SCANCODE_KP_EQUALS,
        KeypadComma = SDL_SCANCODE_KP_COMMA,

        International1 = SDL_SCANCODE_INTERNATIONAL1,
        International2 = SDL_SCANCODE_INTERNATIONAL2,
        International3 = SDL_SCANCODE_INTERNATIONAL3,
        International4 = SDL_SCANCODE_INTERNATIONAL4,
        International5 = SDL_SCANCODE_INTERNATIONAL5,
        International6 = SDL_SCANCODE_INTERNATIONAL6,
        International7 = SDL_SCANCODE_INTERNATIONAL7,
        International8 = SDL_SCANCODE_INTERNATIONAL8,
        International9 = SDL_SCANCODE_INTERNATIONAL9,

        Lang1 = SDL_SCANCODE_LANG1,
        Lang2 = SDL_SCANCODE_LANG2,
        Lang3 = SDL_SCANCODE_LANG3,
        Lang4 = SDL_SCANCODE_LANG4,
        Lang5 = SDL_SCANCODE_LANG5,
        Lang6 = SDL_SCANCODE_LANG6,
        Lang7 = SDL_SCANCODE_LANG7,
        Lang8 = SDL_SCANCODE_LANG8,
        Lang9 = SDL_SCANCODE_LANG9,

        LeftCtrl = SDL_SCANCODE_LCTRL,
        LeftShift = SDL_SCANCODE_LSHIFT,
        LeftAlt = SDL_SCANCODE_LALT,
        LeftGUI = SDL_SCANCODE_LGUI,
        RightCtrl = SDL_SCANCODE_RCTRL,
        RightShift = SDL_SCANCODE_RSHIFT,
        RightAlt = SDL_SCANCODE_RALT,
        RightGUI = SDL_SCANCODE_RGUI,

        AltGr = SDL_SCANCODE_MODE,
    };

    constexpr usize KeyCount = magic_enum::enum_count<KeyCode>();

    [[nodiscard]] String KeyCodeToString(const KeyCode key);
}

#endif