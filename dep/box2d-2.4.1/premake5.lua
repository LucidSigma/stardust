project "box2d"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.h",
        "src/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.Box2D}",
        "%{dependency_includes.Box2D}/../src",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
