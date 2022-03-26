#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <functional>
#include <type_traits>
#include <tuple>
#include <utility>

#include <sol/sol.hpp>

#include "stardust/types/Containers.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    constexpr sol::lua_nil_t Nil = sol::lua_nil;

    using Table = sol::table;
    using TableProxy = sol::table_proxy<const sol::basic_table_core<false, sol::reference>&, sol::detail::proxy_key_t<const String&>>;
    using GlobalTableProxy = sol::table_proxy<const sol::basic_table_core<true, sol::reference>&, std::tuple<StringView>>;

    class ScriptEngine final
    {
    private:
        sol::state m_luaState{ };

    public:
        auto Initialise(class Application& application) -> void;

        auto LoadScript(const StringView script) -> Status;
        [[nodiscard]] auto LoadScriptFile(const StringView scriptFilepath) -> Status;

        [[nodiscard]] inline auto Get(const StringView variableName) const -> decltype(auto) { return m_luaState[variableName]; }

        template <typename T>
        [[nodiscard]] inline auto Get(const StringView variableName) const -> T
        {
            return m_luaState.get<T>(variableName);
        }

        template <typename T>
        inline auto Set(const StringView variableName, const T& value) -> void
        {
            m_luaState[variableName] = value;
        }

        template <typename... Args>
        inline auto CreateTable(const StringView name, Args&&... values) -> void
        {
            m_luaState.create_named_table(name, std::forward<Args>(values)...);
        }

        template <typename T>
        [[nodiscard]] inline auto GetFunction(const StringView functionName) const -> std::function<T>
        {
            return static_cast<std::function<T>>(m_luaState[functionName].get<sol::function>());
        }

        template <typename... Args>
        auto SetFunction(const StringView functionName, Args&&... values) -> void
        {
            m_luaState.set_function(functionName, values...);
        }

        template <typename T>
        [[nodiscard]] inline auto GetFromTable(const StringView tableName, const StringView fieldName) const -> T
        {
            return Get<Table>(tableName)[fieldName];
        }

        template <typename T>
        inline auto SetInTable(const StringView tableName, const StringView fieldName, const T& value) -> void
        {
            Get<Table>(tableName)[fieldName] = value;
        }

        template <typename ReturnType>
        [[nodiscard]] auto CallFunction(const StringView name) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                GetFunction<auto() -> ReturnType>(name)();
            }
            else
            {
                return GetFunction<auto() -> ReturnType>(name)();
            }
        }

        template <typename ReturnType, typename... Args>
        [[nodiscard]] auto CallFunction(const StringView name, Args&&... args) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                GetFunction<auto(Args...) -> ReturnType>(name)(std::forward<Args>(args)...);
            }
            else
            {
                return GetFunction<auto(Args...) -> ReturnType>(name)(std::forward<Args>(args)...);
            }
        }

        template <typename ReturnType, typename... Args>
        [[nodiscard]] auto CallFunction(const StringView name, const Args&... args) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                GetFunction<auto(Args...) -> ReturnType>(name)(args...);
            }
            else
            {
                return GetFunction<auto(Args...) -> ReturnType>(name)(args...);
            }
        }

        template <typename ReturnType>
        [[nodiscard]] auto CallFunctionFromTable(const StringView tableName, const StringView functionName) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                Get<Table>(tableName)[functionName].get<sol::function>()();
            }
            else
            {
                return Get<Table>(tableName)[functionName].get<sol::function>()();
            }
        }

        template <typename ReturnType, typename... Args>
        [[nodiscard]] auto CallFunctionFromTable(const StringView tableName, const StringView functionName, Args&&... args) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                Get<Table>(tableName)[functionName].get<sol::function>()(std::forward<Args>(args)...);
            }
            else
            {
                return Get<Table>(tableName)[functionName].get<sol::function>()(std::forward<Args>(args)...);
            }
        }

        template <typename ReturnType, typename... Args>
        [[nodiscard]] auto CallFunctionFromTable(const StringView tableName, const StringView functionName, const Args&... args) const -> ReturnType
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                Get<Table>(tableName)[functionName].get<sol::function>()(args...);
            }
            else
            {
                return Get<Table>(tableName)[functionName].get<sol::function>()(args...);
            }
        }

        [[nodiscard]] inline auto operator [](const StringView variableName) const -> decltype(auto) { return m_luaState[variableName]; }

        [[nodiscard]] inline auto GetState() noexcept -> sol::state& { return m_luaState; }
        [[nodiscard]] inline auto GetState() const noexcept -> const sol::state& { return m_luaState; }

    private:
        auto LoadApplicationFunctions(class Application& application) -> void;
        auto LoadMathFunctions() -> void;
        auto LoadGraphicsFunctions(class Application& application) -> void;
        auto LoadAnimationFunctions() -> void;
        auto LoadUIFunctions() -> void;
    };
}

#endif
