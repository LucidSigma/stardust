#include <cstdlib>

#include <stardust/Stardust.h>

#include "TestScene.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	sd::Application application(sd::Application::CreateInfo{
		.applicationName = "Sandbox",
		.organisationName = "test_organisation",

		.windowTitle = "Stardust Sandbox",
		.allowResizableWindow = true,

		.filesystem = sd::Application::FilesystemInfo{
			.argv0 = argv[0],
			.assetsArchive = "assets.pak",
			.localesArchive = "locales.pak",
		},
		.filepaths = sd::Application::FilepathsInfo{
			.defaultConfigFilepath = "assets/defaults/config.json",
			.windowIconFile = "assets/icon/icon.png",
		},

		.physicsInfo = sd::Application::PhysicsInfo{
			.fixedTimestep = 0.01,
			.velocityIterations = 8u,
			.positionIterations = 3u,
		},

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