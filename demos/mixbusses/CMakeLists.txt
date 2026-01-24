add_executable(mixbusses main.cpp)

target_include_directories(mixbusses PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(mixbusses PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)