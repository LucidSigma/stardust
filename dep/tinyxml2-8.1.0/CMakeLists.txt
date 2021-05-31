cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TINYXML2_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(TINYXML2_INCLUDE_DIRS "${TINYXML2_INCLUDE_DIRS}" PARENT_SCOPE)

add_library(tinyxml2 "src/tinyxml2.cpp")

set_target_properties(tinyxml2 PROPERTIES FOLDER "Dependencies")
target_include_directories(tinyxml2
	PUBLIC ${TINYXML2_INCLUDE_DIRS}
	PRIVATE ${TINYXML2_INCLUDE_DIRS}/tinyxml2
)