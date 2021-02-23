#pragma once
#ifndef STARDUST_CURSOR_H
#define STARDUST_CURSOR_H

#include <type_traits>

#include <SDL2/SDL.h>

namespace stardust
{
	enum class CursorType
		: std::underlying_type_t<SDL_SystemCursor>
	{
		Arrow = SDL_SYSTEM_CURSOR_ARROW,
		IBeam = SDL_SYSTEM_CURSOR_IBEAM,
		Wait = SDL_SYSTEM_CURSOR_WAIT,
		Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
		WaitArrow = SDL_SYSTEM_CURSOR_WAITARROW,
		ResizeBackwardDiagonal = SDL_SYSTEM_CURSOR_SIZENWSE,
		ResizeForwardDiagonal = SDL_SYSTEM_CURSOR_SIZENESW,
		ResizeHorizontal = SDL_SYSTEM_CURSOR_SIZEWE,
		ResizeVertical = SDL_SYSTEM_CURSOR_SIZENS,
		ResizeAll = SDL_SYSTEM_CURSOR_SIZEALL,
		No = SDL_SYSTEM_CURSOR_NO,
		Hand = SDL_SYSTEM_CURSOR_HAND,
	};

	extern void SetCursor(const CursorType cursorType);
	extern void ResetCursor();
}

#endif