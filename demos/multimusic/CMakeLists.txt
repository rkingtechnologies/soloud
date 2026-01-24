add_executable(multimusic main.cpp)

target_include_directories(multimusic PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(multimusic PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)