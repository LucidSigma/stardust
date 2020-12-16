cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/*.cpp")

add_library(Stardust ${STARDUST_SOURCE_FILES} ${STARDUST_HEADER_FILES})

target_include_directories(Stardust PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/include"
	${SDL2_INCLUDE_DIRS}
)

target_link_directories(Stardust PUBLIC ${SDL2_LIBS_DIR})
target_link_libraries(Stardust PUBLIC ${SDL2_LIBS})

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${STARDUST_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${STARDUST_HEADER_FILES})

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Stardust POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Stardust>)
    endforeach()
endif()