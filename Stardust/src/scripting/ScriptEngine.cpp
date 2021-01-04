#include "stardust\scripting\ScriptEngine.h"

#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	void ScriptEngine::Initialise()
	{
		m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::table);
	}

	[[nodiscard]] Status ScriptEngine::LoadScript(const StringView& filename)
	{
		const Vector<ubyte> scriptData = vfs::ReadFileData(filename);

		if (scriptData.empty())
		{
			return Status::Fail;
		}

		const String scriptString(
			reinterpret_cast<const char*>(scriptData.data()),
			reinterpret_cast<const char*>(scriptData.data() + scriptData.size())
		);

		const auto scriptResult = m_luaState.safe_script(scriptString, sol::script_pass_on_error);

		return scriptResult.valid() ? Status::Success : Status::Fail;
	}
}