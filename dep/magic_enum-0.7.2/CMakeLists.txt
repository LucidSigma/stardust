cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(MAGIC_ENUM_INCLUDE_DIRS "${_IMPORT_PREFIX}/include" PARENT_SCOPE)