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

#ifndef _dnt_game_h_
#define _dnt_game_h_

#include "dntconfig.h"
#include <kobold/kstring.h>
#include <OGRE/OgreSceneManager.h>

namespace DNT
{

   /*! The Game static class keep informations about the current game,
    * to be shared among all our interactions and decisions. */
   class Game
   {
      public:
         /*! Init the game. Usually called when a game is loaded or created */
         static void init(Ogre::SceneManager* sceneManager);
         /*! Finish current game. Called when done playing it. */
         static void finish();

         /*! Do the update to our models and map */
         static void update();

         /*! Load a map and set it the current one. 
          * \note if travelling through maps, use travelToMap instead. 
          * \param filename map file to load.
          * \param fullPath if filename is defined with full path or as
          *        an inner file.
          * \param setPCsPositions set or not PCs at initial map position.
          * \param forceDynamicModels true to force load all models as dynamic.
          * \return loaded Map pointer (NULL if fail to load). */
         static Map* loadMap(Kobold::String filename, bool fullPath=false, 
               bool setPCsPositions=true, bool forceDynamicModels=false);

         /*! \return current loaded map */
         static Map* getCurrentMap();

         /*! Hard set current map.
          * \note used only for MapEditor */
         static void setCurrentMap(Map* map);

         /*! \return current NonPolayableCharacters list */
         static CharacterList* getNpcs();
 
         /*! \return current PlayableCharacters list */
         static CharacterList* getPcs();

         /*! \return pointer to a character from the list */
         static Character* getCharacter(Kobold::String filename, 
               Ogre::Vector3 originalPos);
         /*! \return pointer to a character from the list */
         static Character* getCharacter(Kobold::String filename); 

         /*! Create a new object (with its specialization type) based on its
          * file extension.
          * \param filename file to create (and load) object 
          * \param forceDynamic to force load as a dynamic model (MapEditor
          *   will required that, to it being able to change model's position)
          * \return pointer to the created object
          * \note object's memory deallocation is caller's responsability */
         static Object* createObject(Kobold::String filename, 
               bool forceDynamic = false);

         /*! \return used SceneManager */
         static Ogre::SceneManager* getSceneManager();

          /*! \return game used script manager */
         static ScriptManager* getScriptManager() { return scriptManager; };

         /*! \return if is at fight mode or normal */
         static const bool isAtFightMode() { return fightMode; };

      private:
         /*! Get a character from the lists of #pcs and #npcs */
         static Character* getCharacter(Kobold::String filename, 
               Ogre::Vector3 originalPos, bool usePos);

         static Ogre::SceneManager* sceneManager; /**< Used scene manager */

         static Map* currentMap; /**< Current loaded map pointer */

         /*! Current loaded NPCs */
         static CharacterList* npcs;
         /*! Current loaded PCs */
         static CharacterList* pcs;

         /*! Fight mode controller flag */
         static bool fightMode;
      
         /*! Manager of all DNT scripts */
         static ScriptManager* scriptManager;

   };

};

#endif

