set(MODULE_TITLE "Cleaver Tet Mesher")
set(MODULE_NAME CleaverCLI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(MODULE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/include)

set(MODULE_SRCS CleaverCLI.cxx)

set(MODULE_TARGET_LIBRARIES ${ITK_LIBRARIES} cleaver)

SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  ADDITIONAL_SRCS ${MODULE_SRCS}
  )

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

add_subdirectory(Cleaver)
