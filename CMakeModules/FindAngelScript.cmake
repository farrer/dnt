# Locate AngelScript library
# This module defines
# ANGELSCRIPT_LIBRARY, the name of the library to link against
# ANGELSCRIPT_FOUND, if false, do not try to link to ANGELSCRIPT
# 
# ANGELSCRIPT_INCLUDE_DIR, where to find angelscriptconfig.h
#
# $ANGELSCRIPTDIR - enviroment variable
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

FIND_PATH(ANGELSCRIPT_INCLUDE_DIR NAMES angelscript.h
  HINTS
  $ENV{ANGELSCRIPTDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/angelscript
  /usr/include/angelscript
  /usr/local/include
  /usr/include
  /sw/include/angelscript # Fink
  /sw/include
  /opt/local/include/angelscript # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/angelscript
  /opt/include
)

FIND_LIBRARY(ANGELSCRIPT_LIBRARY 
  NAMES angelscript
  HINTS
  $ENV{ANGELSCRIPTDIR}
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

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ANGELSCRIPT
   REQUIRED_VARS ANGELSCRIPT_LIBRARY ANGELSCRIPT_INCLUDE_DIR)
