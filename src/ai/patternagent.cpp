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

#include "patternagent.h"
#include "../core/util.h"
#include <OGRE/OgreMath.h>
using namespace DNT;

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
PatternAgent::PatternAgent(bool oriented)
             :Agent(oriented)
{
   totalWayPoints = 0;
   totalWalked = 0;
   origX = 0;
   origZ = 0;
   xInc = 0.0f;
   zInc = 0.0f;
   curWayPoint = NULL;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
PatternAgent::~PatternAgent()
{
}

/********************************************************************
 *                         getTotalWayPoints                        *
 ********************************************************************/
int PatternAgent::getTotalWayPoints()
{
   return wayPoints.getTotal();
}

/********************************************************************
 *                        getTotalWalked                            *
 ********************************************************************/
float PatternAgent::getTotalWalked()
{
   return totalWalked;
}

/********************************************************************
 *                     getCurrentWayPoint                           *
 ********************************************************************/
WayPoint* PatternAgent::getCurrentWayPoint()
{
   return curWayPoint;
}

/********************************************************************
 *                          setOrigin                               *
 ********************************************************************/
void PatternAgent::setOrigin(float x, float z)
{
   origX = x;
   origZ = z;
}

/********************************************************************
 *                         calculateAngle                           *
 ********************************************************************/
void PatternAgent::calculateAngle(WayPoint* way, WayPoint* previous)
{
   if(withOrientation)
   {
      way->angle = Util::getAngle(way->x, way->z, previous->x, previous->z);
   }
}

/********************************************************************
 *                    define Next Position                          *
 ********************************************************************/
bool PatternAgent::defineNextPosition(bool run, float runMultiplier)
{
   float varX=0, varZ=0;
   float xIncCur, zIncCur;

   if(!curWayPoint)
   {
      /* No WayPoints defined. don't move. */
      return false; 
   }

   if( (actualX == curWayPoint->x) && (actualZ == curWayPoint->z))
   {
      /* Arrived at the actual Way Point, so change to next! */
      changeToNextWayPoint();
      
      /* calculate angle */
      desiredAngle = curWayPoint->angle;
      if(doAngle())
      {
         return true;
      }
      
   }
   else if(doAngle())
   {
      return true;
   }


   /* Set current increment */
   if(run)
   {
      xIncCur = xInc * runMultiplier;
      zIncCur = zInc * runMultiplier;
   }
   else
   {
      xIncCur = xInc;
      zIncCur = zInc;
   }

   /* Update position, making sure that goes exactly to the waypoint */
   if( ((xIncCur > 0) && (actualX + xIncCur >= curWayPoint->x)) ||
       ((xIncCur < 0) && (actualX + xIncCur <= curWayPoint->x)) ||
       (xIncCur == 0))
   {
      actualX = curWayPoint->x;
   }
   else
   {
      actualX += xIncCur;
   }

   if( ((zIncCur > 0) && (actualZ + zIncCur >= curWayPoint->z)) ||
       ((zIncCur < 0) && (actualZ + zIncCur <= curWayPoint->z)) ||
       (zIncCur == 0))
   {
      actualZ = curWayPoint->z;
   }
   else
   {
      actualZ += zIncCur;
   }

   /* Calculate the linear distance to the orign */ 
   varX = actualX - origX;
   varZ = actualZ - origZ;
   totalWalked = Ogre::Math::Sqrt(varX*varX + varZ*varZ);

   return true;
}

/********************************************************************
 *                           addWayPoint                            *
 ********************************************************************/
void PatternAgent::addWayPoint(float x, float z)
{
   WayPoint* newWay = new(WayPoint);
   newWay->x = x;
   newWay->z = z;
   
   /* insert at the wayPoints */
   wayPoints.insertAtEnd(newWay);

   /* Calculate the orientation angle */
   if( (newWay) != (newWay->getPrevious()))
   {
      /* Calculate angle between this WayPoint and the previous one  */
      calculateAngle(newWay, (WayPoint*)newWay->getPrevious());
   }
   else
   {
      /* First WayPoint, so orientation is equal to current */
      newWay->angle = orientation;
   }

   /* Put orientation at range */
   while(newWay->angle < 0)
   {
      newWay->angle += 360;
   }
   while(newWay->angle >= 360)
   {
      newWay->angle -= 360;
   }

   /* Set the current, if not defined */
   if(curWayPoint == NULL)
   {
      curWayPoint = newWay;
   }
}

/********************************************************************
 *                        addWayPointFirst                          *
 ********************************************************************/
void PatternAgent::addWayPointFirst(float x, float z)
{
   WayPoint* newWay = new(WayPoint);
   newWay->x = x;
   newWay->z = z;

   /* insert at the wayPoints */
   wayPoints.insertAtBegin(newWay);

   /* Calculate the orientation angle */
   if( (newWay) != (newWay->getNext()))
   {
      /* Calculate angle between the next WayPoint and this one  */
      calculateAngle((WayPoint*)newWay->getNext(), newWay);
   }
   else
   {
      /* First WayPoint, so orientation is equal to current */
      newWay->angle = orientation;
   }

   /* Always set the current */
   curWayPoint = newWay;
}

/********************************************************************
 *                         removeWayPoint                           *
 ********************************************************************/
void PatternAgent::removeWayPoint(WayPoint* way)
{
   if(way)
   {
      /* Update the current */
      if(way == curWayPoint)
      {
         curWayPoint = (WayPoint*)curWayPoint->getNext();
      }

      /* Remove from wayPoints */
      wayPoints.remove(way);

      /* Verify if has more elements */
      if(wayPoints.getTotal() == 0)
      {
         curWayPoint = NULL;
      }
   }
}

/********************************************************************
 *                      removeLinearWayPoints                       *
 ********************************************************************/
void PatternAgent::removeLinearWayPoints()
{
   /* note: Only can't remove the first and the last WayPoints. */
   int total = wayPoints.getTotal();
   int i;
   WayPoint* way = (WayPoint*)wayPoints.getFirst();
   WayPoint* oth;
   float valNext;
   for(i = 0; i<total; i++)
   {
      oth = way;
      way = (WayPoint*)way->getNext();
      if( (oth != wayPoints.getFirst()) && 
          (oth != wayPoints.getLast()) )
      {
         valNext = ((WayPoint*)oth->getNext())->angle;
         if( (oth->angle >= valNext-0.1f) &&
             (oth->angle <= valNext+0.1f) )
         {
            /* Can remove the WayPoint */
            removeWayPoint(oth);
         }
      }
   }

   way = (WayPoint*)wayPoints.getFirst();
   for(i=0; i < wayPoints.getTotal(); i++)
   {
      way = (WayPoint*)way->getNext();
   }
}


/********************************************************************
 *                       changeTONextWayPoint                       *
 ********************************************************************/
void PatternAgent::changeToNextWayPoint()
{
   float loops;
   if(!curWayPoint)
   {
      return; //not defined yet the way points, so stay static.
   }
   curWayPoint = (WayPoint*)curWayPoint->getNext();

   if(curWayPoint)
   {
      float dx = curWayPoint->x - actualX;
      float dz = curWayPoint->z - actualZ;
      //float dist = sqrt((dx*dx) + (dz*dz));

      loops = (sqrt((dx*dx)+(dz*dz)) / stepSize);
      if(loops == 0)
      {
         xInc = 0;
         zInc = 0;
      }
      else
      {
         xInc = (float) (dx) / (loops);
         zInc = (float) (dz) / (loops);
      }
   }
   
}

