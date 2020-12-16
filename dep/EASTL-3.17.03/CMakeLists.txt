cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(EASTL_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(EASTL_INCLUDE_DIRS "${EASTL_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE EASTL_SOURCE_FILES "src/*.cpp")

add_library(EASTL ${EASTL_SOURCE_FILES})

set_target_properties(EASTL PROPERTIES FOLDER "Dependencies")
target_include_directories(EASTL PUBLIC ${EASTL_INCLUDE_DIRS} ${EA_BASE_INCLUDE_DIRS})