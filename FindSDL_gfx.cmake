# Locate SDL_gfx library
# This module defines
# SDLGFX_LIBRARY, the name of the library to link against
# SDLGFX_FOUND, if false, do not try to link to SDL
# SDLGFX_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by David Demelier. This was influenced by the FindSDL_mixer.cmake 

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
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(SDLGFX_INCLUDE_DIR SDL_gfxPrimitives.h
        HINTS
        $ENV{SDLGFXDIR}
        $ENV{SDLDIR}
        PATH_SUFFIXES include
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/include/SDL
        /usr/include/SDL
        /usr/local/include/SDL12
        /usr/local/include/SDL11 # FreeBSD ports
        /usr/include/SDL12
        /usr/include/SDL11
        /usr/local/include
        /usr/include
        /sw/include/SDL # Fink
        /sw/include
        /opt/local/include/SDL # DarwinPorts
        /opt/local/include
        /opt/csw/include/SDL # Blastwave
        /opt/csw/include
        /opt/include/SDL
        /opt/include
        )

FIND_PATH(SDLGFX_SOURCE_DIR SDL_framerate.c SDL_gfxBlitFunc.c SDL_gfxPrimitives.c SDL_imageFilter.c SDL_rotozoom.c
  HINTS
  $ENV{SDLGFXDIR}
  $ENV{SDLDIR}
  PATH_SUFFIXES source
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/source/SDL
  /usr/source/SDL
  /usr/local/source/SDL12
  /usr/local/source/SDL11 # FreeBSD ports
  /usr/source/SDL12
  /usr/source/SDL11
  /usr/local/source
  /usr/source
  /sw/source/SDL # Fink
  /sw/source
  /opt/local/source/SDL # DarwinPorts
  /opt/local/source
  /opt/csw/source/SDL # Blastwave
  /opt/csw/source
  /opt/source/SDL
  /opt/source
)

SET(SDLGFX_FOUND "NO")
IF(SDLGFX_SOURCE_DIR AND SDLGFX_INCLUDE_DIR)
  SET(SDLGFX_FOUND "YES")
ENDIF(SDLGFX_SOURCE_DIR AND SDLGFX_INCLUDE_DIR)

