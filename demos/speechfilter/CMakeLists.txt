add_executable(speechfilter main.cpp)

set_target_properties(speechfilter PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(speechfilter PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(speechfilter PRIVATE soloud::soloud soloud::demos_common)