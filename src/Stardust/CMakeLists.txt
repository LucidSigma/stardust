cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/Stardust/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/Stardust/*.cpp")

add_library(Stardust ${STARDUST_SOURCE_FILES} ${STARDUST_HEADER_FILES})

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src/Stardust" FILES ${STARDUST_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${STARDUST_HEADER_FILES})