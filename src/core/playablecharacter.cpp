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
#include "../collision/collision.h"
#include "../ai/astar.h"
#include "../gui/briefing.h"
#include "../gui/dialogwidget.h"

#include <goblin/camera.h>

using namespace DNT;

/*! Delay before defining if is an A* or continous mouse move */
#define WALK_ACTION_DELAY   150
/*! How faster run is than walk */
#define RUN_MULTIPLIER 2.8f 
/*! Distance to toggle walk/run in constinous mouse move */
#define CONTINUOUS_RUN_DISTANCE 3 
/*! Minimum distance floorMouse should be to the character pos to change its
 * angle (too near and some indecision happens) */
#define MIN_DISTANCE_TO_CHANGE_ANGLE 0.5f


/*********************************************************************
 *                          PlayableCharacter                        *
 *********************************************************************/
PlayableCharacter::PlayableCharacter()
                  :Character(true)
{
   /* Create the A* path find to use */
   pathFind = new AStar(true);

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
   if(pathFind)
   {
      pathFind->clear();
      delete pathFind;
   }
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
         "direc.mesh", "models", Game::getSceneManager(),
         Goblin::Model3d::MODEL_DYNAMIC);
   direction->setScaleNow(0.13f, 0.13f, 0.13f);
   direction->hide();
}

bool PlayableCharacter::doCharacterSpecializationParse(
      const Kobold::String& key, const Kobold::String& value)
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
   Ogre::Vector3 curPos = getPosition();
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
      this->setOrientation(walkAngle);
   }
   else
   {
      /* Keep the direction angle */
      walkAngle = this->getOrientation();
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
      run = dist >= CONTINUOUS_RUN_DISTANCE - 0.5f;
   }
   else
   {
      run = dist >= CONTINUOUS_RUN_DISTANCE;
   }
   wasRunning = run;

   /* define direction model angle and position */
   float deltaRadius = (run) ? 1.5f : 0.8f;
   direction->setOrientationNow(-90.0f, 0.0f, walkAngle - 180);
   direction->setPositionNow(
         curPos.x + (deltaRadius) * dir[0], 
         curPos.y + 0.5f, 
         curPos.z + (deltaRadius) * dir[1]);
   
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
            walkPressTimer.pause();
         }
      }
      else if(!walkPressTimer.isPaused())
      {
         /* Clicked with mouse right button and released before
          * start a continous walk move by mouse: we should init
          * our A* search to walk. */
         pathFind->findPath(this, floorMouse.x, floorMouse.z, 
               getWalkInterval());
         walkState = WALK_ASTAR_SEARCHING;
         walkPressTimer.pause();
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
   else if(walkState == WALK_ASTAR_SEARCHING)
   {
      /* Check if A* search is done */
      AStar::AStarState astate = pathFind->getState();
      if(astate == AStar::ASTAR_STATE_FOUND)
      {
         /* Path is found. Move. */
         walkState = WALK_ASTAR_MOVING;
      }
      else if(astate == AStar::ASTAR_STATE_NOT_FOUND)
      {
         /* Path isn't avaiable, so do not run. */
         walkState = WALK_KEYBOARD;
         Briefing::addText(220, 20, 220, gettext("A* could not find a path!"),
            true, true);
      }
   }
   else if(walkState == WALK_ASTAR_MOVING)
   {
      /* Try to update to the new positon */
      Ogre::Vector3 pos = getPosition();
      float ori = getOrientation();
      if(pathFind->getNewPosition(pos, ori, false, 1.0f))
      {
         setPosition(pos);
         setTargetOrientation(ori, 4);
         moved = true;
         return true;
      }
      else
      {
         /* Done with movement */
         pathFind->clear();
         walkState = WALK_KEYBOARD;
      }
   }

   return false;
}

/***********************************************************************
 *                                 tryWalk                             *
 ***********************************************************************/
bool PlayableCharacter::tryWalk(float varX, float varZ)
{
   bool moved = false;
   Ogre::Vector3 curPos = getPosition();

   float newHeight = 0.0f;
   
   /* Check if can walk to the new position */
   if(Collision::canMove(this, Ogre::Vector3(varX, 0.0f, varZ), 0.0f, 
            newHeight, false))
   {
      moved = true;
      curPos.x += varX;
      curPos.z += varZ;
   }
   /* If can't move, let's try with only a single component */
   else if((Ogre::Math::Abs(varX) >= 0.1f) && 
           (Collision::canMove(this, Ogre::Vector3(varX, 0.0f, 0.0f), 
                               0.0f, newHeight, false)))
   {
      moved = true;
      curPos.x += varX;
   }
   else if((Ogre::Math::Abs(varZ) >= 0.1f) && 
           (Collision::canMove(this, Ogre::Vector3(0.0f, 0.0f, varZ), 
                               0.0f, newHeight, false)))
   {
      moved = true;
      curPos.z += varZ;
   }

   if(moved)
   {
      curPos.y = newHeight;
      setPosition(curPos);
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
   float curYaw = getOrientation();
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
      setOrientation(curYaw);
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

   if(isMovingByPath())
   {
      /* Following a path (probably called by script). Shouldn't try to manual
       * move, but still is a move, thus return true. */
      return true;
   }
  
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

   if(triedToMove && DialogWidget::isOpened())
   {
      DialogWidget::close();
   }

   if(moved)
   {
      flushAfterMove();
   }

   return triedToMove;
}

/***********************************************************************
 *                                 interact                            *
 ***********************************************************************/
Thing::InteractResult PlayableCharacter::interact(Character* actor)
{
   //TODO
   return INTERACTION_DONE;
}

