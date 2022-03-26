DEPENDENCIES = {
    ANGLE = {
        version = "2.1.0.5bd4596",
        is_precompiled = true,
        has_separate_build_directories = true,
    },
    Box2D = {
        version = "2.4.1",
        has_premake_file = true,
    },
    Catch2 = {
        version = "2.13.8",
    },
    EnTT = {
        version = "3.9.0",
    },
    FreeType = {
        version = "2.11.1",
        has_premake_file = true,
    },
    ["FreeType-GL"] = {
        version = "0.3.2",
        has_premake_file = true,
    },
    glm = {
        version = "0.9.9.8",
    },
    HarfBuzz = {
        version = "4.1.0",
        has_premake_file = true,
    },
    ICU = {
        version = "70.1",
        has_premake_file = true,
    },
    lua = {
        version = "5.4.4",
        has_premake_file = true,
    },
    magic_enum = {
        version = "0.7.3",
    },
    ["nlohmann-json"] = {
        version = "3.10.5",
    },
    physfs = {
        version = "3.0.2",
        has_premake_file = true,
    },
    pugixml = {
        version = "1.12.1",
        has_premake_file = true,
    },
    SDL2 = {
        version = "2.0.20",
        is_precompiled = true,
    },
    sol2 = {
        version = "3.2.2",
    },
    SoLoud = {
        version = "2020.02.07",
        has_premake_file = true,
    },
    spdlog = {
        version = "1.9.2",
        has_premake_file = true,
    },
    stb_image = {
        version = "2.27",
        has_premake_file = true,
    },
    stb_image_write = {
        version = "1.16",
        has_premake_file = true,
    },
    STX = {
        version = "1.0.1",
        has_premake_file = true,
    },
    ["tl-generator"] = {
        version = "0.3.0",
    },
    ["thread-pool"] = {
        version = "2.0.0",
    },
    tomlplusplus = {
        version = "3.0.1",
    },
    utfcpp = {
        version = "3.2.1",
    },
}

DEPENDENCIES_LOCATION = "%{wks.location}/../dep/"

dependency_includes = { }
dependency_sources = { }

dependencies_to_compile = { }

for dependency, data in pairs(DEPENDENCIES) do
    dependency_includes[dependency] = DEPENDENCIES_LOCATION .. dependency .. "-" .. data.version .. "/include"

    if data.is_precompiled then
        if data.has_separate_build_directories then
            dependency_sources[dependency] = {
                debug = DEPENDENCIES_LOCATION .. dependency .. "-" .. data.version .. "/lib/Debug",
                release = DEPENDENCIES_LOCATION .. dependency .. "-" .. data.version .. "/lib/Release",
            }
        else
            dependency_sources[dependency] = DEPENDENCIES_LOCATION .. dependency .. "-" .. data.version .. "/lib"
        end
    end

    if data.has_premake_file then
        dependencies_to_compile[dependency] = data.version
    end
end

group "Dependencies"
    for dependency, version in pairs(dependencies_to_compile) do
        include (dependency .. "-" .. version)
    end
group ""

dependencies_to_compile = nil
