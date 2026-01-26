add_executable(annex main.cpp)

set_target_properties(annex PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(annex PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(annex PRIVATE soloud::soloud soloud::demos_common)