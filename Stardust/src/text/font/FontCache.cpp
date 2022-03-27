#include "stardust/text/font/FontCache.h"

#include <memory>

namespace stardust
{
    FontCache::FontCache(const FontData& fontData)
    {
        Initialise(fontData);
    }
    
    auto FontCache::Initialise(const FontData& fontData) -> void
    {
        m_fontData = fontData;
    }

    [[nodiscard]] auto FontCache::Add(const Font::Size pointSize) -> Status
    {
        if (!m_pointSizes.contains(pointSize))
        {
            m_pointSizes[pointSize] = std::make_unique<Font>(
                Font::CreateInfo{
                    .filepath = m_fontData.filepath,
                    .pointSize = pointSize,
                    .renderMode = m_fontData.renderMode,
                    .outlineThickness = m_fontData.outlineThickness,
                    .textureAtlasSize = m_fontData.textureAtlasSize,
                    .textureAtlasDepth = m_fontData.textureAtlasDepth,
                    .sampler = m_fontData.sampler,
                }
            );
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
