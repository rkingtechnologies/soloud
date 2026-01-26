add_executable(radiogaga main.cpp)

set_target_properties(radiogaga PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(radiogaga PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(radiogaga PRIVATE soloud::soloud soloud::demos_common)