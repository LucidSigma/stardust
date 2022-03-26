project "spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++14"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.spdlog}",
    }

    defines { 
        "SPDLOG_COMPILED_LIB",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
