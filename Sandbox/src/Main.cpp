#include <cstdlib>

#include "stardust/Stardust.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	sd::Application application(sd::Application::CreateInfo{
		.title = "Stardust Sandbox",
		.logFilepath = "log.txt",
	});

	if (!application.DidInitialiseSuccessfully())
	{
		return EXIT_FAILURE;
	}

	application.Run();

	return EXIT_SUCCESS;
}