project "stx"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.cc",
    }

    removefiles "src/backtrace.cc"

    includedirs {
        "%{dependency_includes.STX}",
    }

    filter "configurations:Debug"
        defines { "STX_ENABLE_DEBUG_ASSERTIONS" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
