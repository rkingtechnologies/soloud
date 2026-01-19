add_executable(piano main.cpp rt_midi.cpp soloud_basicwave.cpp soloud_padsynth.cpp)

target_include_directories(piano PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(piano PRIVATE 
    soloud::core soloud::audiosource 
    soloud::backend_miniaudio soloud::tests_common soloud::filter)