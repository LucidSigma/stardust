cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SANDBOX_HEADER_FILES "include/*.h")
file(GLOB_RECURSE SANDBOX_SOURCE_FILES "src/*.cpp")

add_executable(vfs_test "vfs/src/Main.cpp")

target_include_directories(vfs_test PUBLIC "${PROJECT_SOURCE_DIR}/Stardust/include" ${CATCH2_INCLUDE_DIRS})
target_link_libraries(vfs_test PUBLIC Stardust)

set_target_properties(vfs_test PROPERTIES FOLDER "Tests")

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET vfs_test POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:vfs_test>
        )
    endforeach()
endif()