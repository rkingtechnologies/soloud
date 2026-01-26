add_executable(welcome main.cpp)

target_include_directories(welcome PRIVATE ${SOLOUD_LIB_INCLUDE_DIR})

target_link_libraries(welcome PRIVATE
  soloud::core soloud::audiosource soloud::demos_common
)