#include "stardust/time/Timers.h"

namespace stardust
{
    TimerID AddTimer(const u32 millisecondDelay, const TimerCallback callback, void* const parameter)
    {
        return SDL_AddTimer(millisecondDelay, callback, parameter);
    }

    bool RemoveTimer(const TimerID timerID)
    {
        return SDL_RemoveTimer(timerID) == SDL_TRUE;
    }
}