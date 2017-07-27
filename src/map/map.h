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

#include "mapmesh.h"
#include "wall.h"

#include "../rules/thing.h"

#include <kobold/list.h>
#include <kobold/kstring.h>

#include <map>

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

      /*! \return map's x dimension (in square units) */
      const int getSizeX() const { return xSize;};
      /*! \return map's z dimension (in square units) */
      const int getSizeZ() const { return zSize;};

      /*! \return map's x dimension (in world units) */
      const int getSizeXWorld() const { return xSize * MAP_SQUARE_SIZE; };
      /*! \return map's z dimension (in world units) */
      const int getSizeZWorld() const { return zSize * MAP_SQUARE_SIZE; };


      /*! Open a map to use. 
       * \param mapFileName name of the map file to load.
       * \param fullPath if mapFileName is defined as full path or inner file.
       * \param forceDynamicModels force loading all models as dynamic ones (
       *        MapEditor will need that to change their positions).
       * \return if load was successfull or not. */
      bool load(Kobold::String mapFileName, bool fullPath, 
            bool forceDynamicModels = false);

      /*! \return  map's name */
      Kobold::String getName();
      /*! Set map's name 
       * \param name new map name. */
      void setName(Kobold::String name);

      /*! \return map's filename
       * \note only valid after load. */
      Kobold::String getFilename();

      /*! Update everything related to the map for curent frame.
       * \param refPos reference position for active lights. */
      void update(Ogre::Vector3 refPos);

      /*! \return initial PlayableCharacters position on map
       * \note this position is used just on plain map load, as on map travel,
       * PCs positions are defined by the map connection used. */
      Ogre::Vector3 getInitialPosition();

      /*! \return music's file to play while active */
      Kobold::String getMusicFilename();
      /*! Set music file to be played while map is active 
       * \param filename music filename */
      void setMusicFilename(Kobold::String filename);

      /*! Get a Thing currently at the Map that owns the SceneNode
       * \param sceneNode sceneNode to get the Thing
       * \return Thing's pointer or NULL if none */
      Thing* getThing(Ogre::SceneNode* sceneNode);

      /*! Remove a Thing from the map.
       * \note this function will delete the Thing's pointer. 
       * \param thing pointer to the Thing to be removed. */
      void removeThing(Thing* thing);

      /*! Insert a new thing on the map
       * \param filename filename of the thing to insert
       * \param forceDynamic true to force static models to be dynamic
       *        (usually on editors).
       * \param pos position to put the thing on
       * \param ori orientation to set the thing.
       * \return pointer to the inserted thing */
      Thing* insertThing(Kobold::String filename, bool forceDynamic,
            const Ogre::Vector3& pos, const Ogre::Vector3& ori);

      /*! \return current map lights */
      MapLights* getLights() { return lights; };
      
      MapScriptInstance* getScriptInstance() { return script; };

      Kobold::List* getStaticThings() { return staticThings; };
      Kobold::List* getDynamicThings() { return dynamicThings; };

   private:
      /*! Update all dirty wall meshes */
      void updateAllDirtyWalls();

      /*! Insert a new thing on the map */
      Thing* insertThing(Kobold::String filename, bool forceDynamic);

      MapMesh floor; /**< The indoor floor meshes */
      Kobold::List walls; /**< The indoor walls */

      Kobold::String name;     /**< Map's name */
      Kobold::String filename; /**< Map's filename */
      
      Kobold::String musicFilename; /**< Map's music filename */

      MapLights* lights; /**< All light definitions for map */

      MapScriptInstance* script; /**< Script used for the map */

      //FIXME: use a more suitable structure.
      Kobold::List* dynamicThings; /**< Dynamic things list */
      Kobold::List* staticThings;  /**< Static things list */

      int xSize;  /**< Map size (in square units) for X axys */
      int zSize;  /**< Map size (in square units) for Z axys */

      Ogre::Vector3 initialPos; /**< initial position on map for PCs */

      /*! A map of SceneNode to respective Thing's pointer.
       * this is usually used to get the "Thing" within a SceneNode received
       * as a result of a RayCast from cursor position */
      std::map<Ogre::SceneNode*, Thing*> nodesPerThingMap;
};

}

#endif


