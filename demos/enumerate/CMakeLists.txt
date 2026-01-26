add_executable(enumerate main.cpp)

set_target_properties(enumerate PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(enumerate PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(enumerate PRIVATE soloud::soloud soloud::demos_common)