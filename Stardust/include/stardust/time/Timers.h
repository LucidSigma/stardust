#pragma once
#ifndef STARDUST_TIMERS_H
#define STARDUST_TIMERS_H

#include <SDL2/SDL.h>

#include "stardust/data/Types.h"

namespace stardust
{
	using TimerID = SDL_TimerID;
	using TimerCallback = SDL_TimerCallback;

	TimerID AddTimer(const u32 millisecondDelay, const TimerCallback callback, void* const parameter);
	bool RemoveTimer(const TimerID timerID);
}

#endif