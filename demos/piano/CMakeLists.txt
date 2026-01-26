add_executable(piano main.cpp rt_midi.cpp soloud_basicwave.cpp soloud_padsynth.cpp)

set_target_properties(piano PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(piano PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(piano PRIVATE soloud::soloud soloud::demos_common)