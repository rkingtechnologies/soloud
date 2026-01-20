include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.15)
project(soloud_tests_common_project)

# ------------------------------
# Find OpenGL
# ------------------------------
find_package(OpenGL REQUIRED)

# ------------------------------
# Configure GLFW
# ------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(${SOLOUD_SOURCE_DIR}/tests/external/glfw)

# ------------------------------
# ImGui sources
# ------------------------------
set(IMGUI_DIR ${SOLOUD_SOURCE_DIR}/tests/external/imgui)

file(GLOB IMGUI_SOURCES
  CONFIGURE_DEPENDS
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ------------------------------
# Create soloud_tests_common
# ------------------------------
file(GLOB COMMON_SOURCES
  CONFIGURE_DEPENDS
  ${SOLOUD_SOURCE_DIR}/tests/common/*.cpp
)

add_library(soloud_tests_common STATIC
  ${COMMON_SOURCES}
  ${IMGUI_SOURCES}   # Include ImGui sources directly
)

# Include directories
target_include_directories(soloud_tests_common PUBLIC
  ${SOLOUD_SOURCE_DIR}/tests
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)

# Link libraries
target_link_libraries(soloud_tests_common PUBLIC
  glfw
  OpenGL::GL
)

# Alias for easier usage
add_library(soloud::tests_common ALIAS soloud_tests_common)
