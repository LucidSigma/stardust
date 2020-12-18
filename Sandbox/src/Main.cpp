#include <cstdlib>

#include <stardust/Stardust.h>

#include "TestScene.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	sd::Application application(sd::Application::CreateInfo{
		.title = "Stardust Sandbox",
		.filesystem = sd::Application::FilesystemInfo{
			.argv0 = argv[0],
			.assetsArchive = "assets.pak",
			.localesArchive = "locales.pak",
			.logFilepath = "log.txt",
			.configFilepath = "config.json",
		},
		.fixedTimestep = 0.01f,
	});

	if (!application.DidInitialiseSuccessfully())
	{
		return EXIT_FAILURE;
	}

	application.GetSceneManager().PushScene<TestScene>(application, "Test");
	application.Run();

	return EXIT_SUCCESS;
}