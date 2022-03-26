#pragma once
#ifndef STARDUST_TIMERS_H
#define STARDUST_TIMERS_H

#include "stardust/types/Primitives.h"

namespace stardust
{
    class Timeout final
    {
    private:
        f32 m_accumulatedTime = 0.0f;
        f32 m_targetTime = 0.0f;

    public:
        Timeout() = default;
        explicit Timeout(const u32 milliseconds);
        ~Timeout() noexcept = default;

        auto Update(const f32 deltaTime) -> void;

        [[nodiscard]] auto HasPassed() const noexcept -> bool;

        auto Set(const u32 milliseconds) noexcept -> void;
        auto Clear() noexcept -> void;
    };
}

#endif
