#include "stardust/debug/logging/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace stardust
{
    void Log::Initialise(const StringView& applicationName, const StringView& logFilepath)
    {
        const Vector<spdlog::sink_ptr> logSinks{
            std::make_shared<spdlog::sinks::stderr_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.data(), true),
        };

        logSinks[0u]->set_pattern("%^[%T.%e] %n: %v%$");
        logSinks[1u]->set_pattern("[%T.%e] [%l] %n: %v");

        const String clientLoggerName = string::MakeUpper(applicationName.data());

        s_engineLogger = std::make_shared<spdlog::logger>("STARDUST", std::cbegin(logSinks), std::cend(logSinks));
        s_engineLogger->set_level(spdlog::level::trace);
        s_engineLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_engineLogger);

        s_clientLogger = std::make_shared<spdlog::logger>(clientLoggerName, std::cbegin(logSinks), std::cend(logSinks));
        s_clientLogger->set_level(spdlog::level::trace);
        s_clientLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_clientLogger);
    }

    void Log::Shutdown() noexcept
    {
        spdlog::shutdown();
    }
}