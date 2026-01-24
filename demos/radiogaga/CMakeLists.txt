add_executable(radiogaga main.cpp)

target_include_directories(radiogaga PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(radiogaga PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)