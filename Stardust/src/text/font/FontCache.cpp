#include "stardust/text/font/FontCache.h"

namespace stardust
{
	FontCache::FontCache(const StringView& fontFilepath)
		: m_filepath(fontFilepath)
	{ }

	void FontCache::SetFont(const StringView& fontFilepath)
	{
		m_filepath = fontFilepath;
	}

	[[nodiscard]] Status FontCache::Add(const FontSize pointSize)
	{
		if (!m_pointSizes.contains(pointSize))
		{
			m_pointSizes.emplace(pointSize, Font(m_filepath, pointSize));
		}

		return m_pointSizes[pointSize].IsValid() ? Status::Success : Status::Fail;
	}

	[[nodiscard]] Status  FontCache::Add(const Vector<FontSize>& pointSizes)
	{
		for (const FontSize pointSize : pointSizes)
		{
			if (Add(pointSize) != Status::Success)
			{
				return Status::Fail;
			}
		}

		return Status::Success;
	}

	[[nodiscard]] Font& FontCache::Get(const FontSize pointSize)
	{
		return m_pointSizes[pointSize];
	}

	void FontCache::Remove(const FontSize pointSize)
	{
		m_pointSizes.erase(pointSize);
	}
}