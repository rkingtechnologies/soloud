include_guard(GLOBAL)

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
add_subdirectory(${SOLOUD_DEMO_DIR}/common/external/glfw)

# ------------------------------
# ImGui sources
# ------------------------------
set(IMGUI_DIR ${SOLOUD_DEMO_DIR}/common/external/imgui)

file(GLOB IMGUI_SOURCES
  CONFIGURE_DEPENDS
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ------------------------------
# Create soloud_demos_common
# ------------------------------
file(GLOB COMMON_SOURCES
  CONFIGURE_DEPENDS
  ${SOLOUD_DEMO_DIR}/common/*.cpp
)

add_library(soloud_demos_common STATIC
  ${COMMON_SOURCES}
  ${IMGUI_SOURCES}   # Include ImGui sources directly
)

# Include directories
target_include_directories(soloud_demos_common PUBLIC
  ${SOLOUD_DEMO_DIR}
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)

set(DEMO_ASSETS_DIR "${SOLOUD_DEMO_DIR}/assets")

if(EXISTS ${DEMO_ASSETS_DIR})
  target_compile_definitions(soloud_demos_common PRIVATE
    SOLOUD_DEMO_ASSETS_DIR="${DEMO_ASSETS_DIR}"
  )
else()
  message(WARNING "Demo assets directory not found: ${DEMO_ASSETS_DIR}")
endif()

# Link libraries
target_link_libraries(soloud_demos_common PUBLIC
  glfw
  OpenGL::GL
)

# Alias for easier usage
add_library(soloud::demos_common ALIAS soloud_demos_common)
