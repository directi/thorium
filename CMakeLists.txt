#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the thorium project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

include(${CMAKE_DIR}/common.cmake)

set (PROJNAME ${PLUGIN_NAME})

browserplugin_project(${PROJNAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    )
file (GLOB FILESYSTEM_MODULE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    modules/filesystem/*.cpp
    modules/filesystem/*.h
    )

file (GLOB SOCKET_MODULE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    modules/network/*.cpp
    modules/network/*.h
    )


# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    ${FILESYSTEM_MODULE}
    ${SOCKET_MODULE}
    )

# This will include Win/projectDef.cmake, Linux/projectDef.cmake, etc
include_platform()
