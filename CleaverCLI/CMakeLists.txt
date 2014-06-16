set(MODULE_TITLE "Cleaver Tet Mesher")
set(MODULE_NAME CleaverCLI)

find_package(SlicerExecutionModel REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(MODULE_INCLUDE_DIRECTORIES 
  ${CMAKE_BINARY_DIR}/include
  ${Teem_DIR}/include)

set(MODULE_SRCS CleaverCLI.cxx)

find_package(Teem REQUIRED)
if(NOT Teem_FOUND)
    message(FATAL_ERROR "Need Teem to build")
endif(NOT Teem_FOUND)
include(${Teem_USE_FILE})

set(MODULE_TARGET_LIBRARIES 
  ${ITK_LIBRARIES}
  teem 
  cleaver
  )

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
