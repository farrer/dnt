# Locate Kosound library
# This module defines
# KOSOUND_LIBRARY, the name of the library to link against
# KOSOUND_FOUND, if false, do not try to link to KOSOUND
# 
# KOSOUND_INCLUDE_DIR, where to find kosoundconfig.h
#
# $KOSOUNDDIR - enviroment variable
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

FIND_PATH(KOSOUND_INCLUDE_DIR NAMES kosound/kosoundconfig.h
  HINTS
  $ENV{KOSOUNDDIR}
  PATH_SUFFIXES include include/kosound
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/kosound
  /usr/include/kosound
  /usr/local/include
  /usr/include
  /sw/include/kosound # Fink
  /sw/include
  /opt/local/include/kosound # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/kosound
  /opt/include
)

FIND_LIBRARY(KOSOUND_LIBRARY 
  NAMES kosound
  HINTS
  $ENV{KOSOUNDDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(KOSOUND
   REQUIRED_VARS KOSOUND_LIBRARY KOSOUND_INCLUDE_DIR)
