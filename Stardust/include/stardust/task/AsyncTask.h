#pragma once
#ifndef STARDUST_ASYNC_TASK_H
#define STARDUST_ASYNC_TASK_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <chrono>
#include <concepts>
#include <future>
#include <ratio>
#include <type_traits>
#include <utility>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    template <typename T>
    class AsyncTask final
        : private INoncopyable
    {
    private:
        std::future<T> m_handle;

    public:
        AsyncTask(std::future<T>&& future)
            : m_handle(std::move(future))
        { }

        AsyncTask(AsyncTask&& other) noexcept
            : m_handle(std::move(other.m_handle))
        { }

        auto operator =(AsyncTask&& other) noexcept -> AsyncTask&
        {
            m_handle = std::move(other.m_handle);

            return *this;
        }

        ~AsyncTask() noexcept = default;

        [[nodiscard]] auto Await() -> T
        {
            m_handle.wait();

            return m_handle.get();
        }

        [[nodiscard]] auto AwaitFor(const f32 seconds) -> Optional<T>
            requires !std::is_void_v<T>
        {
            switch (const std::future_status awaitStatus = m_handle.wait_for(std::chrono::milliseconds(static_cast<u32>(seconds) * static_cast<u32>(std::milli::den)));
                awaitStatus)
            {
            case std::future_status::ready:
                return m_handle.get();

            case std::future_status::timeout:
            case std::future_status::deferred:
                return None;
            }
        }

        [[nodiscard]] auto AwaitFor(const f32 seconds) -> bool
            requires std::is_void_v<T>
        {
            switch (const std::future_status awaitStatus = m_handle.wait_for(std::chrono::milliseconds(static_cast<u32>(seconds) * static_cast<u32>(std::milli::den)));
                    awaitStatus)
            {
            case std::future_status::ready:
                m_handle.get();

                return true;

            case std::future_status::timeout:
            case std::future_status::deferred:
                return false;
            }
        }

        template <typename Clock = std::chrono::system_clock, typename Duration = Clock::duration>
            requires !std::is_void_v<T>&& std::chrono::is_clock_v<Clock>
        [[nodiscard]] auto AwaitUntil(const std::chrono::time_point<Clock, Duration>& timePoint) -> Optional<T>
        {
            switch (const std::future_status awaitStatus = m_handle.wait_until(timePoint);
                    awaitStatus)
            {
            case std::future_status::ready:
                return m_handle.get();

            case std::future_status::timeout:
            case std::future_status::deferred:
                return None;
            }
        }

        template <typename Clock = std::chrono::system_clock, typename Duration = Clock::duration>
            requires std::is_void_v<T>&& std::chrono::is_clock_v<Clock>
        [[nodiscard]] auto AwaitUntil(const std::chrono::time_point<Clock, Duration>& timePoint) -> bool
        {
            switch (const std::future_status awaitStatus = m_handle.wait_until(timePoint);
                    awaitStatus)
            {
            case std::future_status::ready:
                m_handle.get();

                return true;

            case std::future_status::timeout:
            case std::future_status::deferred:
                return false;
            }
        }
    };

    template <std::regular_invocable Func, typename... Args>
    [[nodiscard]] inline auto RunAsync(Func&& task, Args&&... args) -> AsyncTask<std::invoke_result_t<Func, Args...>>
    {
        return std::async(std::launch::async, task, std::forward<Args>(args)...);
    }
}

#endif
