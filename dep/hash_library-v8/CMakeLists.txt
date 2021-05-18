cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(HASH_LIBRARY_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(HASH_LIBRARY_INCLUDE_DIRS "${HASH_LIBRARY_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE HASH_LIBRARY_SOURCE_FILES "src/*.cpp")

add_library(hash_library ${HASH_LIBRARY_SOURCE_FILES})

set_target_properties(hash_library PROPERTIES FOLDER "Dependencies")

target_include_directories(hash_library
	PUBLIC ${HASH_LIBRARY_INCLUDE_DIRS}
	PRIVATE ${HASH_LIBRARY_INCLUDE_DIRS}/hash
)