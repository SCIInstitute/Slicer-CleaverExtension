
cmake_minimum_required(VERSION 2.8)
#-----------------------------------------------------------------------------
set(MODULE_NAME CleaverCLI)
set(MODULE_TITLE "Cleaver Tet Mesher")

#-----------------------------------------------------------------------------
set(MODULE_INCLUDE_DIRECTORIES
  )

set(MODULE_SRCS
  )


add_subdirectory(Cleaver)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)


set(MODULE_TARGET_LIBRARIES
#  ${ITK_LIBRARIES}
   cleaver
   teem
  )

#-----------------------------------------------------------------------------
macro(BuildCLI
  NAME ${MODULE_NAME}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  ADDITIONAL_SRCS ${MODULE_SRCS}
  EXECUTABLE_ONLY
  )
endmacro()
#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

