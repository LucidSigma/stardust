cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/stardust/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/*.cpp")

add_library(Stardust ${STARDUST_SOURCE_FILES} ${STARDUST_HEADER_FILES})

target_include_directories(Stardust PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/include"
	${DEBUG_ASSERT_INCLUDE_DIRS}
	${EA_BASE_INCLUDE_DIRS}
	${EASTL_INCLUDE_DIRS}
	${GLAD_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
	${TINY_UTF8_INCLUDE_DIRS}
	${TYPE_SAFE_INCLUDE_DIRS}
)

target_link_directories(Stardust PUBLIC
	${SDL2_LIBS_DIR}
)

target_link_libraries(Stardust PUBLIC
	${SDL2_LIBS}
	OpenGL::GL
	EASTL
	glad
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${STARDUST_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include/stardust" FILES ${STARDUST_HEADER_FILES})

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Stardust POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Stardust>)
    endforeach()
endif()