#pragma once
#ifndef STARDUST_ASSERT_H
#define STARDUST_ASSERT_H

#ifndef STARDUST_ASSERT_LEVEL
    #ifdef NDEBUG
        #define STARDUST_ASSERT_LEVEL 1
    #else
        #define STARDUST_ASSERT_LEVEL 2
    #endif
#endif

#define SDL_ASSERT_LEVEL STARDUST_ASSERT_LEVEL

#include <SDL2/SDL.h>

#define STARDUST_ASSERT_RELEASE(condition) SDL_assert_release(condition)
#define STARDUST_ASSERT(condition) SDL_assert(condition)
#define STARDUST_ASSERT_PARANOID(condition) SDL_assert_paranoid(condition)

namespace stardust
{
    namespace debug
    {
        
    }
}

#endif