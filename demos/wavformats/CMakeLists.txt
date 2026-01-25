add_executable(wavformats main.cpp)

target_include_directories(wavformats PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(wavformats PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)