# This is the CMake definition file for DNT usage.
#
# Usually you should just call:
#
#    cmake .
#
# To create the needed makefiles for DNT compilation.
# 
# You could also set some debug options with:
#    -DDEBUG_ASTAR=ON 
#    -DDNT_DEBUG_PENDING_ACTION=ON
#    -DDNT_DEBUG_SCRIPTS=ON
#
# If you desire to create the OSX dnt.app bundle application, you should define:
#    -DCREATE_BUNDLE=ON
#
# You could also create a system specific package with CPack by defining:
#    -DCREATE_PACKAGE=ON 

cmake_minimum_required(VERSION 2.8)

set(PACKAGE dnt)

# Set Version number
set(VERSION_MAJOR 0)
set(VERSION_MINOR 13)
set(VERSION_PATCH 0)
if(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
else(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
endif(${VERSION_PATCH})

# Set directories
set(DATADIR ${CMAKE_INSTALL_PREFIX}/share)
set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(LOCALE_DIR share/locale)
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

project(dnt)

# Some Options
option(DEBUG_ASTAR "Enable A* Visual Debug" OFF)
option(DNT_DEBUG_PENDING_ACTION "Enable Pending Action Debug Log" OFF)
option(DNT_DEBUG_SCRIPTS "Enable DNT Script Language Debug Log" OFF)
option(CREATE_BUNDLE "Create OSX Bundle Application" OFF)
option(CREATE_PACKAGE "Create package using CPack" OFF)

# Some compiler options
if(UNIX)
   add_definitions(-Wall -g)
endif(UNIX)


# First, try to find required libraries

FIND_PACKAGE(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIR})

FIND_PACKAGE(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

FIND_PACKAGE(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

FIND_PACKAGE(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

FIND_PACKAGE(Ogg REQUIRED)
include_directories(${OGG_INCLUDE_DIR})

FIND_PACKAGE(Vorbis REQUIRED)
include_directories(${VORBIS_INCLUDE_DIR})

FIND_PACKAGE(Vorbisfile REQUIRED)
include_directories(${VORBISFILE_INCLUDE_DIR})

FIND_PACKAGE(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

FIND_PACKAGE(Kobold REQUIRED)
include_directories(${KOBOLD_INCLUDE_DIR})

FIND_PACKAGE(Kosound REQUIRED)
include_directories(${KOSOUND_INCLUDE_DIR})

FIND_PACKAGE(Goblin REQUIRED)
include_directories(${GOBLIN_INCLUDE_DIR})

FIND_PACKAGE(Farso REQUIRED)
include_directories(${FARSO_INCLUDE_DIR})

FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND)
   set(ENABLE_NLS 1)   
ELSE(GETTEXT_FOUND)
   set(ENABLE_NLS 0) 
ENDIF(GETTEXT_FOUND)

FIND_PACKAGE(Libintl)
include_directories(${LIBINTL_INCLUDE_DIR})

if(NOT ${LIBINTL_LIB_FOUND})
   set(LIBINTL_LIBRARIES "")
endif(NOT ${LIBINTL_LIB_FOUND})

# include the ./src as default too
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR})

include_directories(${CMAKE_BINARY_DIR}/src)

# Include the sources
include(sources.cmake)
configure_file("./src/dntconfig.h.in" "./src/dntconfig.h")

# Make Libraries
add_library(dntlib ${DNT_LIB_SOURCES} ${DNT_LIB_HEADERS} )

# Make Binaries
add_executable(dnt WIN32 ${DNT_SOURCES} ${DNT_HEADERS} )
target_link_libraries(dnt dntlib ${FARSO_LIBRARY} 
                          ${GOBLIN_LIBRARY} ${KOSOUND_LIBRARY} 
                          ${KOBOLD_LIBRARY}
                          ${OGRE_LIBRARIES} 
                          ${OGRE_RTShaderSystem_LIBRARIES}
                          ${OGRE_Overlay_LIBRARIES}
                          ${OGRE_Terrain_LIBRARIES}
                          ${FREETYPE_LIBRARIES}
                          ${SDL2_IMAGE_LIBRARY} ${SDL2_LIBRARY} 
                          ${OPENAL_LIBRARY} 
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m
                          ${OPENGL_LIBRARY}
                          ${LIBINTL_LIBRARIES} pthread)
                          
# Create the .gmo files
#IF(NOT WIN32 AND NOT MINGW)
  #include(GettextTranslate)
  #add_subdirectory(./po)
#ENDIF(NOT WIN32 AND NOT MINGW)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# Install files
install(TARGETS dnt DESTINATION bin)
install(DIRECTORY ./data DESTINATION share/${PACKAGE})

