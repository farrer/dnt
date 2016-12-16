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

#include <goblin/camera.h>

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


/***********************************************************************
 *                           checkInputForMovement                     *
 ***********************************************************************/
bool PlayableCharacter::checkInputForMovement()
{
   //TODO: get keys from options!

   //TODO: Run.

   bool triedToMove = false;
   Ogre::Vector3 curPos = getModel()->getPosition();
   float curYaw = getModel()->getYaw();
   float curWalk = getWalkInterval();
   float curTurnAround = getTurnAroundInterval();
   float varX, varZ, varOri;

   /* Let's check for keyboard movement */

   /* Forward and backward movement */
   if((Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_W)) ||
      (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_S)))
   {
      triedToMove = true;

      varX = curWalk * Ogre::Math::Sin(Ogre::Radian(Ogre::Degree(curYaw)));
      varZ = curWalk * Ogre::Math::Cos(Ogre::Radian(Ogre::Degree(curYaw)));

      if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_S))
      {
         /* backward */
         varX *= -1.0f;
         varZ *= -1.0f;
      }

      /* TODO: check if can walk to the new position */
      curPos.x += varX;
      curPos.z += varZ;
      getModel()->setPosition(curPos);
      Goblin::Camera::setPosition(curPos);
   }

   /* Rotate left and rotate right */
   if((Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_A)) ||
      (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_D)))
   {
      triedToMove = true;
      varOri = curTurnAround;

      if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_D))
      {
         /* Clockwise! */
         varOri *= -1.0f;
      }

      curYaw += varOri;
      /* Check within [0, 360) interval */
      if(curYaw < 0)
      {
         curYaw += 360.0f;
      }
      else if(curYaw >= 360.0f)
      {
         curYaw -= 360.0f;
      }

      //TODO: check if can turn around!

      /* Apply new yaw */
      getModel()->setOrientation(curYaw);
   }

   if((triedToMove) && (getCurrentAnimation() != CHARACTER_ANIMATION_WALK))
   {
      setAnimation(CHARACTER_ANIMATION_WALK, true);
   }
   else if((!triedToMove) && 
          (getCurrentAnimation() == CHARACTER_ANIMATION_WALK))
   {
      setAnimation(CHARACTER_ANIMATION_IDLE, true);
   }

   return triedToMove;
}

