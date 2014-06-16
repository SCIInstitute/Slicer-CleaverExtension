#cmake_minimum_required(VERSION 2.8)
#find_package(Slicer COMPONENTS ConfigurePrerequisites)

set(MODULE_TITLE "Cleaver Tet Mesher")
set(MODULE_NAME CleaverCLI)

#find_package(GenerateCLP REQUIRED)
#include(${GenerateCLP_USE_FILE})

find_package(SlicerExecutionModel REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(MODULE_INCLUDE_DIRECTORIES 
  ${CMAKE_BINARY_DIR}/include
  ${Teem_DIR}/include)

set(MODULE_SRCS CleaverCLI.cxx)

#FIND_LIBRARY(TEEM_LIBRARY NAMES teem PATHS ${Teem_LIBRARY_DIRS})
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
#GENERATECLP(MODULE_SRCS CleaverCLI.xml)
#INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

#add_executable(${MODULE_NAME} ${MODULE_SRCS})
#target_link_libraries(${MODULE_NAME} ${MODULE_TARGET_LIBRARIES})
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

add_subdirectory(Cleaver)
