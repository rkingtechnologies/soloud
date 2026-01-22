add_executable(filterfolio main.cpp)

target_include_directories(filterfolio PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(filterfolio PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)