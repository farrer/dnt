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
#include "game.h"
#include "util.h"
#include "../rules/classes.h"
#include "../collision/collision.h"

#include <goblin/camera.h>

using namespace DNT;

/*! Delay before defining if is an A* or continous mouse move */
#define WALK_ACTION_DELAY   250
/*! How faster run is than walk */
#define RUN_MULTIPLIER 2.8f 
/*! Distance to toggle walk/run in constinous mouse move */
#define CONTINUOUS_RUN_DISTANCE 60 
/*! Minimum distance floorMouse should be to the character pos to change its
 * angle (too near and some indecision happens) */
#define MIN_DISTANCE_TO_CHANGE_ANGLE 10


/*********************************************************************
 *                          PlayableCharacter                        *
 *********************************************************************/
PlayableCharacter::PlayableCharacter()
{
   this->upLevels = 0;
   this->xp = 0;
   this->walkState = WALK_KEYBOARD;
   this->walkPressTimer.pause();
   this->direction = NULL;
   this->wasRunning = false;
}

/*********************************************************************
 *                         ~PlayableCharacter                        *
 *********************************************************************/
PlayableCharacter::~PlayableCharacter()
{
   if(direction)
   {
      delete direction;
   }
}

/*********************************************************************
 *                            doAfterLoad                            *
 *********************************************************************/
void PlayableCharacter::doAfterLoad()
{
   Character::doAfterLoad();
   direction = new Goblin::Model3d("directMesh", 
         "other/direc.mesh", Game::getSceneManager(),
         Goblin::Model3d::MODEL_DYNAMIC);
   direction->setScale(2.6f, 2.6f, 2.6f);
   direction->hide();
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
 *                           doMovementByMouse                         *
 ***********************************************************************/
bool PlayableCharacter::doMovementByMouse(const Ogre::Vector3& floorMouse,
      bool& run)
{
   Ogre::Vector3 curPos = getModel()->getPosition();
   Ogre::Vector2 dir(floorMouse.x - curPos.x, floorMouse.z - curPos.z);
   /* Set character orientation (if mouse is far from character,
    * because if it is too near, some weird angles appears) */
   float dist = dir.length();
   dir.normalise();

   float walkAngle = Util::getAngle(floorMouse.x, floorMouse.z,
         curPos.x, curPos.z);
   if(dist > MIN_DISTANCE_TO_CHANGE_ANGLE)
   {
      /* Change the angle */
      this->getModel()->setOrientation(walkAngle);
   }
   else
   {
      /* Keep the direction angle */
      walkAngle = this->getModel()->getOrientation();
      Ogre::Vector3 cDir = this->direction->getPosition() - curPos; 
      dir = Ogre::Vector2(cDir.x, cDir.z);
      dir.normalise();
   }

   /* Verify if is running or walking */
   if(wasRunning)
   {
      /* if was running, avoid toggling to walk too early, puting
       * the toggle distance nearer the character. This way, we avoid
       * unwanted effects of run/walk/run/walk glitches when mouse
       * is near CONTINUOUS_RUN_DISTANCE. */
      run = dist >= CONTINUOUS_RUN_DISTANCE - 10.0f;
   }
   else
   {
      run = dist >= CONTINUOUS_RUN_DISTANCE;
   }
   wasRunning = run;

   /* define direction model angle and position */
   float deltaRadius = (run) ? 30.0f : 12.0f;
   direction->setOrientation(-90.0f, 0.0f, walkAngle - 180);
   Ogre::Aabb aabb = getWalkableBounds();
   direction->setPosition(curPos.x + (aabb.mCenter.x + deltaRadius) * dir[0], 
         curPos.y + 10.0f, 
         curPos.z + (aabb.mCenter.z + deltaRadius) * dir[1]);
   
   /* Reset, now for the continuous walk, the interval */
   float curWalk = getWalkInterval();
   if(run)
   {
      curWalk = curWalk * RUN_MULTIPLIER;
   }

   /* Try to move it forward the angle */
   Ogre::Radian angleRad = Ogre::Radian(Ogre::Degree(walkAngle));
   float varX = curWalk * Ogre::Math::Sin(angleRad);
   float varZ = curWalk * Ogre::Math::Cos(angleRad);
   return tryWalk(varX, varZ);
}

/***********************************************************************
 *                      checkMouseInputForMovement                     *
 ***********************************************************************/
bool PlayableCharacter::checkMouseInputForMovement(
      const Ogre::Vector3& floorMouse, bool& moved, bool& run)
{
   moved = false;
   if(walkState == WALK_KEYBOARD)
   {
      /* Not yet walking with mouse, check init */
      if(Farso::Cursor::isRightButtonPressed())
      {
         /* Init mouse press time (if not yet) */
         if(walkPressTimer.isPaused())
         {
            walkPressTimer.reset();
            return true;
         }
         /* verify if should start or continue to walk */
         else if(walkPressTimer.getMilliseconds() >= WALK_ACTION_DELAY)
         {
            walkState = WALK_MOUSE;
            direction->show();
            //TODO: clear any A* from the character
         }
      }
      else if(!walkPressTimer.isPaused())
      {
         /* Clicked with mouse right button and released before
          * start a continous walk move by mouse: we should init
          * our A* search to walk. */
         //TODO findPath
         walkState = WALK_ASTAR;
      }
   }

   if(walkState == WALK_MOUSE)
   {
      if(Farso::Cursor::isRightButtonPressed())
      {
         moved = doMovementByMouse(floorMouse, run);
         return true;
      }
      else
      {
         /* Done with mouse walking. */ 
         walkState = WALK_KEYBOARD;
         direction->hide();
      }
   }
   else if(walkState == WALK_ASTAR)
   {
      //TODO: check if ASTAR done and walk, etc.
      //for now, just going back to KEYBOARD mode
      walkState = WALK_KEYBOARD;
   }

   return false;
}

/***********************************************************************
 *                                 tryWalk                             *
 ***********************************************************************/
bool PlayableCharacter::tryWalk(float varX, float varZ)
{
   bool moved = false;
   Ogre::Vector3 curPos = getModel()->getPosition();
   
   /* Check if can walk to the new position */
   if(Collision::canMove(this, Ogre::Vector3(varX, 0.0f, varZ), 0.0f))
   {
      moved = true;
      curPos.x += varX;
      curPos.z += varZ;
   }
   /* If can't move, let's try with only a single component */
   else if((varX != 0.0f) && 
           (Collision::canMove(this, Ogre::Vector3(varX, 0.0f, 0.0f), 0.0f)))
   {
      moved = true;
      curPos.x += varX;
   }
   else if((varZ != 0.0f) && 
           (Collision::canMove(this, Ogre::Vector3(0.0f, 0.0f, varZ), 0.0f)))
   {
      moved = true;
      curPos.z += varZ;
   }

   if(moved)
   {
      getModel()->setPosition(curPos);
      Goblin::Camera::setPosition(curPos);
   }

   return moved;
}

/***********************************************************************
 *                     checkKeyboardInputForMovement                   *
 ***********************************************************************/
bool PlayableCharacter::checkKeyboardInputForMovement(bool& moved, bool& run)
{
   //TODO: get keys from options!
   bool triedToMove = false;
   float curYaw = getModel()->getYaw();
   float curWalk = getWalkInterval();
   float curTurnAround = getTurnAroundInterval();
   float varX, varZ, varOri;
   
   /* Let's check for keyboard movement */
   run = false;
   if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_LSHIFT))
   {
      run = true;
   }
   if(run)
   {
      curWalk *= RUN_MULTIPLIER; 
   }

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

      /* Check if can walk to the new position */
      moved = tryWalk(varX, varZ);
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

      /* Note: as using a fixed bouding box (for more fluid movement),
       * always can turn around (avoiding hangs when colliding) */
      moved = true;

      /* Apply new yaw */
      getModel()->setOrientation(curYaw);
   }

   return triedToMove;
}

/***********************************************************************
 *                           checkInputForMovement                     *
 ***********************************************************************/
bool PlayableCharacter::checkInputForMovement(const Ogre::Vector3& floorMouse)
{
   bool triedToMove = false;
   bool moved = false;
   bool run = false;
  
   /* First check if tryed to move if mouse, and if not, check keyboard */
   triedToMove = checkMouseInputForMovement(floorMouse, moved, run);
   if((!triedToMove) && (walkState == WALK_KEYBOARD))
   {
      triedToMove = checkKeyboardInputForMovement(moved, run);
   }
 
   if((moved) && (!run) && (getCurrentAnimation() != CHARACTER_ANIMATION_WALK))
   {
      setAnimation(CHARACTER_ANIMATION_WALK, true);
   }
   else if((moved) && (run) && 
           (getCurrentAnimation() != CHARACTER_ANIMATION_RUN))
   {
      setAnimation(CHARACTER_ANIMATION_RUN, true);
   }
   else if((!moved) && ((getCurrentAnimation() == CHARACTER_ANIMATION_WALK) ||
                        (getCurrentAnimation() == CHARACTER_ANIMATION_RUN)) )
   {
      setAnimation(CHARACTER_ANIMATION_IDLE, true);
   }

   return triedToMove;
}

