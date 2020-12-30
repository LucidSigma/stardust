cmake_minimum_required(VERSION 3.15)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(STB_IMAGE_WRITE_INCLUDE_DIRS "${_IMPORT_PREFIX}/include")
set(STB_IMAGE_WRITE_INCLUDE_DIRS "${STB_IMAGE_INCLUDE_DIRS}" PARENT_SCOPE)

add_library(stb_image_write "src/STBImageWriteImplementation.cpp")

set_target_properties(stb_image_write PROPERTIES FOLDER "Dependencies")
target_include_directories(stb_image_write PUBLIC ${STB_IMAGE_WRITE_INCLUDE_DIRS})