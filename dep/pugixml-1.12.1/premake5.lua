project "pugixml"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"

    targetdir "%{BUILD_DIRECTORY}/bin/%{cfg.buildcfg}"
    objdir "%{BUILD_DIRECTORY}/bin/obj/%{cfg.buildcfg}"

    files {
        "include/**.hpp",
        "src/**.cpp",
    }

    includedirs { 
        "%{dependency_includes.pugixml}",
        "%{dependency_includes.pugixml}/pugixml",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
