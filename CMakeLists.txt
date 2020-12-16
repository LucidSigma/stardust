cmake_minimum_required(VERSION 3.15)

project(StardustMock LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(WIN32)
	set(SDL2_DIR "${PROJECT_SOURCE_DIR}/dep/SDL2-2.0.12/cmake")
endif()

find_package(SDL2 REQUIRED)

add_subdirectory("Stardust")
add_subdirectory("Sandbox")