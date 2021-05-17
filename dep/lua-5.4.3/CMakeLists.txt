cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(LUA_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(LUA_INCLUDE_DIRS "${LUA_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE LUA_SOURCE_FILES "src/*.c")

add_library(lua ${LUA_SOURCE_FILES})

set_target_properties(lua PROPERTIES FOLDER "Dependencies")

target_include_directories(lua
	PUBLIC ${LUA_INCLUDE_DIRS}
	PRIVATE ${LUA_INCLUDE_DIRS}/lua
)