# Locate Kobold library
# This module defines
# KOBOLD_LIBRARY, the name of the library to link against
# KOBOLD_FOUND, if false, do not try to link to KOBOLD
# 
# KOBOLD_INCLUDE_DIR, where to find koboldconfig.h
#
# $KOBOLDDIR - enviroment variable
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

FIND_PATH(KOBOLD_INCLUDE_DIR NAMES koboldconfig.h kobold/koboldcofig.h
  HINTS
  $ENV{KOBOLDDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/kobold
  /usr/include/kobold
  /usr/local/include
  /usr/include
  /sw/include/kobold # Fink
  /sw/include
  /opt/local/include/kobold # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/kobold
  /opt/include
)

FIND_LIBRARY(KOBOLD_LIBRARY 
  NAMES kobold
  HINTS
  $ENV{KOBOLDDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(KOBOLD
   REQUIRED_VARS KOBOLD_LIBRARY KOBOLD_INCLUDE_DIR)
