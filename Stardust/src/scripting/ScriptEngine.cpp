#include "stardust\scripting\ScriptEngine.h"

namespace stardust
{
	void ScriptEngine::Initialise()
	{
		m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
	}
}