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
 *                               loadMap                                *
 ************************************************************************/
Map* Game::loadMap(Kobold::String filename, bool setPCsPositions)
{
   /* Create and try to load map */
   Map* map = new DNT::Map();
   if(!map->load(filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Error: couldn't load map '%s'", filename.c_str());
      return NULL;
   }

   /* Must free any loaded map */
   if(currentMap)
   {
      delete currentMap;
   }

   /* Loaded, must set current map to it */
   currentMap = map;

   if(setPCsPositions)
   {
      /* Put PCs to initial position */
      Character* ch = (Character*) pcs->getFirst();
      for(int i = 0; i < pcs->getTotal(); i++)
      {
         //FIXME: currently putting all at the same position, it should be
         //       fixed to support multiple PCs.
         ch->getModel()->setPosition(map->getInitialPosition());

         ch = (Character*) ch->getNext();
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
 *                             static members                           *
 ************************************************************************/
Map* Game::currentMap = NULL;
Ogre::SceneManager* Game::sceneManager = NULL;
CharacterList* Game::npcs = NULL;
CharacterList* Game::pcs = NULL;

