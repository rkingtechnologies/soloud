add_executable(filterfolio main.cpp)

set_target_properties(filterfolio PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(filterfolio PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(filterfolio PRIVATE soloud::soloud soloud::demos_common)