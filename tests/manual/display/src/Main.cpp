#include <cstdlib>

#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("display test", "log.txt");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return EXIT_FAILURE;
    }

    const sd::u32 displayCount = sd::display::GetVideoDisplayCount();

    sd::Log::Info("{} displays connected.", displayCount);

    for (sd::u32 displayIndex = 0u; displayIndex < displayCount; ++displayIndex)
    {
        const auto displayData = sd::display::GetDisplayData(displayIndex);

        sd::Log::Info("");
        sd::Log::Info("Display {}:", displayIndex);
        sd::Log::Info("Name: {}", displayData.name);

        sd::Log::Info("Size: ({}x{})", displayData.size.x, displayData.size.y);
        sd::Log::Info("Refresh rate: {}", displayData.refreshRate);

        sd::Log::Info("Horizontal DPI: {}", displayData.horizontalDPI);
        sd::Log::Info("Vertical DPI: {}", displayData.verticalDPI);
        sd::Log::Info("Diagonal DPI: {}", displayData.diagonalDPI);
    }

    SDL_Quit();
    sd::Log::Shutdown();

    return EXIT_SUCCESS;
}

