add_executable(welcome main.cpp)

set_target_properties(welcome PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(welcome PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(welcome PRIVATE soloud::soloud soloud::demos_common)