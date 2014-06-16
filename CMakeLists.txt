#-----------------------------------------------------------------------------
#
# Cleaver Extension - A MultiMaterial Tetrahedral Mesher Extension
# -- CMakeList
#
# Author:        Jonathan Bronson (bronson@sci.utah.edu)
#                Brig Bagley      (brig@sci.utah.edu)
#
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.9)

project(CleaverExtension)

#-----------------------------------------------------------------------------
set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/4.3/Extensions/CleaverExtension")
set(EXTENSION_CATEGORY "Mesh Generation")
set(EXTENSION_CONTRIBUTORS "Jonathan Bronson (SCI Institute), Brig Bagley (SCI Institute)")
set(EXTENSION_DESCRIPTION "This extension bundles the Cleaver meshing tool as a CLI module.")
set(EXTENSION_ICONURL "https://github.com/SCIInstitute/Slicer-CleaverExtension/blob/master/CleaverExtension.png")
set(EXTENSION_SCREENSHOTURLS "https://github.com/SCIInstitute/Slicer-CleaverExtension/blob/master/screenshot1.jpeg https://github.com/SCIInstitute/Slicer-CleaverExtension/blob/master/screenshot2.jpeg https://github.com/SCIInstitute/Slicer-CleaverExtension/blob/master/screenshot3.jpeg https://github.com/SCIInstitute/Slicer-CleaverExtension/blob/master/screenshot4.jpeg")

#-----------------------------------------------------------------------------
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})
#-----------------------------------------------------------------------------
add_subdirectory(CleaverCLI)
