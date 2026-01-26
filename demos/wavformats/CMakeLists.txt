add_executable(wavformats main.cpp)

set_target_properties(wavformats PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(wavformats PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(wavformats PRIVATE soloud::soloud soloud::demos_common)