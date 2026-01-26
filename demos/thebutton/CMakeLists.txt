add_executable(thebutton main.cpp)

set_target_properties(thebutton PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_include_directories(thebutton PRIVATE ${SOLOUD_INCLUDE_DIR})

target_link_libraries(thebutton PRIVATE soloud::soloud soloud::demos_common)