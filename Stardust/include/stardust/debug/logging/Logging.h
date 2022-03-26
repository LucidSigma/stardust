#pragma once
#ifndef STARDUST_LOGGING_H
#define STARDUST_LOGGING_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <utility>

#include <spdlog/spdlog.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class Log final
        : private INoncopyable, private INonmovable
    {
    private:
        inline static SharedPointer<spdlog::logger> s_engineLogger = nullptr;
        inline static SharedPointer<spdlog::logger> s_clientLogger = nullptr;

    public:
        static auto Initialise(const StringView applicationName, const StringView logFilepath) -> void;
        static auto Shutdown() noexcept -> void;

        template <typename... Args>
        static auto EngineTrace(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->trace(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto EngineDebug(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->debug(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto EngineInfo(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->info(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto EngineWarn(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->warn(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto EngineError(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->error(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto EngineCritical(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_engineLogger->critical(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Trace(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_clientLogger->trace(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Debug(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_clientLogger->debug(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Info(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_clientLogger->info(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Warn(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_clientLogger->warn(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Error(const StringView message, Args&&... args) -> void
        {
        #ifndef NDEBUG
            s_clientLogger->error(message.data(), std::forward<Args>(args)...);
        #endif
        }

        template <typename... Args>
        static auto Critical(const StringView message, Args&&... args) -> void
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
