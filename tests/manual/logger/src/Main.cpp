#include <cstdlib>

#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("logger test", "log.txt");

    sd::Log::EngineTrace("This message is {}.", "trace");
    sd::Log::EngineDebug("This message is {}.", "debug");
    sd::Log::EngineInfo("This message is {}.", "info");
    sd::Log::EngineWarn("This message is {}.", "warn");
    sd::Log::EngineError("This message is {}.", "error");
    sd::Log::EngineCritical("This message is {}.", "critical");

    sd::Log::Trace("This message is {}.", "trace");
    sd::Log::Debug("This message is {}.", "debug");
    sd::Log::Info("This message is {}.", "info");
    sd::Log::Warn("This message is {}.", "warn");
    sd::Log::Error("This message is {}.", "error");
    sd::Log::Critical("This message is {}.", "critical");

    sd::Log::Shutdown();

    STARDUST_ASSERT_RELEASE(sd::fs::DoesPathExist(sd::fs::GetApplicationBaseDirectory() + "log.txt"));

    return EXIT_SUCCESS;
}
