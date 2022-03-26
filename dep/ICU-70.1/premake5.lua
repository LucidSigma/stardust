local ICU_MAJOR_VERSION = string.match(DEPENDENCIES.ICU.version, "([^.]+)")

project "icu-data"
    kind "SharedLib"
    language "C++"

    targetname(string.format("icudt%s", ICU_MAJOR_VERSION))

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/stubdata/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.ICU}",
        "%{dependency_includes.ICU}/icu",
    }

    defines {
        " _CRT_SECURE_NO_WARNINGS",
        "TRUE=1",
        "FALSE=0",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"

project "icu-common"
    kind "SharedLib"
    language "C++"
    warnings "Off"

    targetname(string.format("icuuc%s", ICU_MAJOR_VERSION))

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.h",
        "src/common/**.h",
        "src/common/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.ICU}",
        "%{dependency_includes.ICU}/icu",
        "%{dependency_includes.ICU}/../src/common",
    }

    links {
        "icu-data",
    }

    defines {
        " _CRT_SECURE_NO_WARNINGS",
        "U_COMMON_IMPLEMENTATION",
        "TRUE=1",
        "FALSE=0",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
