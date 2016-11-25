# Locate Ogg library
# This module defines
# OGG_LIBRARY, the name of the library to link against
# OGG_FOUND, if false, do not try to link to OGG
# 
# OGG_INCLUDE_DIR, where to find ogg/ogg.h
#
# $OGGDIR - enviroment variable
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

FIND_PATH(OGG_INCLUDE_DIR ogg/ogg.h
  HINTS
  $ENV{OGGDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/ogg
  /usr/include/ogg
  /usr/local/include
  /usr/include
  /sw/include/ogg # Fink
  /sw/include
  /opt/local/include/ogg # DarwinPorts
  /opt/local/include
  /opt/csw/include/ogg # Blastwave
  /opt/csw/include 
  /opt/include/ogg
  /opt/include
)

FIND_LIBRARY(OGG_LIBRARY 
  NAMES ogg
  HINTS
  $ENV{OGGDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGG
                                  REQUIRED_VARS OGG_LIBRARY OGG_INCLUDE_DIR)
