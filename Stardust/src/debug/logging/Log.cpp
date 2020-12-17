#include "stardust/debug/logging/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace stardust
{
	void Log::Initialise(const String& logFilepath)
	{
		const Vector<spdlog::sink_ptr> logSinks{
			std::make_shared<spdlog::sinks::stderr_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.cpp_str(), true),
		};

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_engineLogger = std::make_shared<spdlog::logger>("STARDUST", eastl::cbegin(logSinks), eastl::cend(logSinks));
		spdlog::register_logger(s_engineLogger);
		s_engineLogger->set_level(spdlog::level::trace);
		s_engineLogger->flush_on(spdlog::level::trace);

		s_clientLogger = std::make_shared<spdlog::logger>("CLIENT", eastl::cbegin(logSinks), eastl::cend(logSinks));
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->flush_on(spdlog::level::trace);
	}
}