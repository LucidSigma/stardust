cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(GLAD_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(GLAD_INCLUDE_DIRS "${GLAD_INCLUDE_DIRS}" PARENT_SCOPE)

add_library(glad "src/glad.c")

set_target_properties(glad PROPERTIES FOLDER "Dependencies")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIRS})