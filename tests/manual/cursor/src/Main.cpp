#include <cstdlib>

#include <magic_enum/magic_enum.hpp>
#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("cursor test", "log.txt");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return EXIT_FAILURE;
    }

    sd::Window window(
        sd::Window::CreateInfo{
            .title = "Cursor Test",
            .x = sd::Window::Position::Undefined,
            .y = sd::Window::Position::Undefined,
            .size = sd::UVector2{ 512u, 512u },
            .flags = { sd::Window::CreateFlag::Shown },
        }
    );

    if (!window.IsValid())
    {
        SDL_Quit();
        sd::Log::Shutdown();

        return EXIT_FAILURE;
    }

    for (const auto& [value, name] : magic_enum::enum_entries<sd::cursor::Type>())
    {
        sd::cursor::SetType(value);
        sd::Log::Trace("Current cursor: {}.", name);

        sd::time::WaitForSeconds(1u);
    }

    sd::cursor::Reset();
    sd::Log::Trace("Current cursor: Default.");
    sd::time::WaitForSeconds(1u);

    window.Destroy();
    SDL_Quit();
    sd::Log::Shutdown();

    return EXIT_SUCCESS;
}

