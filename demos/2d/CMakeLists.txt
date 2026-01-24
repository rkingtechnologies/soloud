add_executable(2d main.cpp)

target_include_directories(2d PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(2d PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)