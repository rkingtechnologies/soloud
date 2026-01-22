add_executable(enumerate main.cpp)

target_include_directories(enumerate PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(enumerate PRIVATE soloud::core)