project "global_resources_test"
    language "C++"
    cppdialect "C++20"

    targetdir "%{BUILD_DIRECTORY}/bin/tests/%{cfg.buildcfg}/unit"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "src/**.cpp",
    }

    vpaths {
        ["*"] = {
            "src/**",
        },
    }

    includedirs {
        "%{STARDUST_INCLUDE_DIRECTORY}",
        "%{dependency_includes.ANGLE}",
        "%{dependency_includes.ANGLE}/ANGLE",
        "%{dependency_includes.Box2D}",
        "%{dependency_includes.Catch2}",
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
        "Stardust",
        "SDL2",
        "SDL2main",
    }

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
