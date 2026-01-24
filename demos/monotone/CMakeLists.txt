add_executable(monotone main.cpp)

target_include_directories(monotone PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(monotone PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)