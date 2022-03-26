#include "stardust/text/font/FontCache.h"

#include <memory>

namespace stardust
{
    FontCache::FontCache(const Font::CreateInfo& createInfo)
    {
        SetFontData(createInfo);
    }
    
    auto FontCache::SetFontData(const Font::CreateInfo& createInfo) -> void
    {
        m_createInfo = createInfo;
    }

    [[nodiscard]] auto FontCache::Add(const Font::Size pointSize) -> Status
    {
        if (!m_pointSizes.contains(pointSize))
        {
            m_createInfo.pointSize = pointSize;
            m_pointSizes[pointSize] = std::make_unique<Font>(m_createInfo);
        }

        return m_pointSizes[pointSize]->IsValid() ? Status::Success : Status::Fail;
    }

    [[nodiscard]] auto FontCache::Add(const List<Font::Size>& pointSizes) -> Status
    {
        for (const Font::Size pointSize : pointSizes)
        {
            if (Add(pointSize) != Status::Success)
            {
                return Status::Fail;
            }
        }

        return Status::Success;
    }

    [[nodiscard]] auto FontCache::Get(const Font::Size pointSize) -> Font&
    {
        return *m_pointSizes[pointSize];
    }

    auto FontCache::Remove(const Font::Size pointSize) -> void
    {
        m_pointSizes.erase(pointSize);
    }
}
