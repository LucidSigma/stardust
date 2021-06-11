cmake_minimum_required(VERSION 3.15)

# Workspace setup.
project(Stardust LANGUAGES C CXX VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STARDUST_BUILD_SANDBOX "Whether to build the sandbox application or not." OFF)
option(STARDUST_BUILD_TESTS "Whether to build the unit tests or not." OFF)
option(STARDUST_BUILD_BENCHMARKS "Whether to build the benchmarks or not." OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# External CMake modules.
if(WIN32)
    set(SDL2_DIR "${PROJECT_SOURCE_DIR}/dep/SDL2-2.0.14/cmake")
    set(SDL2_TTF_DIR "${PROJECT_SOURCE_DIR}/dep/SDL2_ttf-2.0.15/cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/dep/glm-0.9.9.8/cmake/glm")
find_package(GLM REQUIRED)

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "Could not find OpenGL libraries.")
endif()

# Header-only dependencies.
add_subdirectory("dep/Catch2-2.13.6")
add_subdirectory("dep/cereal-1.3.0")
add_subdirectory("dep/concurrentqueue-1.0.3")
add_subdirectory("dep/entt-3.7.1")
add_subdirectory("dep/fifo_map-1.0.0")
add_subdirectory("dep/nlohmann_json-3.9.1")
add_subdirectory("dep/magic_enum-0.7.2")
add_subdirectory("dep/psched-1.9.0")
add_subdirectory("dep/spdlog-1.8.5")
add_subdirectory("dep/sol2-3.2.2")
add_subdirectory("dep/tomlplusplus-2.4.0")

# Other dependencies.
add_subdirectory("dep/benchmark-1.5.5")
add_subdirectory("dep/box2d-2.4.1")
add_subdirectory("dep/cpu_features-0.6.0")
add_subdirectory("dep/Glad-0.1.34")
add_subdirectory("dep/hash_library-v8")
add_subdirectory("dep/lua-5.4.3")
add_subdirectory("dep/physfs-3.0.2")
add_subdirectory("dep/SoLoud-2020.02.07")
add_subdirectory("dep/stb_image-2.26")
add_subdirectory("dep/tinyxml2-8.1.0")

# Projects in workspace.
add_subdirectory("Stardust")

if (STARDUST_BUILD_SANDBOX)
    add_subdirectory("Sandbox")
endif()

if (STARDUST_BUILD_TESTS)
    message(STATUS "Building tests.")

    add_subdirectory("tests")
endif()

if (STARDUST_BUILD_BENCHMARKS)
    message(STATUS "Building benchmarks.")

    add_subdirectory("benchmarks")
endif()