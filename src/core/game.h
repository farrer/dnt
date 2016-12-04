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

         /*! Load a map and set it the current one. 
          * \note if travelling through maps, use #travelToMap instead. 
          * \param filename map file to load.
          * \param setPCsPositions set or not PCs at initial map position.
          * \return loaded Map pointer (NULL if fail to load). */
         static Map* loadMap(Kobold::String filename, 
               bool setPCsPositions=true);

         /*! \return current loaded map */
         static Map* getCurrentMap();

         /*! \return current NonPolayableCharacters list */
         static CharacterList* getNpcs();
 
         /*! \return current PlayableCharacters list */
         static CharacterList* getPcs();

      private:
         static Ogre::SceneManager* sceneManager; /**< Used scene manager */

         static Map* currentMap; /**< Current loaded map pointer */

         /*! Current loaded NPCs */
         static CharacterList* npcs;
         /*! Current loaded PCs */
         static CharacterList* pcs;


   };

};

#endif

