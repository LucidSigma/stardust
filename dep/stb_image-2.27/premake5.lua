project "stb_image"
    kind "StaticLib"
    language "C++"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.stb_image}",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
