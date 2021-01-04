#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <type_traits>
#include <utility>

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
		[[nodiscard]] inline T Get(const StringView& variableName)
		{
			return m_luaState.get<T>(variableName);
		}

		template <typename T>
		inline void Set(const StringView& variableName, const T& value)
		{
			m_luaState[variableName] = value;
		}

		template <typename... Args>
		inline void CreateTable(const StringView& name, Args&&... values)
		{
			m_luaState.create_named_table(name, values);
		}

		inline decltype(auto) operator [](const StringView& variableName) { return m_luaState[variableName]; }

		inline sol::state& GetState() noexcept { return m_luaState; }
		inline const sol::state& GetState() const noexcept { return m_luaState; }
	};
}

#endif