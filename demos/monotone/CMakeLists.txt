add_executable(monotone main.cpp)

set_target_properties(monotone PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(monotone PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(monotone PRIVATE soloud::soloud soloud::demos_common)