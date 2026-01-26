add_executable(simplest main.cpp)

set_target_properties(simplest PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(simplest PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(simplest PRIVATE soloud::soloud soloud::demos_common)