add_executable(env main.cpp)

set_target_properties(env PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(env PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(env PRIVATE soloud::soloud soloud::demos_common)