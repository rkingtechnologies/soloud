add_executable(tedsid main.cpp)

set_target_properties(tedsid PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(tedsid PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(tedsid PRIVATE soloud::soloud soloud::demos_common)