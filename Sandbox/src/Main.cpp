#include <cstdlib>

#include <stardust/Stardust.h>

#include "TestScene.h"

auto main([[maybe_unused]] const sd::i32 argc, char* argv[]) -> sd::i32
{
    sd::Application application(
        sd::Application::CreateInfo{
            .applicationInfo = sd::Application::CreateInfo::ApplicationInfo{
                .applicationName = "Sandbox",
                .organisationName = "TestOrganisation",
            },

            .windowInfo = sd::Application::CreateInfo::WindowInfo{
                .title = "Sandbox",
                .isResizable = false,
            },

            .filesystemInfo = sd::Application::CreateInfo::FilesystemInfo{
                .assetsArchiveRelativeFilepath = "resources/assets.pak",
                .localesArchiveRelativeFilepath = "resources/locales.pak",
                .scriptsArchiveRelativeFilepath = "resources/scripts.pak",
                .logFileRelativeFilepath = "log.txt",
            },

            .virtualFilesystemInfo = sd::Application::CreateInfo::VirtualFilesystemInfo{
                .windowIconPath = "assets/icon/icon.png",
                .gameControllerDatabasePath = "assets/input/gamecontrollerdb.txt",
                .defaultControlsPath = "assets/input/default_controls.json",
                .shadersDirectoryPath = "assets/shaders",
            },

            .preferenceInfo = sd::Application::CreateInfo::PreferenceInfo{
                .preferencesRelativeDirectory = "preferences",
                .userPrefsFilename = "user_prefs.toml",
                .controlPrefsFilename = "controls_prefs.msgpack",
            },

            .graphicsInfo = sd::Application::CreateInfo::GraphicsInfo{
                .cameraHalfSize = 8.0f,
                .maxShapesPerBatch = 4'000u,
                .maxTextureSlotsPerBatch = 16u,
            },

            .physicsInfo = sd::Application::CreateInfo::PhysicsInfo{
                .fixedTimestep = 0.01,
                .positionIterations = 3u,
                .velocityIterations = 8u,
            },

            .argv0 = argv[0],

            .initialiseCallback = sd::Application::InitialiseCallback(
                [](const auto& application) -> sd::Status
                {
                    sd::Log::Debug("Application initialised.");
                
                    return sd::Status::Success;
                }
            ),
            .exitCallback = sd::None,
        }
    );

    if (!application.DidInitialiseSuccessfully())
    {
        return EXIT_FAILURE;
    }

    application.GetSceneManager().PushScene<TestScene>(application, "Test");
    application.Run();

    return EXIT_SUCCESS;
}