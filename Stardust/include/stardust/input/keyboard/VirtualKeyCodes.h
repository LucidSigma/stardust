#pragma once
#ifndef STARDUST_VIRTUAL_KEY_CODES_H
#define STARDUST_VIRTUAL_KEY_CODES_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class VirtualKeyCode
        : std::underlying_type_t<SDL_KeyCode>
    {
        Unknown = SDLK_UNKNOWN,

        Return = SDLK_RETURN,
        Escape = SDLK_ESCAPE,
        Backspace = SDLK_BACKSPACE,
        Tab = SDLK_TAB,
        Space = SDLK_SPACE,

        ExclamationMark = SDLK_EXCLAIM,
        DoubleQuote = SDLK_QUOTEDBL,
        Hash = SDLK_HASH,
        Percent = SDLK_PERCENT,
        DollarSign = SDLK_DOLLAR,
        Ampersand = SDLK_AMPERSAND,
        Apostrophe = SDLK_QUOTE,
        LeftParenthesis = SDLK_LEFTPAREN,
        RightParenthesis = SDLK_RIGHTPAREN,
        Asterisk = SDLK_ASTERISK,
        Plus = SDLK_PLUS,
        Comma = SDLK_COMMA,
        Minus = SDLK_MINUS,
        FullStop = SDLK_PERIOD,
        ForwardSlash = SDLK_SLASH,

        Zero = SDLK_0,
        One = SDLK_1,
        Two = SDLK_2,
        Three = SDLK_3,
        Four = SDLK_4,
        Five = SDLK_5,
        Six = SDLK_6,
        Seven = SDLK_7,
        Eight = SDLK_8,
        Nine = SDLK_9,

        Colon = SDLK_COLON,
        Semicolon = SDLK_SEMICOLON,
        LessThan = SDLK_LESS,
        Equals = SDLK_EQUALS,
        GreaterThan = SDLK_GREATER,
        QuestionMark = SDLK_QUESTION,
        AtSign = SDLK_AT,

        LeftBracket = SDLK_LEFTBRACKET,
        Backslash = SDLK_BACKSLASH,
        RightBracket = SDLK_RIGHTBRACKET,
        Caret = SDLK_CARET,
        Underscore = SDLK_UNDERSCORE,
        GraveAccent = SDLK_BACKQUOTE,

        A = SDLK_a,
        B = SDLK_b,
        C = SDLK_c,
        D = SDLK_d,
        E = SDLK_e,
        F = SDLK_f,
        G = SDLK_g,
        H = SDLK_h,
        I = SDLK_i,
        J = SDLK_j,
        K = SDLK_k,
        L = SDLK_l,
        M = SDLK_m,
        N = SDLK_n,
        O = SDLK_o,
        P = SDLK_p,
        Q = SDLK_q,
        R = SDLK_r,
        S = SDLK_s,
        T = SDLK_t,
        U = SDLK_u,
        V = SDLK_v,
        W = SDLK_w,
        X = SDLK_x,
        Y = SDLK_y,
        Z = SDLK_z,

        CapsLock = SDLK_CAPSLOCK,

        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,

        PrintScreen = SDLK_PRINTSCREEN,
        ScrollLock = SDLK_SCROLLLOCK,
        PauseBreak = SDLK_PAUSE,
        Insert = SDLK_INSERT,
        Home = SDLK_HOME,
        PageUp = SDLK_PAGEUP,
        Delete = SDLK_DELETE,
        End = SDLK_END,
        PageDown = SDLK_PAGEDOWN,

        Right = SDLK_RIGHT,
        Left = SDLK_LEFT,
        Down = SDLK_DOWN,
        Up = SDLK_UP,

        NumLockClear = SDLK_NUMLOCKCLEAR,
        KeypadDivide = SDLK_KP_DIVIDE,
        KeypadMultiply = SDLK_KP_MULTIPLY,
        KeypadSubtract = SDLK_KP_MINUS,
        KeypadAdd = SDLK_KP_PLUS,
        KeypadEnter = SDLK_KP_ENTER,
        Keypad1 = SDLK_KP_1,
        Keypad2 = SDLK_KP_2,
        Keypad3 = SDLK_KP_3,
        Keypad4 = SDLK_KP_4,
        Keypad5 = SDLK_KP_5,
        Keypad6 = SDLK_KP_6,
        Keypad7 = SDLK_KP_7,
        Keypad8 = SDLK_KP_8,
        Keypad9 = SDLK_KP_9,
        Keypad0 = SDLK_KP_0,
        KeypadFullStop = SDLK_KP_PERIOD,

        Application = SDLK_APPLICATION,
        Power = SDLK_POWER,

        KeypadEquals = SDLK_KP_EQUALS,

        F13 = SDLK_F13,
        F14 = SDLK_F14,
        F15 = SDLK_F15,
        F16 = SDLK_F16,
        F17 = SDLK_F17,
        F18 = SDLK_F18,
        F19 = SDLK_F19,
        F20 = SDLK_F20,
        F21 = SDLK_F21,
        F22 = SDLK_F22,
        F23 = SDLK_F23,
        F24 = SDLK_F24,

        Execute = SDLK_EXECUTE,
        Help = SDLK_HELP,
        Menu = SDLK_MENU,
        Select = SDLK_SELECT,
        Stop = SDLK_STOP,
        Again = SDLK_AGAIN,
        Undo = SDLK_UNDO,
        Cut = SDLK_CUT,
        Copy = SDLK_COPY,
        Paste = SDLK_PASTE,
        Find = SDLK_FIND,
        Mute = SDLK_MUTE,
        VolumeUp = SDLK_VOLUMEUP,
        VolumeDown = SDLK_VOLUMEDOWN,

        KeypadComma = SDLK_KP_COMMA,
        KeypadEqualsAS400 = SDLK_KP_EQUALSAS400,

        AlternateErase = SDLK_ALTERASE,
        SysReq = SDLK_SYSREQ,
        Cancel = SDLK_CANCEL,
        Clear = SDLK_CLEAR,
        Prior = SDLK_PRIOR,
        Return2 = SDLK_RETURN2,
        Separator = SDLK_SEPARATOR,
        Out = SDLK_OUT,
        Oper = SDLK_OPER,
        ClearAgain = SDLK_CLEARAGAIN,
        CrSel = SDLK_CRSEL,
        ExSel = SDLK_EXSEL,

        Keypad00 = SDLK_KP_00,
        Keypad000 = SDLK_KP_000,
        KeypadThousandsSeparator = SDLK_THOUSANDSSEPARATOR,
        KeypadDecimalSeparator = SDLK_DECIMALSEPARATOR,

        KeypadCurrencyUnit = SDLK_CURRENCYUNIT,
        KeypadCurrencySubUnit = SDLK_CURRENCYSUBUNIT,
        KeypadLeftParenthesis = SDLK_KP_LEFTPAREN,
        KeypadRightParenthesis = SDLK_KP_RIGHTPAREN,
        KeypadLeftBrace = SDLK_KP_LEFTBRACE,
        KeypadRightBrace = SDLK_KP_RIGHTBRACE,
        KeypadTab = SDLK_KP_TAB,
        KeypadBackspace = SDLK_KP_BACKSPACE,

        KeypadA = SDLK_KP_A,
        KeypadB = SDLK_KP_B,
        KeypadC = SDLK_KP_C,
        KeypadD = SDLK_KP_D,
        KeypadE = SDLK_KP_E,
        KeypadF = SDLK_KP_F,

        KeypadXOR = SDLK_KP_XOR,
        KeypadExponent = SDLK_KP_POWER,
        KeypadPercent = SDLK_KP_PERCENT,
        KeypadLessThan = SDLK_KP_LESS,
        KeypadGreaterThan = SDLK_KP_GREATER,
        KeypadAmpersand = SDLK_KP_AMPERSAND,
        KeypadDoubleAmpersand = SDLK_KP_DBLAMPERSAND,
        KeypadVerticalBar = SDLK_KP_VERTICALBAR,
        KeypadDoubleVerticalBar = SDLK_KP_DBLVERTICALBAR,
        KeypadColon = SDLK_KP_COLON,
        KeypadHash = SDLK_KP_HASH,
        KeypadSpace = SDLK_KP_SPACE,
        KeypadAtSign = SDLK_KP_AT,
        KeypadExclamationMark = SDLK_KP_EXCLAM,

        KeypadMemoryStore = SDLK_KP_MEMSTORE,
        KeypadMemoryRecall = SDLK_KP_MEMRECALL,
        KeypadMemoryClear = SDLK_KP_MEMCLEAR,
        KeypadMemoryAdd = SDLK_KP_MEMADD,
        KeypadMemorySubtract = SDLK_KP_MEMSUBTRACT,
        KeypadMemoryMultiply = SDLK_KP_MEMMULTIPLY,
        KeypadMemoryDivide = SDLK_KP_MEMDIVIDE,

        KeypadPlusMinus = SDLK_KP_PLUSMINUS,

        KeypadClear = SDLK_KP_CLEAR,
        KeypadClearEntry = SDLK_KP_CLEARENTRY,

        KeypadBinary = SDLK_KP_BINARY,
        KeypadOctal = SDLK_KP_OCTAL,
        KeypadDecimal = SDLK_KP_DECIMAL,
        KeypadHexadecimal = SDLK_KP_HEXADECIMAL,

        LeftControl = SDLK_LCTRL,
        LeftShift = SDLK_LSHIFT,
        LeftAlt = SDLK_LALT,
        LeftGUI = SDLK_LGUI,
        RightControl = SDLK_RCTRL,
        RightShift = SDLK_RSHIFT,
        RightAlt = SDLK_RALT,
        RightGUI = SDLK_RGUI,

        Mode = SDLK_MODE,

        AudioNext = SDLK_AUDIONEXT,
        AudioPrevious = SDLK_AUDIOPREV,
        AudioStop = SDLK_AUDIOSTOP,
        AudioPlay = SDLK_AUDIOPLAY,
        AudioMute = SDLK_AUDIOMUTE,

        MediaSelect = SDLK_MEDIASELECT,
        WWW = SDLK_WWW,
        Mail = SDLK_MAIL,
        Calculator = SDLK_CALCULATOR,
        Computer = SDLK_COMPUTER,

        ACSearch = SDLK_AC_SEARCH,
        ACHome = SDLK_AC_HOME,
        ACBack = SDLK_AC_BACK,
        ACForward = SDLK_AC_FORWARD,
        ACStop = SDLK_AC_STOP,
        ACRefresh = SDLK_AC_REFRESH,
        ACBookmarks = SDLK_AC_BOOKMARKS,

        DisplayBrightnessDown = SDLK_BRIGHTNESSDOWN,
        DisplayBrightnessUp = SDLK_BRIGHTNESSUP,
        DisplaySwitch = SDLK_DISPLAYSWITCH,
        KeyboardBrightnessToggle = SDLK_KBDILLUMTOGGLE,
        KeyboardBrightnessDown = SDLK_KBDILLUMDOWN,
        KeyboardBrightnessUp = SDLK_KBDILLUMUP,

        Eject = SDLK_EJECT,
        Sleep = SDLK_SLEEP,

        App1 = SDLK_APP1,
        App2 = SDLK_APP2,

        AudioRewind = SDLK_AUDIOREWIND,
        AudioFastForward = SDLK_AUDIOFASTFORWARD,
    };

    enum class KeyCode;

    [[nodiscard]] extern auto ConvertVirtualKeyCodeToKeyCode(const VirtualKeyCode virtualKeyCode) -> KeyCode;
    [[nodiscard]] extern auto GetInternalVirtualKeyCodeName(const VirtualKeyCode virtualKeyCode) -> String;
}

#endif
