cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TMX_LITE_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(TMX_LITE_INCLUDE_DIRS "${TMX_LITE_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE TMX_LITE_SOURCE_FILES "src/*.cpp" "src/*.c" "src/detail/*.cpp")

add_library(tmxlite ${TMX_LITE_SOURCE_FILES})

set_target_properties(tmxlite PROPERTIES FOLDER "Dependencies")
target_include_directories(tmxlite PUBLIC ${TMX_LITE_INCLUDE_DIRS})