include(cmake/demos_common.cmake)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/2d")
  add_subdirectory(2d)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/annex")
  add_subdirectory(annex)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/enumerate")
  add_subdirectory(enumerate)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/env")
  add_subdirectory(env)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/filterfolio")
  add_subdirectory(filterfolio)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mixbusses")
  add_subdirectory(mixbusses)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/monotone")
  add_subdirectory(monotone)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/multimusic")
  add_subdirectory(multimusic)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/piano")
  add_subdirectory(piano)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/radiogaga")
  add_subdirectory(radiogaga)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/simplest")
  add_subdirectory(simplest)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/speechfilter")
  add_subdirectory(speechfilter)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tedsid")
  add_subdirectory(tedsid)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thebutton")
  add_subdirectory(thebutton)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/wavformats")
  add_subdirectory(wavformats)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/welcome")
  add_subdirectory(welcome)
endif()
