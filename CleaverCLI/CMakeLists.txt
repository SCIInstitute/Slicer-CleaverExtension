cmake_minimum_required(VERSION 2.8)

set(MODULE_TITLE "Cleaver Tet Mesher")
set(MODULE_NAME CleaverCLI)

find_package(GenerateCLP REQUIRED)
include(${GenerateCLP_USE_FILE})
find_package(Slicer3 REQUIRED)
include(${Slicer3_USE_FILE})

set(MODULE_SRCS CleaverCLI.cxx)
set(MODULE_TARGET_LIBRARIES cleaver teem)
set(MODULE_INCLUDE_DIRECTORIES)
add_subdirectory(Cleaver)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

GENERATECLP(MODULE_SRCS CleaverCLI.xml)

add_executable(${MODULE_NAME} ${MyFilter_SOURCE})
target_link_libraries(MyFilter ${MODULE_TARGET_LIBRARIES})
slicer3_set_plugins_output_path(CleaverCLI)
