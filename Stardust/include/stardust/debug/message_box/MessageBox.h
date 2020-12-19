#pragma once
#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <string_view>
#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"

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

		void Show(const StringView& title, const StringView& message, const Type type);
		void Show(const std::string_view& title, const std::string_view& message, const Type type);
	}

	namespace modal = message_box;
}

#endif