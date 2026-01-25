add_executable(thebutton main.cpp)

target_include_directories(thebutton PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(thebutton PRIVATE 
    soloud::core soloud::audiosource soloud::filter soloud::demos_common)