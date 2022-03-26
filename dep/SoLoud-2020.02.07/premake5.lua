project "soloud"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/audiosource/**.h",
        "src/audiosource/**.c",
        "src/audiosource/**.cpp",
        "src/backend/sdl2_static/**.cpp",
        "src/core/**.cpp",
        "src/filter/**.cpp",
    }

    includedirs {
        "%{dependency_includes.SoLoud}",
        "%{dependency_includes.SoLoud}/soloud",
        "%{dependency_includes.SDL2}/SDL2",
    }

    libdirs {
        "%{dependency_sources.SDL2}",
    }

    links {
        "SDL2",
    }

    defines {
        "WITH_SDL2_STATIC",
    }

    filter "system:windows"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
