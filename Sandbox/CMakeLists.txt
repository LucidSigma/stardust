cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SANDBOX_HEADER_FILES "include/*.h")
file(GLOB_RECURSE SANDBOX_SOURCE_FILES "src/*.cpp")

add_executable(Sandbox ${SANDBOX_SOURCE_FILES} ${SANDBOX_HEADER_FILES})

target_include_directories(Sandbox PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${PROJECT_SOURCE_DIR}/Stardust/include"
)

target_link_libraries(Sandbox PUBLIC Stardust)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${SANDBOX_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${SANDBOX_HEADER_FILES})

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/resources/assets.pak" $<TARGET_FILE_DIR:Sandbox>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/resources/locales.pak" $<TARGET_FILE_DIR:Sandbox>
)

if(WIN32)
    foreach(SDL2_DLL ${SDL2_DLLS})
        add_custom_command(TARGET Sandbox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:Sandbox>
        )
    endforeach()

    foreach(SDL2_TTF_DLL ${SDL2_TTF_DLLS})
        add_custom_command(TARGET Sandbox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:Sandbox>
        )
    endforeach()

    foreach(RMLUI_DLL ${RMLUI_DLLS})
        add_custom_command(TARGET Sandbox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RMLUI_DLL} $<TARGET_FILE_DIR:Sandbox>
        )
    endforeach()
endif()