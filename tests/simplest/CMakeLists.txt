add_executable(simplest main.cpp)

target_include_directories(simplest PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(simplest PRIVATE soloud::core soloud::audiosource soloud::backend_miniaudio)