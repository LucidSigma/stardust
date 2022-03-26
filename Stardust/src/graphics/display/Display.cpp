#include "stardust/graphics/display/Display.h"

namespace stardust
{
    namespace display
    {
        [[nodiscard]] auto GetVideoDisplayCount() -> u32
        {
            return static_cast<u32>(SDL_GetNumVideoDisplays());
        }

        [[nodiscard]] auto GetDisplayData(const Index displayIndex) -> DisplayData
        {
            DisplayData displayData{ };

            displayData.name = SDL_GetDisplayName(static_cast<i32>(displayIndex));

            SDL_DisplayMode displayMode{ };
            SDL_GetCurrentDisplayMode(static_cast<i32>(displayIndex), &displayMode);

            displayData.size = UVector2{ displayMode.w, displayMode.h };
            displayData.refreshRate = displayMode.refresh_rate;

            displayData.verticalDPI = 0.0f;
            displayData.horizontalDPI = 0.0f;
            displayData.diagonalDPI = 0.0f;
            SDL_GetDisplayDPI(static_cast<i32>(displayIndex), &displayData.diagonalDPI, &displayData.horizontalDPI, &displayData.verticalDPI);

            return displayData;
        }

        [[nodiscard]] auto GetAllDisplayData() -> List<DisplayData>
        {
            List<DisplayData> displays{ };
            const u32 displayCount = GetVideoDisplayCount();
            displays.reserve(static_cast<usize>(displayCount));

            for (Index displayIndex = 0u; displayIndex < static_cast<Index>(displayCount); ++displayIndex)
            {
                displays.push_back(GetDisplayData(displayIndex));
            }

            return displays;
        }
        
        [[nodiscard]] auto SetPresentationMode(const PresentationMode presentationMode) -> Status
        {
            return SDL_GL_SetSwapInterval(static_cast<i32>(presentationMode)) == 0
                ? Status::Success
                : Status::Fail;
        }
    }
}
