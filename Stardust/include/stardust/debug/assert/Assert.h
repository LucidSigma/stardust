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

#ifndef SDL_ASSERT_LEVEL
    #define SDL_ASSERT_LEVEL STARDUST_ASSERT_LEVEL
#endif

#include <SDL2/SDL.h>

#define STARDUST_ASSERT_RELEASE(condition) SDL_assert_release(condition)
#define STARDUST_ASSERT(condition) SDL_assert(condition)
#define STARDUST_ASSERT_PARANOID(condition) SDL_assert_paranoid(condition)

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace debug
    {
        struct AssertionData
        {
            bool isAlwaysIgnored;

            u32 triggerCount;
            String condition;
            
            String filename;
            u32 lineNumber;
            String functionName;
        };

        [[nodiscard]] Vector<AssertionData> GetAssertionReport();
        void ResetAssertionReport();

        void InitialiseAssertionCallback();
        void ResetAssertionCallback();
    }
}

#endif