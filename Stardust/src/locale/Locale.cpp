#include "stardust/locale/Locale.h"

#include <utility>

#include "stardust/filesystem//vfs/VFS.h"

namespace stardust
{
	void Locale::Initialise(const StringView& baseLocaleDirectory)
	{
		m_baseLocaleDirectory = baseLocaleDirectory;
	}

	[[nodiscard]] Status Locale::SetLocale(const String& localeName)
	{
		const String localeDirectory = m_baseLocaleDirectory + "/" + localeName;

		if (!vfs::IsDirectory(localeDirectory))
		{
			return Status::Fail;
		}

		const auto localeFiles = vfs::GetAllFilesInDirectory(localeDirectory);

		if (localeFiles.empty())
		{
			return Status::Fail;
		}

		nlohmann::json localeAccumulator{ };

		for (const auto& localeFile : localeFiles)
		{
			if (vfs::GetFileExtension(localeFile) == ".json")
			{
				const auto localeSubdata = LoadLocaleFile(localeFile);

				if (!localeSubdata.has_value())
				{
					return Status::Fail;
				}

				localeAccumulator.merge_patch(*localeSubdata);

				if (localeAccumulator.is_discarded())
				{
					return Status::Fail;
				}
			}
		}

		m_currentLocale = std::move(localeAccumulator);
		m_currentLocaleName = localeName;

		return Status::Success;
	}

	[[nodiscard]] Optional<nlohmann::json> Locale::LoadLocaleFile(const String& filepath) const
	{
		const Vector<ubyte> localeData = vfs::ReadFileData(filepath);

		if (localeData.empty())
		{
			return NullOpt;
		}

		const nlohmann::json locale = nlohmann::json::parse(
			reinterpret_cast<const unsigned char*>(localeData.data()),
			reinterpret_cast<const unsigned char*>(localeData.data()) + localeData.size(),
			nullptr,
			false
		);

		if (locale.is_discarded())
		{
			return NullOpt;
		}
		else
		{
			return locale;
		}
	}
}