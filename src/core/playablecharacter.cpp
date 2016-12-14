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


#include "playablecharacter.h"
#include "../rules/classes.h"
using namespace DNT;

/*********************************************************************
 *                          PlayableCharacter                        *
 *********************************************************************/
PlayableCharacter::PlayableCharacter()
{
   this->upLevels = 0;
   this->xp = 0;
}

/*********************************************************************
 *                         ~PlayableCharacter                        *
 *********************************************************************/
PlayableCharacter::~PlayableCharacter()
{
}

/*********************************************************************
 *                             canClass                              *
 *********************************************************************/
bool PlayableCharacter::canClass(Class* cl)
{
   int i;
   for(i=0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      /* If one multiclass spaces is empty or the Character already has a 
       * level on this class, it can get a level on it. */
      if( (classes[i] == NULL) || (classes[i] == cl))
      {
         return true;
      }
   }

   return false;
}

/*********************************************************************
 *                          getNewClassLevel                         *
 *********************************************************************/
void PlayableCharacter::getNewClassLevel(Class* cl)
{
   int i;

   if( (canClass(cl)) && (upLevels > 0))
   {
      /* Got the level */
      upLevels--;

      /* Find the class space */
      i = 0;
      while( (i < CHARACTER_MAX_DISTINCT_CLASSES) &&
            (classes[i] != NULL) && (classes[i] != cl) )
      {
         i++;
      }
      assert(i != CHARACTER_MAX_DISTINCT_CLASSES);

      /* Define the class new level */
      classes[i] = cl;
      classLevel[i]++;

      /* Roll a life dice */
      int constitutionBonus = 
         getSkills()->getSkillByString("CONSTITUTION")->getAttributeBonus();
      Dice d(cl->getLifeDiceType());
      setMaxLifePoints(getMaxLifePoints() + d.roll() + constitutionBonus);

      /* Apply skill costs! */
      applySkillCosts();

      /* Apply the Saves and Bonus */
      applyBonusAndSaves();
   }
}

bool PlayableCharacter::doCharacterSpecializationParse(Ogre::String key, 
      Ogre::String value)
{
   //TODO
   return false;
}

bool PlayableCharacter::doCharacterSpecializationSave(std::ofstream& file)
{
   //TODO
   return true;
}


