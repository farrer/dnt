# Locate Farso library
# This module defines
# FARSO_LIBRARY, the name of the library to link against
# FARSO_OGRE3D_LIBRARY, ogre3d farso renderer library
# FARSO_OPENGL_LIBRARY, opengl farso renderer library
# FARSO_LIBRARIES, all farso libraries
# FARSO_FOUND, if false, do not try to link to FARSO
# 
# FARSO_INCLUDE_DIR, where to find farsoconfig.h
#
# $FARSODIR - enviroment variable
#
# Created by Farrer. This was influenced by the FindSDL_image.cmake module.

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

FIND_PATH(FARSO_INCLUDE_DIR NAMES farsoconfig.h farso/farsoconfig.h
  HINTS
  $ENV{FARSODIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/farso
  /usr/include/farso
  /usr/local/include
  /usr/include
  /sw/include/farso # Fink
  /sw/include
  /opt/local/include/farso # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/farso
  /opt/include
)

FIND_LIBRARY(FARSO_LIBRARY 
  NAMES farso
  HINTS
  $ENV{FARSODIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(FARSO_OPENGL_LIBRARY 
  NAMES farso-opengl
  HINTS
  $ENV{FARSODIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(FARSO_OGRE3D_LIBRARY 
  NAMES farso-ogre3d
  HINTS
  $ENV{FARSODIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(FARSO
   REQUIRED_VARS FARSO_LIBRARY FARSO_INCLUDE_DIR)

if(FARSO_FOUND)
   set(FARSO_LIBRARIES ${FARSO_LIBRARY})
   if(FARSO_OPENGL_LIBRARY_FOUND)
      set(FARSO_LIBRARIES ${FARSO_LIBRARIES} ${FARSO_OPENGL_LIBRARY})
   endif()
   if(FARSO_OGRE3D_LIBRARY_FOUND)
      set(FARSO_LIBRARIES ${FARSO_LIBRARIES} ${FARSO_OGRE3D_LIBRARY})
   endif()
endif()


