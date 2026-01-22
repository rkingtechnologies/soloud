add_executable(annex main.cpp)

target_include_directories(annex PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(annex PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)