#include "stardust/debug/logging/Logging.h"

#include <memory>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "stardust/utility/string/String.h"

namespace stardust
{
    auto Log::Initialise(const StringView applicationName, const StringView logFilepath) -> void
    {
        const List<spdlog::sink_ptr> logSinks{
            std::make_shared<spdlog::sinks::stderr_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.data(), true),
        };

        logSinks[0u]->set_pattern("%^[%T.%e] %n: %v%$");
        logSinks[1u]->set_pattern("[%T.%e] [%l] %n: %v");

        const String clientLoggerName = string::ToUppercase(applicationName.data());

        s_engineLogger = std::make_shared<spdlog::logger>("STARDUST", std::cbegin(logSinks), std::cend(logSinks));
        s_engineLogger->set_level(spdlog::level::trace);
        s_engineLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_engineLogger);

        s_clientLogger = std::make_shared<spdlog::logger>(clientLoggerName.data(), std::cbegin(logSinks), std::cend(logSinks));
        s_clientLogger->set_level(spdlog::level::trace);
        s_clientLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_clientLogger);
    }

    auto Log::Shutdown() noexcept -> void
    {
        spdlog::shutdown();
    }
}
