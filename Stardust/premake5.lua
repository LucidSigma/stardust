STARDUST_INCLUDE_DIRECTORY = "%{wks.location}/../Stardust/include"

project "Stardust"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    vpaths {
        ["Source/*"] = {
            "include/stardust/**",
            "src/**",
        },
    }

    includedirs {
        "include",
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
        "%{dependency_includes.tomlplusplus}",
        "%{dependency_includes.utfcpp}",
    }

    libdirs {
        "%{dependency_sources.SDL2}",
    }

    links {
        "box2d",
        "freetype",
        "freetype-gl",
        "harfbuzz",
        "icu-common",
        "icu-data",
        "libEGL",
        "libGLESv2",
        "lua",
        "physfs",
        "pugixml",
        "SDL2",
        "SDL2main",
        "soloud",
        "spdlog",
        "stb_image",
        "stb_image_write",
        "STX",
    }

    filter "action:vs*"
        buildoptions { "/bigobj" }
        linkoptions { "/IGNORE:4006" }

    filter "configurations:Debug"
        libdirs { "%{dependency_sources.ANGLE.debug}" }
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
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
