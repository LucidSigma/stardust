#include "stardust/scripting/ScriptEngine.h"

#include "stardust/application/Application.h"
#include "stardust/debug/logging/Logging.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace
    {
        auto LoadLoggingFunctions(sol::state& luaState, Application& application) -> void
        {
            auto logNamespace = luaState["log"].get_or_create<Table>();
            logNamespace.set_function("info", &Log::Info<>);
            logNamespace.set_function("debug", &Log::Debug<>);
            logNamespace.set_function("trace", &Log::Trace<>);
            logNamespace.set_function("warn", &Log::Warn<>);
            logNamespace.set_function("error", &Log::Error<>);
            logNamespace.set_function("critical", &Log::Critical<>);
        }

        auto LoadTimeFunctions(sol::state& luaState, Application& application) -> void
        {
            auto timeNamespace = luaState["time"].get_or_create<Table>();

            timeNamespace.set_function("elapsed_time", [application = &application]() -> f64 { return application->GetElapsedTime(); });
            timeNamespace.set_function("delta_time", [application = &application]() -> f32 { return static_cast<f32>(application->GetTimestepController().GetDeltaTime()); });
            timeNamespace.set_function("fixed_timestep", [application = &application]() -> f32 { return static_cast<f32>(application->GetTimestepController().GetFixedTimestep()); });
        }

        auto LoadSceneFunctions(sol::state& luaState, Application& application) -> void
        {
            auto sceneNamespace = luaState["scene"].get_or_create<Table>();

            sceneNamespace.set_function("is_current_finished", [application = &application]() -> bool { return application->GetSceneManager().CurrentScene()->IsFinished(); });
            sceneNamespace.set_function("finish_current", [application = &application]() { application->GetSceneManager().CurrentScene()->Finish(); });
        }
    }

    auto ScriptEngine::LoadApplicationFunctions(Application& application) -> void
    {
        auto applicationNamespace = m_luaState["application"].get_or_create<Table>();

        applicationNamespace.set_function("force_quit", [application = &application]() { application->ForceQuit(); });
        applicationNamespace.set_function("is_running", [application = &application]() -> bool { return application->IsRunning(); });

        LoadLoggingFunctions(m_luaState, application);
        LoadTimeFunctions(m_luaState, application);
        LoadSceneFunctions(m_luaState, application);
    }
}
