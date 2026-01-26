cmake_minimum_required(VERSION 4.2.1) 

project(SOLOUD VERSION 1.0.0 LANGUAGES CXX C)

set(SOLOUD_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/core")
set(SOLOUD_AUDIOSOURCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/audiosource")
set(SOLOUD_FILTERS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/filters")
set(SOLOUD_MINIAUDIO_BACKEND_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/miniaudio")

file(GLOB_RECURSE SOLOUD_SOURCES CONFIGURE_DEPENDS
  "${SOLOUD_CORE_SOURCE_DIR}/*.cpp"
  "${SOLOUD_AUDIOSOURCE_SOURCE_DIR}/*.cpp"
  "${SOLOUD_AUDIOSOURCE_SOURCE_DIR}/*.c"
  "${SOLOUD_FILTERS_SOURCE_DIR}/*.cpp"
  "${SOLOUD_MINIAUDIO_BACKEND_SOURCE_DIR}/*.cpp"
)

add_library(soloud STATIC ${SOLOUD_SOURCES})
add_library(soloud::soloud ALIAS soloud)

set_target_properties(soloud PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  EXPORT_NAME soloud
)

set(SOLOUD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(soloud PUBLIC
  $<BUILD_INTERFACE:${SOLOUD_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(SOLOUD_BUILD_DEMOS "Build demos for soloud" ON)
set(SOLOUD_DEMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/demos")

if(SOLOUD_BUILD_DEMOS AND EXISTS ${SOLOUD_DEMO_DIR})
  add_subdirectory(${SOLOUD_DEMO_DIR})
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
  TARGETS soloud
  EXPORT soloudTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers (flat layout: include/soloud.h → <prefix>/include/soloud.h)
install(
  DIRECTORY "${SOLOUD_INCLUDE_DIR}/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)

install(
  EXPORT soloudTargets
  FILE soloudTargets.cmake
  NAMESPACE soloud::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/soloud
)

# ── Package config so find_package(soloud) / find_package(SoLoud) works ─
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/soloudConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/soloudConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/soloud
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/soloudConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/soloudConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/soloudConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/soloud
)