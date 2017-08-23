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

#include "pendingaction.h"
#include "scriptobjectcharacter.h"
#include "../ai/astar.h"
#include "../rules/character.h"
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
PendingActionSleep::PendingActionSleep(unsigned int seconds)
                   :PendingAction(TYPE_SLEEP)
{
   this->seconds = seconds;
   timer.resume();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
PendingActionSleep::~PendingActionSleep()
{
}

/**************************************************************************
 *                                 update                                 *
 **************************************************************************/
bool PendingActionSleep::update()
{
   return timer.getMilliseconds() > (seconds * 1000);
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
PendingActionMove::PendingActionMove(ScriptObjectCharacter* actor, 
      Character* actorPtr, float pX, float pZ)
                  :PendingAction(TYPE_MOVE_CHARACTER)
{
   this->actor = actor;
   this->pathFind = new AStar(false);
   this->pathFind->findPath(actorPtr, pX, pZ, 
         actorPtr->getWalkInterval(), true);
   this->state = STATE_SEARCHING;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
PendingActionMove::~PendingActionMove()
{
   if(pathFind)
   {
      pathFind->clear();
      delete pathFind;
   }
}

/**************************************************************************
 *                                 update                                 *
 **************************************************************************/
bool PendingActionMove::update()
{
   if(!actor->isValid())
   {
      /*! Character pointer is no longer valid, must finish. */
      return true;
   }

   if(state == STATE_SEARCHING)
   {
      AStar::AStarState astate = pathFind->getState();
      if(astate == AStar::ASTAR_STATE_FOUND)
      {
         /* Found a path, must start moving */
         state = STATE_MOVING;
         actor->setToMoveByFoundPath(pathFind);
      }
      else if(astate == AStar::ASTAR_STATE_NOT_FOUND)
      {
         /* Couldn't find a path, the pending action is finished, but
          * with the 'failed' way. */
         return true;
      }
   }
   else if(state == STATE_MOVING)
   {
      /* Check if character done with movement */
      return !actor->isMovingByPath();
   }
  
   /* Not done yet */
   return false;
}


