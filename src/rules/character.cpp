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
#include "classes.h"
#include "race.h"
#include "feats.h"
#include "modeffect.h"

#include <assert.h>

using namespace DNT;

/***********************************************************************
 *                                Character                            *
 ***********************************************************************/
Character::Character()
{
   /* Default: alive */
   this->dead = false;

   /* Set ModEffects owner */
   this->effects.setOwner(this);

   /* Nullify things */
   this->curAlign = NULL;
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      this->classes[i] = NULL;
      this->classLevel[i] = 0;
   }
   this->race = NULL;

   /* Create Feats list and define needed Feats */
   feats = new Feats();
   insertDefaultNeededFeats();

   /* Define default bare hands damage dice */
   bareHandsDice.setBaseDice(Dice(Dice::DICE_TYPE_D2));
   bareHandsDice.setInitialLevel(1);
}

/***********************************************************************
 *                               ~Character                            *
 ***********************************************************************/
Character::~Character()
{
   if(feats)
   {
      delete feats;
   }
}

/***********************************************************************
 *                        insertDefaultNeededFeats                     *
 ***********************************************************************/
void Character::insertDefaultNeededFeats()
{
   feats->insert(FeatsList::getFeatByNumber(FEAT_WEAPON_ATTACK));
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
   return doCharacterSpecializationParse(key, value);
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel()
{
   int totalLevels = 0;
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         totalLevels += classLevel[i];
      }
   }
   return totalLevels;
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel(Class* cl)
{
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] == cl)
      {
         return classLevel[i];
      }
   }
   return 0;
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel(Kobold::String classId)
{
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if((classes[i] != NULL) && 
         (classes[i]->getStringId() == classId))
      {
         return classLevel[i];
      }
   }
   return 0;
}

/*********************************************************************
 *                              instantKill                          *
 *********************************************************************/
void Character::instantKill()
{
   dead = true;
   //TODO: skeleton state: set as dead!
   //scNode->getModel()->setState(STATE_DEAD);
}

/*********************************************************************
 *                           applySkillCosts                         *
 *********************************************************************/
void Character::applySkillCosts()
{
   /* Clear current costs */
   sk.clearCosts();

   /* Apply Race Costs */
   if(race) 
   {
      race->applySkillCosts(&sk);
   }

   //FIXME Classes Costs will be only per actual class?
   //      In the way bellow is always for all classes.
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         classes[i]->applySkillCosts(&sk);
      }
   }
}

/*********************************************************************
 *                         applyBonusAndSaves                        *
 *********************************************************************/
void Character::applyBonusAndSaves()
{
   /* Clear the current */
   curBonusAndSaves.clear();

   /* Add from all classes */
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         curBonusAndSaves = curBonusAndSaves + 
            classes[i]->getBonusAndSaves(classLevel[i] - 1);
      }
   }
}


