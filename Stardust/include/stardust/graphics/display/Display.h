#pragma once
#ifndef STARDUST_DISPLAY_H
#define STARDUST_DISPLAY_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    namespace display
    {
        using Index = u32;

        enum class PresentationMode
            : i32
        {
            AdaptiveVSync = -1,
            Immediate = 0,
            VSync = 1,
        };

        struct DisplayData final
        {
            String name;

            UVector2 size;
            u32 refreshRate;

            f32 verticalDPI;
            f32 horizontalDPI;
            f32 diagonalDPI;
        };

        [[nodiscard]] extern auto GetVideoDisplayCount() -> u32;
        [[nodiscard]] extern auto GetDisplayData(const Index displayIndex) -> DisplayData;
        [[nodiscard]] extern auto GetAllDisplayData() -> List<DisplayData>;

        [[nodiscard]] extern auto SetPresentationMode(const PresentationMode presentationMode) -> Status;
    }
}

#endif
