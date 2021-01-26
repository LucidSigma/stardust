#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <functional>
#include <type_traits>
#include <utility>

#include <sol/sol.hpp>

#include "stardust/data/Containers.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
	constexpr sol::nil_t Nil = sol::nil;

	class ScriptEngine
	{
	private:
		sol::state m_luaState{ };

	public:
		ScriptEngine() = default;
		~ScriptEngine() noexcept = default;

		void Initialise(class Application& application);

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
			m_luaState.create_named_table(name, std::forward<Args>(values)...);
		}

		template <typename T>
		[[nodiscard]] inline std::function<T> GetFunction(const StringView& functionName) const
		{
			return static_cast<std::function<T>>(sol::function(m_luaState[functionName]));
		}

		template <typename... Args>
		void SetFunction(const StringView& functionName, Args&&... values)
		{
			m_luaState.set_function(functionName, values...);
		}

		template <typename Result, typename... Args>
		[[nodiscard]] Result CallFunction(const StringView& name, Args&&... args) const
		{
			if constexpr (std::is_same_v<Result, void>)
			{
				GetFunction<Result(Args...)>(name)(args...);
			}
			else
			{
				return GetFunction<Result(Args...)>(name)(args...);
			}
		}

		inline decltype(auto) operator [](const StringView& variableName) { return m_luaState[variableName]; }

		inline sol::state& GetState() noexcept { return m_luaState; }
		inline const sol::state& GetState() const noexcept { return m_luaState; }
	};
}

#endif