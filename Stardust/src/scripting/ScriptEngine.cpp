#include "stardust/scripting/ScriptEngine.h"

#include <lua/lua.h>

#include "stardust/application/Application.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    auto ScriptEngine::Initialise(Application& application) -> void
    {
        m_luaState.open_libraries(
            sol::lib::base,
            sol::lib::coroutine,
            sol::lib::io,
            sol::lib::math,
            sol::lib::package,
            sol::lib::string,
            sol::lib::table,
            sol::lib::utf8
        );

        LoadApplicationFunctions(application);
        LoadMathFunctions();
        LoadGraphicsFunctions(application);
        LoadAnimationFunctions();
        LoadUIFunctions();

        LoadScript(R"LUA(
            function create_set(values)
                local set = { }

                for _, value in ipairs(values) do
                    set[value] = true
                end

                return set
            end
        )LUA");
    }

    auto ScriptEngine::LoadScript(const StringView script) -> Status
    {
        const sol::protected_function_result scriptResult = m_luaState.safe_script(
            script,
    #ifdef NDEBUG
            sol::script_pass_on_error
    #else
            sol::script_throw_on_error
    #endif
        );

        return scriptResult.valid() ? Status::Success : Status::Fail;
    }

    [[nodiscard]] auto ScriptEngine::LoadScriptFile(const StringView scriptFilepath) -> Status
    {
        auto scriptReadResult = vfs::ReadFileString(scriptFilepath);

        if (scriptReadResult.is_err())
        {
            return Status::Fail;
        }

        const String script = std::move(scriptReadResult).unwrap();
        
        return LoadScript(script);
    }
}

template <typename T, typename Handler>
auto sol_lua_check(const sol::types<stardust::Optional<T>> types, lua_State* const L, const stardust::i32 index, Handler&& handler, sol::stack::record& tracking) -> bool
{
    const stardust::i32 absoluteIndex = lua_absindex(L, index);
    const bool success = sol::stack::check<T>(L, absoluteIndex, handler) || sol::stack::check<sol::lua_nil_t>(L, absoluteIndex, handler);
    tracking.use(1);

    return success;
}

template <typename T>
auto sol_lua_get(const sol::types<stardust::Optional<T>> types, lua_State* const L, const stardust::i32 index, sol::stack::record& tracking) -> stardust::Optional<T>
{
    const stardust::i32 absoluteIndex = lua_absindex(L, index);
    const sol::optional<T> getResult = sol::stack::check_get<T>(L, absoluteIndex);
    tracking.use(1);

    if (getResult.has_value())
    {
        return getResult.value();
    }
    else
    {
        return stardust::None;
    }
}

template <typename T>
auto sol_lua_push(lua_State* const L, const stardust::Optional<T>& value) -> stardust::i32
{
    const stardust::i32 amount = value.has_value()
        ? sol::stack::push(L, value.value())
        : sol::stack::push(L, sol::lua_nil);

    return amount;
}
