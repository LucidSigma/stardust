#include "stardust/config/Config.h"

#include <fstream>
#include <sstream>

#include "stardust/debug/message_box/MessageBox.h"

namespace stardust
{
	[[nodiscard]] Status Config::Initialise(const String& configFilepath)
	{
		std::ifstream configFile(configFilepath.data());

		if (!configFile.is_open())
		{
			return Status::Fail;
		}

		std::ostringstream configStringStream;
		configStringStream << configFile.rdbuf();
		configFile.close();

		m_data = nlohmann::json::parse(configStringStream.str(), nullptr, false);
		m_filepath = configFilepath;

		return m_data.is_discarded() ? Status::Fail : Status::Success;
	}

	void Config::Save() const
	{
		std::ofstream configFile(m_filepath.data(), std::ios_base::out | std::ios_base::trunc);

		if (!configFile.is_open())
		{
			using namespace std::literals::string_literals;

			message_box::Show("Config Warning"s, "Failed to save config file."s, message_box::Type::Warning);

			return;
		}

		configFile << m_data << "\n";
		configFile.close();
	}
}