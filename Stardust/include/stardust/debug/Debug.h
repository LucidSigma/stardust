#pragma once
#ifndef STARDUST_DEBUG_H
#define STARDUST_DEBUG_H

#include <SDL2/SDL.h>

#ifdef STARDUST_TRIGGER_BREAKPOINT
    #undef STARDUST_TRIGGER_BREAKPOINT
#endif

#define STARDUST_TRIGGER_BREAKPOINT() SDL_TriggerBreakpoint()

#endif
