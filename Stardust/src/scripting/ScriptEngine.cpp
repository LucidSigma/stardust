#include "stardust/scripting/ScriptEngine.h"

#include "stardust/application/Application.h"
#include "stardust/data/Types.h"
#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
	namespace
	{
		struct ApplicationWrapper
		{
			const stardust::Application* const application = nullptr;

			inline stardust::f64 GetElapsedTime() const noexcept { return application->GetElapsedTime(); }
		};
	}

	void ScriptEngine::Initialise(const Application& application)
	{
		m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::table);

		SetFunction("get_elapsed_time", &ApplicationWrapper::GetElapsedTime, ApplicationWrapper{ &application });

		SetFunction("log_info", &Log::Info<>);
		SetFunction("log_debug", &Log::Debug<>);
		SetFunction("log_trace", &Log::Trace<>);
		SetFunction("log_warn", &Log::Warn<>);
		SetFunction("log_error", &Log::Error<>);
		SetFunction("log_critical", &Log::Critical<>);
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