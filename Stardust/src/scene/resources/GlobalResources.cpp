#include "stardust/scene/resources/GlobalResources.h"

namespace stardust
{
    [[nodiscard]] auto GlobalResources::HasKey(const StringView key) -> bool
    {
        return m_data.contains(key.data());
    }

    auto GlobalResources::Remove(const StringView key) -> void
    {
        m_data.erase(key.data());
    }

    auto GlobalResources::Clear() -> void
    {
        m_data.clear();
    }
}
