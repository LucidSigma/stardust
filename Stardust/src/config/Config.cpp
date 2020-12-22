#include "stardust/config/Config.h"

#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/filesystem/Filesystem.h"

namespace stardust
{
	[[nodiscard]] Status Config::Initialise(const StringView& configFilepath)
	{
		const String configFileData = filesystem::ReadFile(configFilepath);
		m_data = nlohmann::json::parse(configFileData, nullptr, false);
		m_filepath = configFilepath;

		return m_data.is_discarded() ? Status::Fail : Status::Success;
	}

	Status Config::Save() const
	{
		return filesystem::WriteToFile(m_filepath, m_data);
	}
}