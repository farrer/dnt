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

#include "initiative.h"
#include "../rules/dices.h"

#include "../gui/briefing.h"

#include <kobold/kstring.h>

using namespace DNT;

/************************************************************************
 *                          InitiativeInfo                              *
 ************************************************************************/
InitiativeInfo::InitiativeInfo(Character* dude, int value)
{
   this->dude = dude;
   this->value = value;
}

/************************************************************************
 *                         ~InitiativeInfo                              *
 ************************************************************************/
InitiativeInfo::~InitiativeInfo()
{
}

/************************************************************************
 *                           getCharacter                               *
 ************************************************************************/
Character* InitiativeInfo::getCharacter()
{
   return dude;
}

/************************************************************************
 *                             getValue                                 *
 ************************************************************************/
int InitiativeInfo::getValue()
{
   return value;
}



/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
Initiative::Initiative()
{
   current = NULL;
   next = NULL;
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
Initiative::~Initiative()
{
}

/***************************************************************
 *                      getFirstLesser                         *
 ***************************************************************/
InitiativeInfo* Initiative::getFirstLesser(int value)
{
   InitiativeInfo* inf = (InitiativeInfo*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      if(inf->getValue() < value)
      {
         return inf;
      }
      inf = (InitiativeInfo*) inf->getNext();
   }

   return inf;
}

/***************************************************************
 *                      insertCharacter                        *
 ***************************************************************/
void Initiative::insertCharacter(Character* pers)
{
   Dice d20;
   Factor attDex = Factor(MOD_TYPE_ATT, "DEXTERITY");
   Factor initiative = Factor(MOD_TYPE_THING, THING_ARMATURE_CLASS);

   /* Roll its initiative value */
   int value = d20.roll() + pers->getBonusValue(attDex) +
      pers->getBonusValue(initiative);

   /* Show user what it is */
   Briefing::addText(gettext("%s rolls initiative: %d."), 
         pers->getName().c_str(), value); 

   /* Check where and insert at list */
   InitiativeInfo* lesser = getFirstLesser(value);
   if(lesser == NULL)
   {
      /* None is lesser it, insert as last */
      Kobold::List::insertAtEnd(new InitiativeInfo(pers, value));
   }
   else
   {
      Kobold::List::insertBefore(new InitiativeInfo(pers, value), lesser);
   }
}

/***************************************************************
 *                           newRound                          *
 ***************************************************************/
void Initiative::newRound()
{
   next = (InitiativeInfo*) getFirst();
}

/***************************************************************
 *                        nextCharacter                        *
 ***************************************************************/
Character* Initiative::getNextCharacter()
{
   current = next;
   if(next)
   {
      next = (InitiativeInfo*) next->getNext();
      if(next == getFirst())
      {
         /* Got end of the list, must set as NULL */
         next = NULL;
      }
   }

   return getCurrentCharacter();
}

/***************************************************************
 *                      actualCharacter                        *
 ***************************************************************/
Character* Initiative::getCurrentCharacter()
{
   if(current)
   {
      return current->getCharacter();
   }
   else
   {
      return NULL;
   }
}

/***************************************************************
 *                            clear                            *
 ***************************************************************/
void Initiative::clear()
{
   Kobold::List::clear();
   
   current = NULL;
   next = NULL;
}

