# Locate Vorbisfile library
# This module defines
# VORBISFILE_LIBRARY, the name of the library to link against
# VORBISFILE_FOUND, if false, do not try to link to VORBISFILE
# 
# VORBISFILE_INCLUDE_DIR, where to find vorbisfile/vorbisfile.h
#
# $VORBISFILEDIR - enviroment variable
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

FIND_PATH(VORBISFILE_INCLUDE_DIR vorbis/vorbisfile.h
  HINTS
  $ENV{VORBISFILEDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/vorbisfile
  /usr/include/vorbisfile
  /usr/local/include
  /usr/include
  /sw/include/vorbisfile # Fink
  /sw/include
  /opt/local/include/vorbisfile # DarwinPorts
  /opt/local/include
  /opt/csw/include/vorbisfile # Blastwave
  /opt/csw/include 
  /opt/include/vorbisfile
  /opt/include
)

FIND_LIBRARY(VORBISFILE_LIBRARY 
  NAMES vorbisfile
  HINTS
  $ENV{VORBISFILEDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(VORBISFILE
                                  REQUIRED_VARS 
                                  VORBISFILE_LIBRARY VORBISFILE_INCLUDE_DIR)
