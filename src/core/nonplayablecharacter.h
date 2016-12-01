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

#ifndef _dnt_non_playable_character_h
#define _dnt_non_playable_character_h

#include "dntconfig.h"
#include "../rules/character.h"

namespace DNT
{

/*! A Character controlled by the computer master (ie: AI). */
class NonPlayableCharacter : public Character
{
   public:

      /*! State of the NPC to the playable character(s) */
      enum PsychoState
      {
         PS_HOSTILE = 0,
         PS_NEUTRAL = 1,
         PS_FRIENDLY = 2
      };


   protected:

      /*! Parse key/value pairs related to the NonPlayableCharacter */
      bool doCharacterSpecializationParse(Ogre::String key, Ogre::String value);

      PsychoState psychoState; /**< Psycho state of the thing relative to
                                    current PCs */

};

}


#endif



