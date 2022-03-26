project "harfbuzz"
    kind "StaticLib"
    language "C++"
    warnings "Off"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/harfbuzz.cc",
    }

    includedirs {
        "%{dependency_includes.FreeType}",
        "%{dependency_includes.HarfBuzz}",
        "%{dependency_includes.HarfBuzz}/harfbuzz",
        "%{dependency_includes.HarfBuzz}../src",
        "%{dependency_includes.ICU}",
        "%{dependency_includes.ICU}/icu",
    }

    links {
        "freetype",
        "icu-common",
        "icu-data",
    }

    defines {
        "_CRT_NONSTDC_NO_WARNINGS",
        "_CRT_SECURE_NO_WARNINGS",
        "HAVE_FREETYPE=1",
        "HAVE_ICU",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
