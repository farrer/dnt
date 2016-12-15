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

#include "game.h"

#include "object.h"
#include "money.h"
#include "item.h"
#include "scenery.h"

#include "../map/map.h"
#include "../rules/character.h"

#include <kobold/log.h>

using namespace DNT;

/************************************************************************
 *                                  init                                *
 ************************************************************************/
void Game::init(Ogre::SceneManager* sceneManager)
{
   Game::sceneManager = sceneManager;
   npcs = new CharacterList();
   pcs = new CharacterList();
}

/************************************************************************
 *                               finish                                 *
 ************************************************************************/
void Game::finish()
{
   if(currentMap)
   {
      delete currentMap;
      currentMap = NULL;
   }
   if(npcs)
   {
      delete npcs;
      npcs = NULL;
   }
   if(pcs)
   {
      delete pcs;
      pcs = NULL;
   }
   sceneManager = NULL;
}

/************************************************************************
 *                           setCurrentMap                              *
 ************************************************************************/
void Game::setCurrentMap(Map* map)
{
   if(currentMap)
   {
      delete currentMap;
   }
   currentMap = map;
}

/************************************************************************
 *                               loadMap                                *
 ************************************************************************/
Map* Game::loadMap(Kobold::String filename, bool fullPath, bool setPCsPositions)
{
   /* Must freed any loaded map */
   if(currentMap)
   {
      delete currentMap;
      currentMap = NULL;
   }

   /* Create and try to load map */
   Map* map = new DNT::Map();
   if(!map->load(filename, fullPath))
   {
      /* Must delete the failed to load map */
      delete map;

      /* Log error and exit. */
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Error: couldn't load map '%s'", filename.c_str());
      return NULL;
   }

   /* Loaded, must set current map to it */
   currentMap = map;

   if(setPCsPositions)
   {
      /* Put PCs to initial position */
      Character* ch = static_cast<Character*>(pcs->getFirst());
      for(int i = 0; i < pcs->getTotal(); i++)
      {
         //FIXME: currently putting all at the same position, it should be
         //       fixed to support multiple PCs.
         ch->getModel()->setPosition(map->getInitialPosition());

         ch = static_cast<Character*>(ch->getNext());
      }
   }

   return map;
}

/************************************************************************
 *                             getCurrentMap                            *
 ************************************************************************/
Map* Game::getCurrentMap()
{
   return currentMap;
}

/************************************************************************
 *                                 getNpcs                              *
 ************************************************************************/
CharacterList* Game::getNpcs()
{
   return npcs;
}
 
/************************************************************************
 *                                 getPcs                               *
 ************************************************************************/
CharacterList* Game::getPcs()
{
   return pcs;
}

/************************************************************************
 *                            getSceneManager                           *
 ************************************************************************/
Ogre::SceneManager* Game::getSceneManager()
{
   return sceneManager;
}

/************************************************************************
 *                             createObject                             *
 ************************************************************************/
Object* Game::createObject(Kobold::String filename)
{
   Object* obj = NULL;

   /* verify if is money */
   if(filename == DNT_MONEY_OBJECT)
   {
      obj = static_cast<Object*>(new Money());
   }
   else
   {
      if(filename.find(".scn", 0) != Kobold::String::npos)
      {
         /* Static Scenery */
         obj = static_cast<Object*>(new Scenery());
      }
      else if(filename.find(".itn", 0) != Kobold::String::npos)
      {
         /* Item */
         obj = static_cast<Object*>(new Item());
      }
      //TODO: weapon
      //else if(filename.find(".wcc", 0) != Kobold::String::npos)
   }

   if(obj)
   {
      /* Load the object */
      obj->load(filename);
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Could not define type or could not load object '%s'",
            filename.c_str());
   }

   return obj;
}


/************************************************************************
 *                             static members                           *
 ************************************************************************/
Map* Game::currentMap = NULL;
Ogre::SceneManager* Game::sceneManager = NULL;
CharacterList* Game::npcs = NULL;
CharacterList* Game::pcs = NULL;

