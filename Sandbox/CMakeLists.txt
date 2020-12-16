cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SANDBOX_HEADER_FILES "include/*.h")
file(GLOB_RECURSE SANDBOX_SOURCE_FILES "src/*.cpp")

add_executable(Sandbox ${SANDBOX_SOURCE_FILES} ${SANDBOX_HEADER_FILES})

target_include_directories(Sandbox PUBLIC "${PROJECT_SOURCE_DIR}/Stardust/include")
target_link_libraries(Sandbox PUBLIC Stardust)

target_include_directories(Sandbox PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/include"
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${SANDBOX_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${SANDBOX_HEADER_FILES})