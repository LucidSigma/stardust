#include "stardust/scripting/ScriptEngine.h"

#include "stardust/application/Application.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace
    {
        struct ApplicationWrapper
        {
            Application* const application = nullptr;

            inline void FinishCurrentScene() noexcept { application->FinishCurrentScene(); }
            inline void ForceQuit() noexcept { application->ForceQuit(); }

            [[nodiscard]] inline f64 GetElapsedTime() const noexcept { return application->GetElapsedTime(); }
        };
    }

    void ScriptEngine::Initialise(Application& application)
    {
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::table);

        SetFunction("finish_current_scene", &ApplicationWrapper::FinishCurrentScene, ApplicationWrapper{ &application });
        SetFunction("force_quit", &ApplicationWrapper::ForceQuit, ApplicationWrapper{ &application });
        SetFunction("get_elapsed_time", &ApplicationWrapper::GetElapsedTime, ApplicationWrapper{ &application });

        SetFunction("log_info", &Log::Info<>);
        SetFunction("log_debug", &Log::Debug<>);
        SetFunction("log_trace", &Log::Trace<>);
        SetFunction("log_warn", &Log::Warn<>);
        SetFunction("log_error", &Log::Error<>);
        SetFunction("log_critical", &Log::Critical<>);

        LoadVectorType<Vec2, sol::constructors<Vec2(), Vec2(f32), Vec2(f32, f32), Vec2(const Vec2&)>>();
        LoadVectorType<Vec3, sol::constructors<Vec3(), Vec3(f32), Vec3(f32, f32, f32), Vec3(const Vec3&)>>();
        LoadVectorType<Vec4, sol::constructors<Vec4(), Vec4(f32), Vec4(f32, f32, f32, f32), Vec4(const Vec4&)>>();
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