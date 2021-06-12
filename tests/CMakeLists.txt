cmake_minimum_required(VERSION 3.15)

add_executable(colour_test "colour/src/Main.cpp")
add_executable(config_test "config/src/Main.cpp")
add_executable(fs_test "filesystem/src/Main.cpp")
add_executable(graph_test "graph/src/Main.cpp")
add_executable(rect_test "rect/src/Main.cpp")
add_executable(string_test "string/src/Main.cpp")

set(TEST_TARGETS colour_test config_test fs_test graph_test rect_test string_test)

foreach(TEST_TARGET ${TEST_TARGETS})
    target_include_directories(${TEST_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/Stardust/include" ${CATCH2_INCLUDE_DIRS})
    target_link_libraries(${TEST_TARGET} PUBLIC Stardust)
endforeach()

set_target_properties(
    ${TEST_TARGETS}
	PROPERTIES
    FOLDER "Tests"
    ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Tests
    LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Tests
	RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Tests
)

add_custom_command(TARGET config_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/config/config.json" $<TARGET_FILE_DIR:config_test>/config
)

add_custom_command(TARGET fs_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/filesystem/fs_resources.zip" $<TARGET_FILE_DIR:fs_test>
)

add_custom_command(TARGET fs_test POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/filesystem/resources $<TARGET_FILE_DIR:fs_test>/fs_resources
)

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET ${TEST_TARGETS} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:colour_test>
        )
    endforeach()

    foreach(SDL2_TTF_DLL ${SDL2_TTF_DLLS})
        add_custom_command(TARGET ${TEST_TARGETS} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:colour_test>
        )
    endforeach()

    foreach(RMLUI_DLL ${RMLUI_DLLS})
        add_custom_command(TARGET ${TEST_TARGETS} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RMLUI_DLL} $<TARGET_FILE_DIR:colour_test>
        )
    endforeach()
endif()