#pragma once
#ifndef STARDUST_ASSERT_H
#define STARDUST_ASSERT_H

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

#ifdef NDEBUG
    static_assert(SDL_ASSERT_LEVEL == 1);
#else
    static_assert(SDL_ASSERT_LEVEL == 2);
#endif

#ifdef STARDUST_ASSERT
    #undef STARDUST_ASSERT
#endif

#ifdef STARDUST_ASSERT_RELEASE
    #undef STARDUST_ASSERT_RELEASE
#endif

#define STARDUST_ASSERT(condition) SDL_assert_release(condition)
#define STARDUST_ASSERT_RELEASE(condition) SDL_assert(condition)

namespace stardust
{
    namespace debug
    {
        struct AssertionData final
        {
            bool isAlwaysIgnored;

            u32 triggerCount;
            String condition;

            String filename;
            u32 lineNumber;
            String functionName;
        };

        [[nodiscard]] extern auto GetAssertionReport() -> List<AssertionData>;
        extern auto ClearAssertions() -> void;

        extern auto InitialiseAssertionCallback() -> void;
        extern auto ResetAssertionCallback() -> void;
    }
}

#endif
