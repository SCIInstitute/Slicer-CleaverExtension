cmake_minimum_required(VERSION 2.8)
find_package(Slicer COMPONENTS ConfigurePrerequisites) 

set(MODULE_TITLE "Cleaver Tet Mesher")
set(MODULE_NAME CleaverCLI)

find_package(GenerateCLP REQUIRED)
include(${GenerateCLP_USE_FILE})

set(MODULE_SRCS CleaverCLI.cxx)
GENERATECLP(MODULE_SRCS CleaverCLI.xml)
set(MODULE_TARGET_LIBRARIES cleaver teem)
set(MODULE_INCLUDE_DIRECTORIES)
add_subdirectory(Cleaver)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)


add_executable(${MODULE_NAME} ${MODULE_SRCS})
target_link_libraries(${MODULE_NAME} ${MODULE_TARGET_LIBRARIES})
#slicer3_set_plugins_output_path(CleaverCLI)
