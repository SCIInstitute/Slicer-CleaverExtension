#-----------------------------------------------------------------------------
#
# Cleaver Extension - A MultiMaterial Tetrahedral Mesher Extension
# -- CMakeList
#
# Author:        Jonathan Bronson (bronson@sci.utah.edu)
#
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.9)

find_package(Slicer COMPONENTS ConfigurePrerequisites) 

project(CleaverExtension)

#-----------------------------------------------------------------------------
set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/4.3/Extensions/CleaverExtension")
set(EXTENSION_CATEGORY "Mesh Generation")
set(EXTENSION_CONTRIBUTORS "Jonathan Bronson (SCI Institute)")
set(EXTENSION_DESCRIPTION "This extension bundles the Cleaver meshing tool as a CLI module.")
set(EXTENSION_ICONURL "http://viewvc.slicer.org/viewvc.cgi/Slicer4/trunk/Extensions/Testing/CleaverExtension/CleaverExtension.png?revision=21745&view=co")
set(EXTENSION_SCREENSHOTURLS "http://wiki.slicer.org/slicerWiki/images/a/ab/Slicer-r19441-CleaverExtension-screenshot.png http://wiki.slicer.org/slicerWiki/images/1/1e/Slicer-r19441-CleaverExtension-screenshot-2.png")

#-----------------------------------------------------------------------------
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
add_subdirectory(CleaverCLI)

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
