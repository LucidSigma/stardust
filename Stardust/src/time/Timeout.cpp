#include "stardust/time/Timeout.h"

#include <ratio>

namespace stardust
{
    Timeout::Timeout(const u32 milliseconds)
    {
        Set(milliseconds);
    }

    auto Timeout::Update(const f32 deltaTime) -> void
    {
        m_accumulatedTime += deltaTime;
    }

    [[nodiscard]] auto Timeout::HasPassed() const noexcept -> bool
    {
        return m_targetTime >= m_accumulatedTime;
    }
    
    auto Timeout::Set(const u32 milliseconds) noexcept -> void
    {
        m_targetTime = static_cast<f32>(milliseconds) / static_cast<f32>(std::milli::den);
    }

    auto Timeout::Clear() noexcept -> void
    {
        m_targetTime = 0.0f;
    }
}
