cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/Sandbox/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/Sandbox/*.cpp")

add_executable(Sandbox ${SANDBOX_SOURCE_FILES} ${SANDBOX_SOURCE_FILES})

target_include_directories(Sandbox PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(Sandbox PUBLIC Stardust)