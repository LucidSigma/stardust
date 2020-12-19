cmake_minimum_required(VERSION 3.15)

# Workspace setup.
project(Stardust LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# External CMake modules.
if(WIN32)
	set(SDL2_DIR "${PROJECT_SOURCE_DIR}/dep/SDL2-2.0.12/cmake")
endif()

find_package(SDL2 REQUIRED)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/dep/glm-0.9.9.8/cmake/glm")
find_package(GLM REQUIRED)

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "Could not find OpenGL libraries.")
endif()

# Header-only dependencies.
add_subdirectory("dep/concurrentqueue-1.0.2")
add_subdirectory("dep/EABase-2.09.05")
add_subdirectory("dep/nlohmann_json-3.9.1")
add_subdirectory("dep/spdlog-1.8.2")

# Other dependencies.
add_subdirectory("dep/EASTL-3.17.03")
add_subdirectory("dep/Glad-0.1.34")
add_subdirectory("dep/physfs-3.0.2")
add_subdirectory("dep/stb_image-2.26")

# Projects in workspace.
add_subdirectory("Stardust")
add_subdirectory("Sandbox")