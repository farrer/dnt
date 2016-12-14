/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_h
#define _dnt_map_h

#include "dntconfig.h"

#include "indoortexturemesh.h"

#include <kobold/list.h>
#include <kobold/kstring.h>

namespace DNT
{

class Map
{
   public:
      /*! Constructor */
      Map();

      /*! Destructor */
      ~Map();

      /*! Create a new (and empty) map to use.
       * \param sizeX x dimension
       * \param sizeZ z dimension */
      void create(int sizeX, int sizeZ);

      /*! Open a map to use. 
       * \param mapFileName name of the map file to load.
       * \param fullPath if mapFileName is defined as full path or inner file.
       * \return if load was successfull or not. */
      bool load(Kobold::String mapFileName, bool fullPath);

      /*! \return map's filename
       * \note only valid after load. */
      Kobold::String getFilename();

      /*! Update everithing related to the map for curent frame. */
      void update(Ogre::Vector3 floorMouse);

      /*! \return initial PlayableCharacters position on map
       * \note this position is used just on plain map load, as on map travel,
       * PCs positions are defined by the map connection used. */
      Ogre::Vector3 getInitialPosition();

      /*! \return music's file to play while active */
      Kobold::String getMusicFilename();
      /*! Set music file to be played while map is active 
       * \param filename music filename */
      void setMusicFilename(Kobold::String filename);

   private:

      IndoorTextureMeshes floor; /**< The indoor floor meshes */
      IndoorTextureMeshes walls; /**< The indoor wall meshes */

      Kobold::String filename; /**< Map's filename */
      
      Kobold::String musicFilename; /**< Map's music filename */

      MapLights* lights; /**< All light definitions for map */

      //FIXME: use a more suitable structure.
      Kobold::List* things; /**< Things list */

      int xSize;  /**< Map size (in square units) for X axys */
      int zSize;  /**< Map size (in square units) for Z axys */

      Ogre::Vector3 initialPos; /**< initial position on map for PCs */
};

}

#endif


