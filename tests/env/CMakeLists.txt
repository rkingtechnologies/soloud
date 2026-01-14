add_executable(env main.cpp)

target_include_directories(env PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(env PRIVATE 
    soloud::core soloud::audiosource 
    soloud::backend_miniaudio soloud::tests_common soloud::filter)