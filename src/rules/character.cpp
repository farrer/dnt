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

#include "character.h"

#include "alignment.h"
#include "modeffect.h"

using namespace DNT;

/***********************************************************************
 *                                Character                            *
 ***********************************************************************/
Character::Character()
{
   this->dead = false;
   this->effects.setOwner(this);
   this->curAlign = NULL;
}

/***********************************************************************
 *                               ~Character                            *
 ***********************************************************************/
Character::~Character()
{
}

/***********************************************************************
 *                             addModEffect                            *
 ***********************************************************************/
void Character::addModEffect(int mod, int time, int periodicTime,
      Kobold::String factorId, Kobold::String factorType)
{
   /* Create ModEffect */
   ModEffect* modEffect = new ModEffect(mod, time, periodicTime, 
         factorId, factorType);

   /* Add and apply it */
   effects.insert(modEffect, true);
}

/***********************************************************************
 *                           removeAllModEffect                        *
 ***********************************************************************/
void Character::removeAllModEffects()
{
   effects.clear();
}

/***********************************************************************
 *                              getEffects                             *
 ***********************************************************************/
ModEffectList* Character::getEffects()
{
   return &effects;
}

/***********************************************************************
 *                             updateEffects                           *
 ***********************************************************************/
void Character::updateEffects()
{
   effects.doStep();
}

/***********************************************************************
 *                               isAlignOf                             *
 ***********************************************************************/
bool Character::isAlignOf(Kobold::String al)
{
   if(curAlign)
   {
      return curAlign->getStringId().find(al) != Kobold::String::npos;
   }

   return false;
}

/***********************************************************************
 *                           doSpecificParse                           *
 ***********************************************************************/
bool Character::doSpecificParse(Ogre::String key, Ogre::String value)
{
   //TODO
   return false;
}


