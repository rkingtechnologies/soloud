cmake_minimum_required(VERSION 4.2.1) 

project(SOLOUD VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOLOUD_RUNTIME_OUTPUT_DIR ${SOLOUD_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLOUD_RUNTIME_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SOLOUD_RUNTIME_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SOLOUD_RUNTIME_OUTPUT_DIR})

list(APPEND CMAKE_MODULE_PATH
  "${SOLOUD_SOURCE_DIR}/cmake"
)

set(SOLOUD_LIB_INCLUDE_DIR "${SOLOUD_SOURCE_DIR}/include")

add_definitions(-DWITH_MINIAUDIO)

# core

set(SOLOUD_CORE_LIB_SOURCE_DIR "${SOLOUD_SOURCE_DIR}/src/core")

file(GLOB_RECURSE SOLOUD_CORE_LIB_SOURCES CONFIGURE_DEPENDS
  "${SOLOUD_CORE_LIB_SOURCE_DIR}/*.cpp"
)

add_library(soloud_core STATIC ${SOLOUD_CORE_LIB_SOURCES})
add_library(soloud::core ALIAS soloud_core)

target_include_directories(soloud_core PUBLIC
  $<BUILD_INTERFACE:${SOLOUD_LIB_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# audiosource

set(SOLOUD_AUDIOSOURCE_LIB_SOURCE_DIR "${SOLOUD_SOURCE_DIR}/src/audiosource")

file(GLOB_RECURSE SOLOUD_AUDIOSOURCE_LIB_SOURCES CONFIGURE_DEPENDS
  "${SOLOUD_AUDIOSOURCE_LIB_SOURCE_DIR}/*.cpp"
  "${SOLOUD_AUDIOSOURCE_LIB_SOURCE_DIR}/*.c"
)

add_library(soloud_audiosource STATIC ${SOLOUD_AUDIOSOURCE_LIB_SOURCES})
add_library(soloud::audiosource ALIAS soloud_audiosource)

target_include_directories(soloud_audiosource PUBLIC
  $<BUILD_INTERFACE:${SOLOUD_LIB_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# backend - miniaudo

set(SOLOUD_BACKEND_MINIAUDIO_LIB_SOURCE_DIR "${SOLOUD_SOURCE_DIR}/external/miniaudio")

file(GLOB_RECURSE SOLOUD_BACKEND_MINIAUDIO_LIB_SOURCES CONFIGURE_DEPENDS
  "${SOLOUD_BACKEND_MINIAUDIO_LIB_SOURCE_DIR}/*.cpp"
)

add_library(soloud_backend_miniaudio STATIC ${SOLOUD_BACKEND_MINIAUDIO_LIB_SOURCES})
add_library(soloud::backend_miniaudio ALIAS soloud_backend_miniaudio)

target_include_directories(soloud_backend_miniaudio PUBLIC
  $<BUILD_INTERFACE:${SOLOUD_LIB_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# filter

set(SOLOUD_FILTER_LIB_SOURCE_DIR "${SOLOUD_SOURCE_DIR}/src/filter")

file(GLOB_RECURSE SOLOUD_FILTER_LIB_SOURCES CONFIGURE_DEPENDS
  "${SOLOUD_FILTER_LIB_SOURCE_DIR}/*.cpp"
)

add_library(soloud_filter STATIC ${SOLOUD_FILTER_LIB_SOURCES})
add_library(soloud::filter ALIAS soloud_filter)

target_include_directories(soloud_filter PUBLIC
  $<BUILD_INTERFACE:${SOLOUD_LIB_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# ------------------------------------------------------------------------------
# tests / demos
# ------------------------------------------------------------------------------

option(SOLOUD_BUILD_TESTS "Build tests for soloud" ON)
set(SOLOUD_TEST_DIR "${SOLOUD_SOURCE_DIR}/tests")

if(SOLOUD_BUILD_TESTS AND EXISTS ${SOLOUD_TEST_DIR})
  enable_testing()
  include(cmake/tests_common.cmake)
  add_subdirectory(${SOLOUD_TEST_DIR})
endif()