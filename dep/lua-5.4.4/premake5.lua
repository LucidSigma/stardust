project "lua"
    kind "StaticLib"
    language "C"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "include/**.hpp",
        "src/**.c",
    }

    removefiles {
        "src/luac.c",
    }

    includedirs { 
        "%{dependency_includes.lua}",
        "%{dependency_includes.lua}/lua",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
