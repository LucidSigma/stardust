project "Sandbox"
    language "C++"
    cppdialect "C++20"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    pchheader "StardustPCH.h"
    pchsource "src/StardustPCH.cpp"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    vpaths {
        ["Source/*"] = {
            "include/**",
            "src/**",
        },
    }

    includedirs {
        "include",
        "%{STARDUST_INCLUDE_DIRECTORY}",
        "%{dependency_includes.ANGLE}",
        "%{dependency_includes.ANGLE}/ANGLE",
        "%{dependency_includes.Box2D}",
        "%{dependency_includes.EnTT}",
        "%{dependency_includes.FreeType}",
        "%{dependency_includes[\"FreeType-GL\"]}",
        "%{dependency_includes.glm}",
        "%{dependency_includes.HarfBuzz}",
        "%{dependency_includes.HarfBuzz}/harfbuzz",
        "%{dependency_includes.ICU}",
        "%{dependency_includes.ICU}/icu",
        "%{dependency_includes.lua}",
        "%{dependency_includes.magic_enum}",
        "%{dependency_includes[\"nlohmann-json\"]}",
        "%{dependency_includes.physfs}",
        "%{dependency_includes.pugixml}",
        "%{dependency_includes.SDL2}",
        "%{dependency_includes.SDL2}/SDL2",
        "%{dependency_includes.sol2}",
        "%{dependency_includes.SoLoud}",
        "%{dependency_includes.spdlog}",
        "%{dependency_includes.stb_image}",
        "%{dependency_includes.stb_image_write}",
        "%{dependency_includes.STX}",
        "%{dependency_includes[\"tl-generator\"]}",
        "%{dependency_includes[\"thread-pool\"]}",
        "%{dependency_includes.tomlplusplus}",
        "%{dependency_includes.utfcpp}",
    }

    libdirs {
        "%{dependency_sources.SDL2}",
    }

    links {
        "Stardust",
        "SDL2",
        "SDL2main",
    }

    filter "configurations:Debug"
        kind "ConsoleApp"
        libdirs { "%{dependency_sources.ANGLE.debug}" }
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        libdirs { "%{dependency_sources.ANGLE.release}" }
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"

    filter "system:windows"
        defines { "STARDUST_PLATFORM_WINDOWS" }

    filter "system:macosx"
        defines { "STARDUST_PLATFORM_MACOS" }

    filter "system:linux"
        defines { "STARDUST_PLATFORM_LINUX" }
