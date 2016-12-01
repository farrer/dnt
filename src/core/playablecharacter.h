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

      /*! Verify if can take levels on an specific class
       * \param cl -> class to verify if can take levels 
       * \return -> true if can take a level, false otherwise */
      bool canClass(Class* cl);

      /*! Get a new level on a class
       * \param cl -> pointer to the class to get a new level
       * \note -> will only take a level if have enought XP
       *          and can take a level at the specified class */
      void getNewClassLevel(Class* cl);

   protected:

      /*! Parse key/value pairs related to the PlayableCharacter */
      bool doCharacterSpecializationParse(Ogre::String key, Ogre::String value);

   private:
      int upLevels; /**< Number of levels a character can advance now. */
      int xp;       /**< Current eXperience Points. */

      bool canMove;   /**< If still have movements at current fight's turn */
      bool canAttack; /**< If still have actions at current fight's turn */
};

}


#endif


