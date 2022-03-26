#pragma once
#ifndef STARDUST_MESSAGE_BOX_H
#define STARDUST_MESSAGE_BOX_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace message_box
    {
        enum class Type
            : std::underlying_type_t<SDL_MessageBoxFlags>
        {
            Error = SDL_MESSAGEBOX_ERROR,
            Warning = SDL_MESSAGEBOX_WARNING,
            Information = SDL_MESSAGEBOX_INFORMATION,
        };

        enum class ButtonFlag
            : std::underlying_type_t<SDL_MessageBoxButtonFlags>
        {
            ReturnKeyDefault = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            EscapeKeyDefault = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
        };

        using ButtonID = i32;

        struct ButtonData final
        {
            ButtonID id;
            String text;

            List<ButtonFlag> flags;
        };

        auto Show(const StringView title, const StringView message, const Type type) -> void;
        auto ShowComplex(const StringView title, const StringView message, const Type type, const List<ButtonData>& buttons) -> ButtonID;
    }
}

#endif
