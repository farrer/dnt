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

#include "nonplayablecharacter.h"
using namespace DNT;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
NonPlayableCharacter::NonPlayableCharacter(const Kobold::String& filename)
                     :Character(false),
                      psychoState(PS_NEUTRAL)
{
   load(filename);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
NonPlayableCharacter::~NonPlayableCharacter()
{
}

/***********************************************************************
 *                            setPsychoState                           *
 ***********************************************************************/
void NonPlayableCharacter::setPsychoState(PsychoState psycho)
{
   this->psychoState = psycho;
}

/***********************************************************************
 *                    doCharacterSpecializationParse                   *
 ***********************************************************************/
bool NonPlayableCharacter::doCharacterSpecializationParse(
      const Kobold::String& key, const Kobold::String& value)
{
   //TODO
   return false;
}

/***********************************************************************
 *                    doCharacterSpecializationSave                    *
 ***********************************************************************/
bool NonPlayableCharacter::doCharacterSpecializationSave(std::ofstream& file)
{
   //TODO
   return false;
}

/***********************************************************************
 *                              interact                               *
 ***********************************************************************/
Thing::InteractResult NonPlayableCharacter::interact(Character* actor)
{
   if((psychoState != PS_HOSTILE) && (hasConversationFile()))
   {
      return Thing::INTERACTION_OPEN_CONVERSATION;
   }

   return Thing::INTERACTION_DONE;
}

