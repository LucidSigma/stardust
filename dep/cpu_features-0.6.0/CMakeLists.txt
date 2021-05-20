cmake_minimum_required(VERSION 3.0)

# option() honors normal variables.
# see: https://cmake.org/cmake/help/git-stage/policy/CMP0077.html
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(CpuFeatures VERSION 0.6.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Default Build Type to be Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# BUILD_TESTING is a standard CMake variable, but we declare it here to make it
# prominent in the GUI.
option(BUILD_TESTING "Enable test (depends on googletest)." OFF)
# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to make
# it prominent in the GUI.
# cpu_features uses bit-fields which are - to some extends - implementation-defined (see https://en.cppreference.com/w/c/language/bit_field).
# As a consequence it is discouraged to use cpu_features as a shared library because different compilers may interpret the code in different ways.
# Prefer static linking from source whenever possible.
option(BUILD_SHARED_LIBS "Build library as shared." OFF)
# PIC
option(BUILD_PIC "Build with Position Independant Code." OFF) # Default is off at least for GCC

# Force PIC on unix when building shared libs
# see: https://en.wikipedia.org/wiki/Position-independent_code
if(BUILD_SHARED_LIBS AND UNIX)
  set(BUILD_PIC ON)
endif()

include(CheckincludeFile)
include(CheckSymbolExists)
include(GNUInstallDirs)

macro(setup_include_and_definitions TARGET_NAME)
  target_include_directories(${TARGET_NAME}
    PUBLIC  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cpu_features>
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cpu_features/internal>
  )
  target_compile_definitions(${TARGET_NAME}
    PUBLIC STACK_LINE_READER_BUFFER_SIZE=1024
  )
endmacro()

set(PROCESSOR_IS_MIPS FALSE)
set(PROCESSOR_IS_ARM FALSE)
set(PROCESSOR_IS_AARCH64 FALSE)
set(PROCESSOR_IS_X86 FALSE)
set(PROCESSOR_IS_POWER FALSE)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^mips")
  set(PROCESSOR_IS_MIPS TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  set(PROCESSOR_IS_ARM TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  set(PROCESSOR_IS_AARCH64 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
  set(PROCESSOR_IS_X86 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)")
  set(PROCESSOR_IS_POWER TRUE)
endif()

macro(add_cpu_features_headers_and_sources HDRS_LIST_NAME SRCS_LIST_NAME)
  list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpu_features_macros.h)
  list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpu_features_cache_info.h)
  if(PROCESSOR_IS_MIPS)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpuinfo_mips.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_mips.c)
  elseif(PROCESSOR_IS_ARM)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpuinfo_arm.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_arm.c)
  elseif(PROCESSOR_IS_AARCH64)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpuinfo_aarch64.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_aarch64.c)
  elseif(PROCESSOR_IS_X86)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpuinfo_x86.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/internal/cpuid_x86.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_x86.c)
  elseif(PROCESSOR_IS_POWER)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features/cpuinfo_ppc.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_ppc.c)
  else()
    message(FATAL_ERROR "Unsupported architectures ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
endmacro()

set (CPU_FEATURES_SRCS)
set (CPU_FEATURES_HDRS)

list(APPEND CPU_FEATURES_SRCS
  ${PROJECT_SOURCE_DIR}/include/cpu_features/internal/bit_utils.h
  ${PROJECT_SOURCE_DIR}/include/cpu_features/internal/filesystem.h
  ${PROJECT_SOURCE_DIR}/include/cpu_features/internal/stack_line_reader.h
  ${PROJECT_SOURCE_DIR}/include/cpu_features/internal/string_view.h
  ${PROJECT_SOURCE_DIR}/src/filesystem.c
  ${PROJECT_SOURCE_DIR}/src/stack_line_reader.c
  ${PROJECT_SOURCE_DIR}/src/string_view.c
)

#
# library : cpu_features
#

add_cpu_features_headers_and_sources(CPU_FEATURES_HDRS CPU_FEATURES_SRCS)
add_library(cpu_features ${CPU_FEATURES_HDRS} ${CPU_FEATURES_SRCS})
set_target_properties(cpu_features PROPERTIES PUBLIC_HEADER "${CPU_FEATURES_HDRS}")
setup_include_and_definitions(cpu_features)
target_link_libraries(cpu_features PUBLIC ${CMAKE_DL_LIBS})
set_property(TARGET cpu_features PROPERTY POSITION_INDEPENDENT_CODE ${BUILD_PIC})
target_include_directories(cpu_features
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_include/cpu_featuresDIR}/cpu_features>
)
if(PROCESSOR_IS_X86)
  if(APPLE)
    target_compile_definitions(cpu_features PRIVATE HAVE_SYSCTLBYNAME)
  endif()
endif()
add_library(CpuFeature::cpu_features ALIAS cpu_features)

set_target_properties(cpu_features PROPERTIES FOLDER "Dependencies")