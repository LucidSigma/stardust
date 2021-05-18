#pragma once
#ifndef STARDUST_ASYNC_H
#define STARDUST_ASYNC_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <chrono>
#include <concepts>
#include <future>
#include <type_traits>
#include <utility>

#include "stardust/data/Types.h"

namespace stardust
{
    template <typename T>
    class AsyncTask
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

        AsyncTask& operator =(AsyncTask&& other) noexcept
        {
            m_handle = std::move(other.m_handle);

            return *this;
        }

        ~AsyncTask() noexcept = default;

        [[nodiscard]] T Await()
        {
            m_handle.wait();

            return m_handle.get();
        }

        [[nodiscard]] Optional<T> AwaitFor(const f32 seconds)
        {
            switch (const std::future_status awaitStatus = m_handle.wait_for(std::chrono::milliseconds(static_cast<u32>(seconds * 1'000.0f)));
                    awaitStatus)
            {
            case std::future_status::ready:
                return m_handle.get();

            case std::future_status::timeout:
            case std::future_status::deferred:
            default:
                return NullOpt;
            }
        }

        template <typename Clock = std::chrono::system_clock, typename Duration = Clock::duration/*, typename = std::enable_if_t<std::is_clock_v<Clock>*/>
        [[nodiscard]] Optional<T> AwaitUntil(const std::chrono::time_point<Clock, Duration>& timePoint)
        {
            switch (const std::future_status awaitStatus = m_handle.wait_until(timePoint);
                    awaitStatus)
            {
            case std::future_status::ready:
                return m_handle.get();

            case std::future_status::timeout:
            case std::future_status::deferred:
            default:
                return NullOpt;
            }
        }
    };

    template <std::regular_invocable Func, typename... Args>
    [[nodiscard]] inline AsyncTask<std::invoke_result_t<Func, Args...>> RunAsync(Func&& task, Args&&... args)
    {
        return std::async(std::launch::async, task, std::forward<Args>(args)...);
    }
}

#endif