cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/*.cpp")

add_library(Stardust ${STARDUST_SOURCE_FILES} ${STARDUST_HEADER_FILES})

target_include_directories(Stardust PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/include"
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${STARDUST_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${STARDUST_HEADER_FILES})