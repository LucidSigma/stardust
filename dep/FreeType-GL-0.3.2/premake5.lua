project "freetype-gl"
    kind "StaticLib"
    language "C++"
    warnings "Off"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/**.c",
    }

    includedirs { 
        "%{dependency_includes[\"FreeType-GL\"]}",
        "%{dependency_includes[\"FreeType-GL\"]}/ftgl",
        "%{dependency_includes.FreeType}",
    }

    links {
        "freetype",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
