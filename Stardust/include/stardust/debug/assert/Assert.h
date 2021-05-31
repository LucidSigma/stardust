#pragma once
#ifndef STARDUST_ASSERT_H
#define STARDUST_ASSERT_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

#define STARDUST_ASSERT_RELEASE(condition) SDL_assert_release(condition)
#define STARDUST_ASSERT(condition) SDL_assert(condition)
#define STARDUST_ASSERT_PARANOID(condition) SDL_assert_paranoid(condition)

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