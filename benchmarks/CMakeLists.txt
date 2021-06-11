cmake_minimum_required(VERSION 3.15)

add_executable(string_benchmark "string/src/Main.cpp")

set(BENCHMARK_TARGETS string_benchmark)

foreach(BENCHMARK_TARGET ${BENCHMARK_TARGETS})
    target_include_directories(${BENCHMARK_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/Stardust/include" ${BENCHMARK_INCLUDE_DIRS})
    target_link_libraries(${BENCHMARK_TARGET} PUBLIC Stardust)
endforeach()

set_target_properties(
    ${BENCHMARK_TARGETS}
	PROPERTIES
    FOLDER "Benchmarks"
    ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Benchmarks
    LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Benchmarks
	RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Benchmarks
)