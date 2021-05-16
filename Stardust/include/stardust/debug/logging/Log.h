#pragma once
#ifndef STARDUST_LOG_H
#define STARDUST_LOG_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <utility>

#include <spdlog/spdlog.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
    class Log
        : private INoncopyable, private INonmovable
    {
    private:
        inline static SharedPtr<spdlog::logger> s_engineLogger = nullptr;
        inline static SharedPtr<spdlog::logger> s_clientLogger = nullptr;

    public:
        static void Initialise(const StringView& applicationName, const StringView& logFilepath);

        template <typename... Args>
        static void EngineTrace(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->trace(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void EngineDebug(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->debug(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void EngineInfo(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->info(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void EngineWarn(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->warn(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void EngineError(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->error(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void EngineCritical(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_engineLogger->critical(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Trace(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->trace(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Debug(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->debug(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Info(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->info(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Warn(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->warn(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Error(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->error(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static void Critical(const StringView& message, Args&&... args)
        {
        #ifndef NDEBUG
            s_clientLogger->critical(message.data(), std::forward<Args>(args)...);
        #endif
        }

        Log() = delete;
        ~Log() noexcept = delete;
    };
}

#endif