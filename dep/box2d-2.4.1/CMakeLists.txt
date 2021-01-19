cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(BOX2D_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(BOX2D_INCLUDE_DIRS "${BOX2D_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE BOX2D_SOURCE_FILES "src/*.cpp")

add_library(box2d ${BOX2D_SOURCE_FILES})

set_target_properties(box2d PROPERTIES FOLDER "Dependencies")

target_include_directories(box2d
	PUBLIC ${BOX2D_INCLUDE_DIRS}
	PRIVATE "${_IMPORT_PREFIX}/src"
)