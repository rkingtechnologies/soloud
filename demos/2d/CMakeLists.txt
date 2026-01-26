add_executable(2d main.cpp)

set_target_properties(2d PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(2d PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(2d PRIVATE soloud::soloud soloud::demos_common)