# Locate Goblin library
# This module defines
# GOBLIN_LIBRARY, the name of the library to link against
# GOBLIN_FOUND, if false, do not try to link to GOBLIN
# 
# GOBLIN_INCLUDE_DIR, where to find goblinconfig.h
#
# $GOBLINDIR - enviroment variable
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

FIND_PATH(GOBLIN_INCLUDE_DIR NAMES goblinconfig.h goblin/goblincofig.h
  HINTS
  $ENV{GOBLINDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/goblin
  /usr/include/goblin
  /usr/local/include
  /usr/include
  /sw/include/goblin # Fink
  /sw/include
  /opt/local/include/goblin # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/goblin
  /opt/include
)

FIND_LIBRARY(GOBLIN_LIBRARY 
  NAMES goblin
  HINTS
  $ENV{GOBLINDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GOBLIN
   REQUIRED_VARS GOBLIN_LIBRARY GOBLIN_INCLUDE_DIR)
