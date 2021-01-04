#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <sol/sol.hpp>

namespace stardust
{
	class ScriptEngine
	{
	private:
		sol::state m_luaState{ };

	public:
		ScriptEngine() = default;
		~ScriptEngine() noexcept = default;

		void Initialise();
	};
}

#endif