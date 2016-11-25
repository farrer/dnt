# Locate Vorbis library
# This module defines
# VORBIS_LIBRARY, the name of the library to link against
# VORBIS_FOUND, if false, do not try to link to VORBIS
# 
# VORBIS_INCLUDE_DIR, where to find vorbis/vorbis.h
#
# $VORBISDIR - enviroment variable
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

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/vorbisenc.h
  HINTS
  $ENV{VORBISDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/vorbis
  /usr/include/vorbis
  /usr/local/include
  /usr/include
  /sw/include/vorbis # Fink
  /sw/include
  /opt/local/include/vorbis # DarwinPorts
  /opt/local/include
  /opt/csw/include/vorbis # Blastwave
  /opt/csw/include 
  /opt/include/vorbis
  /opt/include
)

FIND_LIBRARY(VORBIS_LIBRARY 
  NAMES vorbis
  HINTS
  $ENV{VORBISDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(VORBIS
                                  REQUIRED_VARS VORBIS_LIBRARY VORBIS_INCLUDE_DIR)
