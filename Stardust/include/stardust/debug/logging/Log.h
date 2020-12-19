#pragma once
#ifndef LOG_H
#define LOG_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <memory>
#include <utility>

#include <spdlog/spdlog.h>

#include "stardust/data/Containers.h"

namespace stardust
{
	class Log
		: private INoncopyable, private INonmovable
	{
	private:
		inline static std::shared_ptr<spdlog::logger> s_engineLogger = nullptr;
		inline static std::shared_ptr<spdlog::logger> s_clientLogger = nullptr;

	public:
		static void Initialise(const String& logFilepath);

		template <typename... Args>
		static void EngineTrace(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->trace(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineDebug(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->debug(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineInfo(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->info(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineWarn(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->warn(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineError(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->error(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineCritical(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->critical(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Trace(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->trace(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Debug(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->debug(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Info(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->info(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Warn(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->warn(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Error(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->error(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Critical(const String& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->critical(message.c_str(), std::forward<Args>(args)...);
		#endif
		}

		Log() = delete;
		~Log() noexcept = delete;
	};
}

#endif