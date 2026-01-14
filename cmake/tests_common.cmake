include_guard(GLOBAL)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Configure GLFW
set(GLFW_BUILD_EXAMPLES           OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS              OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS               OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL                  OFF CACHE BOOL "" FORCE)
add_subdirectory(${SOLOUD_SOURCE_DIR}/external/glfw)

# Configure ImGui
set(IMGUI_DIR ${SOLOUD_SOURCE_DIR}/external/imgui)

file(GLOB IMGUI_SOURCES
  CONFIGURE_DEPENDS
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_library(soloud_tests_common STATIC 
  ${SOLOUD_SOURCE_DIR}/tests/common/window.cpp 
)

target_include_directories(soloud_tests_common PRIVATE ${SOLOUD_SOURCE_DIR}/tests/common)

target_link_libraries(soloud_tests_common PUBLIC imgui glfw OpenGL::GL)