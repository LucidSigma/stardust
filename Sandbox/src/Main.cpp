#include <cstdlib>

#include <stardust/Stardust.h>

#include "TestScene.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
    sd::Application application(sd::Application::CreateInfo{
        .appTOMLRelativeFilepath = "app.toml",
        .argv0 = argv[0],

        .initialiseCallback = sd::Application::InitialiseCallback([](const auto& application) -> sd::Status { sd::Log::Debug("Application initialised."); return sd::Status::Success; }),
        .exitCallback = sd::NullOpt,
    });

    if (!application.DidInitialiseSuccessfully())
    {
        return EXIT_FAILURE;
    }

    application.GetSceneManager().PushScene<TestScene>(application, "Test");
    application.Run();

    return EXIT_SUCCESS;
}