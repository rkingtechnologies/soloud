add_executable(multimusic main.cpp)

set_target_properties(multimusic PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(multimusic PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(multimusic PRIVATE soloud::soloud soloud::demos_common)