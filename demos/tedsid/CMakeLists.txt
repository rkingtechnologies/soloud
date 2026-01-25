add_executable(tedsid main.cpp)

target_include_directories(tedsid PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(tedsid PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)