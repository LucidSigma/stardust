#pragma once
#ifndef STARDUST_MESSAGE_BOX_H
#define STARDUST_MESSAGE_BOX_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace message_box
    {
        enum class Type
            : std::underlying_type_t<SDL_MessageBoxFlags>
        {
            Error = SDL_MESSAGEBOX_ERROR,
            Information = SDL_MESSAGEBOX_INFORMATION,
            Warning = SDL_MESSAGEBOX_WARNING,
        };

        enum class ButtonFlag
            : std::underlying_type_t<SDL_MessageBoxButtonFlags>
        {
            ReturnKeyDefault = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            EscapeKeyDefault = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
        };

        using ButtonID = i32;

        struct ButtonData
        {
            ButtonID id;
            String text;

            Vector<ButtonFlag> flags;
        };

        void Show(const StringView& title, const StringView& message, const Type type);
        ButtonID ShowComplex(const StringView& title, const StringView& message, const Type type, const Vector<ButtonData>& buttons);
    }

    namespace modal = message_box;
}

#endif