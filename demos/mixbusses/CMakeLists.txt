add_executable(mixbusses main.cpp)

set_target_properties(mixbusses PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(mixbusses PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(mixbusses PRIVATE soloud::soloud soloud::demos_common)