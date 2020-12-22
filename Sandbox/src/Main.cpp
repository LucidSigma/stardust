#include <cstdlib>

#include <stardust/Stardust.h>

#include "TestScene.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	sd::Application application(sd::Application::CreateInfo{
		.applicationName = "Sandbox",
		.organisationName = "test_organisation",

		.windowTitle = "Stardust Sandbox",

		.filesystem = sd::Application::FilesystemInfo{
			.argv0 = argv[0],
			.assetsArchive = "assets.pak",
			.localesArchive = "locales.pak",
		},
		.filepaths = {
			.configFilepath = "config.json",
			.logFilepath = "log.txt",
			.screenshotDirectory = "screenshots",
			.windowIconFile = "assets/icon/icon.png",
		},
		.fixedTimestep = 0.01f,

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