#include "stardust/window/display/Display.h"

namespace stardust
{
	namespace display
	{
		[[nodiscard]] u32 GetVideoDisplayCount()
		{
			return static_cast<u32>(SDL_GetNumVideoDisplays());
		}

		[[nodiscard]] u32 GetVideoDriverCount()
		{
			return static_cast<u32>(SDL_GetNumVideoDrivers());
		}

		[[nodiscard]] DisplayData GetDisplayData(const i32 displayIndex)
		{
			DisplayData displayData{ };

			displayData.name = SDL_GetDisplayName(displayIndex);

			SDL_DisplayMode displayMode{ };
			SDL_GetCurrentDisplayMode(displayIndex, &displayMode);

			displayData.size = UVec2{ displayMode.w, displayMode.h };
			displayData.refreshRate = displayMode.refresh_rate;
			displayData.format = displayMode.format;

			SDL_GetDisplayBounds(displayIndex, &displayData.bounds);
			SDL_GetDisplayUsableBounds(displayIndex, &displayData.usableBounds);

			displayData.orientation = static_cast<DisplayOrientation>(SDL_GetDisplayOrientation(displayIndex));

			displayData.verticalDPI = 0.0f;
			displayData.horizontalDPI = 0.0f;
			displayData.diagonalDPI = 0.0f;
			SDL_GetDisplayDPI(displayIndex, &displayData.diagonalDPI, &displayData.horizontalDPI, &displayData.verticalDPI);

			return displayData;
		}
	}
}