#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <sol/sol.hpp>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

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

		[[nodiscard]] Status LoadScript(const StringView& filename);

		template <typename T>
		[[nodiscard]] T Get(const StringView& variableName)
		{
			return m_luaState[variableName];
		}

		template <typename T>
		void Set(const StringView& variableName, const T& value)
		{
			m_luaState[variableName] = value;
		}
	};
}

#endif