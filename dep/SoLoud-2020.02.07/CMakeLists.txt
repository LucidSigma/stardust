cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOLOUD_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(SOLOUD_INCLUDE_DIRS "${SOLOUD_INCLUDE_DIRS}" PARENT_SCOPE)

file(GLOB_RECURSE SOLOUD_SOURCE_FILES
	"src/audiosource/*.h"
	"src/audiosource/*.c"
	"src/audiosource/*.cpp"
	"src/backend/sdl2_static/*.cpp"
	"src/core/*.cpp"
	"src/filter/*.cpp"
)

add_library(soloud ${SOLOUD_SOURCE_FILES})

add_compile_definitions(WITH_SDL2_STATIC)

set_target_properties(soloud PROPERTIES FOLDER "Dependencies")

target_include_directories(soloud
	PUBLIC ${SOLOUD_INCLUDE_DIRS}
	PRIVATE ${SOLOUD_INCLUDE_DIRS}/soloud ${SDL2_INCLUDE_DIRS}/SDL2
)