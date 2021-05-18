cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE STARDUST_HEADER_FILES "include/stardust/*.h")
file(GLOB_RECURSE STARDUST_SOURCE_FILES "src/*.cpp")

add_library(Stardust ${STARDUST_SOURCE_FILES} ${STARDUST_HEADER_FILES})

target_include_directories(Stardust PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    ${BOX2D_INCLUDE_DIRS}
    ${CONCURRENT_QUEUE_INCLUDE_DIRS}
    ${ENTT_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${MAGIC_ENUM_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIRS}
    ${PHYSFS_INCLUDE_DIRS}
    ${PSCHED_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SOL2_INCLUDE_DIRS}
    ${SOLOUD_INCLUDE_DIRS}
    ${SPDLOG_INCLUDE_DIRS}
    ${STB_IMAGE_INCLUDE_DIRS}
)

target_link_directories(Stardust PUBLIC
    ${SDL2_LIBS_DIR}
    ${SDL2_TTF_LIBS_DIR}
)

target_link_libraries(Stardust PUBLIC
    ${SDL2_LIBS}
    ${SDL2_TTF_LIBS}
    OpenGL::GL
    box2d
    glad
    hash_library
    lua
    physfs
    soloud
    stb_image
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${STARDUST_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include/stardust" FILES ${STARDUST_HEADER_FILES})

if(WIN32)
    foreach(SDL2_DLL ${SDL2_DLLS})
        add_custom_command(TARGET Stardust POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:Stardust>
        )
    endforeach()
    
    foreach(SDL2_TTF_DLL ${SDL2_TTF_DLLS})
        add_custom_command(TARGET Stardust POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:Stardust>
        )
    endforeach()
endif()