add_executable(speechfilter main.cpp)

target_include_directories(speechfilter PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(speechfilter PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)