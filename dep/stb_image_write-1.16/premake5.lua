project "stb_image_write"
    kind "StaticLib"
    language "C++"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.stb_image_write}",
    }

    defines {
        " _CRT_SECURE_NO_WARNINGS",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
