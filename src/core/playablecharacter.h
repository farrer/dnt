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


#ifndef _dnt_playable_character_h
#define _dnt_playable_character_h

#include "dntconfig.h"
#include "../rules/character.h"

#include <kobold/timer.h>

namespace DNT
{

/*! A Character controlled by the human player */
class PlayableCharacter : public Character
{
   public:
      /*! Constructor */
      PlayableCharacter();
      /*! Destructor */
      ~PlayableCharacter();

      /*! Check user input for PlayableCharacter's movement 
       * \return true if tried to move. 
       * \param floorMouse mouse projected to Y=0 plane. */
      bool checkInputForMovement(const Ogre::Vector3& floorMouse);

      /*! Verify if can take levels on an specific class
       * \param cl -> class to verify if can take levels 
       * \return -> true if can take a level, false otherwise */
      bool canClass(Class* cl);

      /*! Get a new level on a class
       * \param cl -> pointer to the class to get a new level
       * \note -> will only take a level if have enought XP
       *          and can take a level at the specified class */
      void getNewClassLevel(Class* cl);

      /*! Overriden from Character class */
      void doAfterLoad();

   protected:

      /*! Parse key/value pairs related to the PlayableCharacter */
      bool doCharacterSpecializationParse(Ogre::String key, Ogre::String value);
      bool doCharacterSpecializationSave(std::ofstream& file);

      /*! Do the check for character movement with keyboard.
       * \return if tryed to move */
      bool checkKeyboardInputForMovement(bool& moved, bool& run);
      /*! Do the checks for movement by mouse 
       * \return if tryed to move */
      bool checkMouseInputForMovement(const Ogre::Vector3& floorMouse, 
            bool& moved, bool& run);
      /*! Do the character movement direct using mouse coordinates
       * \return if moved */
      bool doMovementByMouse(const Ogre::Vector3& floorMouse, bool& run);
      /*! Try to walk the character varX,varZ distance.
       * \return true if walked */
      bool tryWalk(float varX, float varZ);

   private:
      enum WalkStatus
      {
         WALK_KEYBOARD,
         WALK_MOUSE,
         WALK_ASTAR
      };
      WalkStatus walkState; /**< Current Walk state (keep track of how the user
                                 is currently - or potentially - moving). */
      Kobold::Timer walkPressTimer; /**< Timer to toggle between continous
                                         mouse walk and A* search */

      int upLevels; /**< Number of levels a character can advance now. */
      int xp;       /**< Current eXperience Points. */

      bool canMove;   /**< If still have movements at current fight's turn */
      bool canAttack; /**< If still have actions at current fight's turn */

      Goblin::Model3d* direction; /**< Direction when moving with mouse */
};

}


#endif


