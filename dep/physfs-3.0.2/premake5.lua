project "physfs"
    kind "StaticLib"
    language "C"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.c",
    }

    includedirs { 
        "%{dependency_includes.physfs}",
        "%{dependency_includes.physfs}/physfs",
    }

    filter "system:macosx"
        files { "src/**.m" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
