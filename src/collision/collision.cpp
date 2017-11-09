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

#include "collision.h"
#include "../rules/thing.h"
#include "../rules/character.h"
#include "../core/game.h"
#include "../map/map.h"

#define ASTAR_CALL_FACTOR 1.02f

#include <assert.h>

using namespace DNT;

/***********************************************************************
 *                                   init                              *
 ***********************************************************************/
void Collision::init(int x, int z, float squareSize)
{
   assert(grid == NULL);

   rwLock = new Kobold::ReadWriteLock();

   Collision::xSize = x;
   Collision::zSize = z;
   Collision::squareSize = squareSize;

   rwLock->lockForWrite();
   /* Create our square matrix (grid) */
   Collision::grid = new Square*[x];
   for(int i = 0; i < x; i++)
   {
      Collision::grid[i] = new Square[z];
      for(int j = 0; j < z; j++)
      {
         /* Define the square 'bouding box' */
         Collision::grid[i][j].setBoundingBox(
               Ogre::Vector3(i * squareSize, 0.0f, j * squareSize),
               Ogre::Vector3((i + 1) * squareSize, 
                  20.0f, (j + 1) * squareSize));
      }
   }
   rwLock->unlockForWrite();
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void Collision::finish()
{
   assert(grid != NULL);
   assert(rwLock != NULL);

   rwLock->lockForWrite();
   for(int x = 0; x < xSize; x++)
   {
      delete[] grid[x];
   }
   delete[] grid;
   grid = NULL;
   rwLock->unlockForWrite();

   delete rwLock;
   rwLock = NULL;
}

/***********************************************************************
 *                                canMove                              *
 ***********************************************************************/
bool Collision::canMove(Thing* actor, const Ogre::Vector3& varPos,
      const float varOri, float& newHeight, bool aStarCall)
{
   assert(actor != NULL);

   /* Apply position change */
   Ogre::Aabb walkableAabb = actor->getWalkableBounds();
   walkableAabb.mCenter = actor->getPosition() + varPos;

   if(aStarCall)
   {
      /* When searching with A*, we use a bit larger bounding box */
      walkableAabb.mHalfSize *= ASTAR_CALL_FACTOR;
   }

   return canBeAt(walkableAabb.getMinimum(), walkableAabb.getMaximum(), actor,
         newHeight);
}

/***********************************************************************
 *                                canBeAt                              *
 ***********************************************************************/
bool Collision::canBeAt(const Ogre::Vector3& min, const Ogre::Vector3& max,
      Thing* actor, float& newHeight)
{
   std::pair<bool, Element*> res;

   /* Make sure we aren't outside map bounds */
   if((min.x < 0.0f) || (min.z < 0.0f) || 
      (max.x > Game::getCurrentMap()->getSizeXWorld()) ||
      (max.z > Game::getCurrentMap()->getSizeZWorld()))
   {
      return false;
   }

   /* Note: the min.y decrement is necessary, otherwise if we are 'up' on an
    * element, it won't collide and our newHeight will be 0, which is wrong
    * (resulting in the actor going inner the element). */
   Ogre::AxisAlignedBox actorBox(
         Ogre::Vector3(min.x, min.y - 1.0f, min.z), max);

   /* Check all potentially occuped squares */
   int minqx = static_cast<int>((min.x) / squareSize);
   int minqz = static_cast<int>((min.z) / squareSize);
   int maxqx = static_cast<int>((max.x) / squareSize);
   int maxqz = static_cast<int>((max.z) / squareSize);

   rwLock->lockForRead();

   for(int x = minqx; x <= maxqx; x++)
   {
      for(int z = minqz; z <= maxqz; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            res = square->hasCollisions(actorBox, actor, newHeight);
            if(res.first)
            {
               rwLock->unlockForRead();
               return false;
            }
         }
      }
   }

   rwLock->unlockForRead();

   /* No collisions with any square */
   return true;
}

/***********************************************************************
 *                                canMove                              *
 ***********************************************************************/
bool Collision::canMove(Thing* actor, const Ogre::Vector3& origin,
      const Ogre::Vector3& destiny, bool aStarCall)
{
   assert(actor != NULL);

   float dist = (destiny - origin).length();
   Ogre::Aabb aabb = actor->getWalkableBounds();
   if(aStarCall)
   {
      /* When searching with A*, we use a bit larger bounding box */
      aabb.mHalfSize *= ASTAR_CALL_FACTOR;
   }

   /* Do a ray intersection between positions.
    * It'll use two rays, each from origin character's top to destiny floor,
    * and another from origin floor to character top at destiny */
   Ogre::Ray ray1(Ogre::Vector3(origin.x, 
            origin.y + aabb.mCenter.y + aabb.mHalfSize.y, origin.z),
            (destiny - origin).normalisedCopy()); 
   Ogre::Ray ray2(destiny, (origin - destiny).normalisedCopy());

   /* Define squares to check */
   int initSqX = static_cast<int>(origin.x / squareSize);
   int initSqZ = static_cast<int>(origin.z / squareSize);
   int endSqX = static_cast<int>(destiny.x / squareSize);
   int endSqZ = static_cast<int>(destiny.z / squareSize);

   rwLock->lockForRead();

   /* Check all potential squares for collisions */
   for(int x = initSqX; x <= endSqX; x++)
   {
      for(int z = initSqZ; z <= endSqZ; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            if((square->hasCollisions(ray1, dist, actor)) ||
               (square->hasCollisions(ray2, dist, actor)))
            {
               /* Collided: can't move. */
               rwLock->unlockForRead();
               return false;
            }
         }
      }
   }

   /* Do a bounding box check at final and middle positions */
   float newHeight = 0.0f;
   aabb.mCenter = destiny;
   if(!canBeAt(aabb.getMinimum(), aabb.getMaximum(), actor, newHeight))
   {
      rwLock->unlockForRead();
      return false;
   }
   aabb.mCenter = (destiny - origin) / 2.0f + origin;
   if(!canBeAt(aabb.getMinimum(), aabb.getMaximum(), actor, newHeight))
   {
      rwLock->unlockForRead();
      return false;
   }
   
   rwLock->unlockForRead();
   return true;
}

/***********************************************************************
 *                              canOccupy                              *
 ***********************************************************************/
bool Collision::canOccupy(Thing* actor, const Ogre::Vector3& pos, 
      float& newHeight)
{
   assert(actor != NULL);

   /* Define position */
   Ogre::Aabb aabb = actor->getWalkableBounds();
   aabb.mCenter = pos;
   
   return canBeAt(aabb.getMinimum(), aabb.getMaximum(), actor, newHeight);
}

/***********************************************************************
 *                              isAtSight                              *
 ***********************************************************************/
bool Collision::isAtSight(Thing* actor, Thing* target, const Ogre::Ray& ray, 
      const float& dist, int initSqX, int initSqZ, int endSqX, int endSqZ)
{
   std::pair<bool, Element*> res;
   bool collidedWithTarget = false;
   float curDist = dist;
   float collidedDist = dist;

   rwLock->lockForRead();

   /* Check all potential squares for collisions */
   for(int x = initSqX; x <= endSqX; x++)
   {
      for(int z = initSqZ; z <= endSqZ; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            res = square->hasCollisions(ray, curDist, collidedDist, actor);
            if(res.first)
            {
               /* Collided. For next checks we must use this as the 
                * minimum collision */
               curDist = collidedDist;
               /* Check if nearest collision was with the target. */
               collidedWithTarget = (res.second->thing == target);
            }
         }
      }
   }

   rwLock->unlockForRead();

   return collidedWithTarget;
}

/***********************************************************************
 *                              isAtSight                              *
 ***********************************************************************/
bool Collision::isAtSight(Thing* actor, Thing* target, bool reverseCheck)
{
   assert(actor != NULL);
   assert(target != NULL);
   
   if(actor == target)
   {
      /* One is always visible to itself */
      return true;
   }

   /* Define positions and direction */
   Ogre::Aabb aabb = actor->getWalkableBounds();

   Ogre::Vector3 actorPos = actor->getPosition();
   Ogre::Vector3 targetPos = target->getWorldAabb().mCenter;
   Ogre::Vector3 diffPos = targetPos - actorPos;
   float dist = diffPos.length();
   diffPos.normalise();

   /* Define squares to check */
   int initSqX = static_cast<int>(actorPos.x / squareSize);
   int initSqZ = static_cast<int>(actorPos.z / squareSize);
   int endSqX = static_cast<int>(targetPos.x / squareSize);
   int endSqZ = static_cast<int>(targetPos.z / squareSize);

   /* Make sure end is greater than init */
   if(endSqX < initSqX)
   {
      int tmp = endSqX;
      endSqX = initSqX;
      initSqX = tmp;
   }
   if(endSqZ < initSqZ)
   {
      int tmp = endSqZ;
      endSqZ = initSqZ;
      initSqZ = tmp;
   }

   /* Check lower actor to object's center */
   Ogre::Ray ray(actorPos, diffPos);
   if(isAtSight(actor, target, ray, dist, initSqX, initSqZ, endSqX, endSqZ))
   {
      /* Already got is at sight */
      return true;
   }

   /* Must check upper actor to object's center */
   Ogre::Vector3 upperActor(actorPos.x, 
         aabb.mCenter.y + aabb.mHalfSize.y, actorPos.z);
   diffPos = targetPos - upperActor;
   dist = diffPos.length();
   diffPos.normalise();
   ray = Ogre::Ray(upperActor, diffPos);
   if(isAtSight(actor, target, ray, dist, initSqX, initSqZ, endSqX, endSqZ))
   {
      return true;
   }

   /* Must check upper actor to object's up center */
   targetPos.y += target->getWorldAabb().mHalfSize.y;
   diffPos = targetPos - upperActor;
   dist = diffPos.length();
   diffPos.normalise();
   ray = Ogre::Ray(upperActor, diffPos);
   if(isAtSight(actor, target, ray, dist, initSqX, initSqZ, endSqX, endSqZ))
   {
      return true;
   }

   /* Must check the reverse */
   if(reverseCheck)
   {
      return isAtSight(target, actor, false);
   }

   return false;
}

/***********************************************************************
 *                              addElement                             *
 ***********************************************************************/
void Collision::addElement(const Ogre::Vector3& min, const Ogre::Vector3& max)
{
   addElement(min, max, NULL);
}

/***********************************************************************
 *                              addElement                             *
 ***********************************************************************/
void Collision::addElement(Thing* thing)
{
   assert(thing != NULL);
   Ogre::Aabb aabb;
   if(thing->getThingType() != Thing::THING_TYPE_CHARACTER)
   {
      aabb = thing->getWorldAabbUpdated();
   }
   else
   {
      aabb = thing->getWalkableBounds();
   }
   addElement(aabb.getMinimum(), aabb.getMaximum(), thing);
}

/***********************************************************************
 *                              addElement                             *
 ***********************************************************************/
void Collision::addElement(const Ogre::Vector3& min, 
      const Ogre::Vector3& max, Thing* thing)
{
   /* Should add the element on all squares it potentially is */
   int minqx = static_cast<int>((min.x) / squareSize);
   int minqz = static_cast<int>((min.z) / squareSize);
   int maxqx = static_cast<int>((max.x) / squareSize);
   int maxqz = static_cast<int>((max.z) / squareSize);

   rwLock->lockForWrite();

   for(int x = minqx; x <= maxqx; x++)
   {
      for(int z = minqz; z <= maxqz; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            square->addElement(min, max, thing);
         }
      }
   }

   rwLock->unlockForWrite();
}

/***********************************************************************
 *                           removeElement                             *
 ***********************************************************************/
void Collision::removeElement(Thing* thing)
{
   assert(thing != NULL);

   rwLock->lockForWrite();

   /* Remove all related Elements to the Thing */
   Kobold::List* colElements = thing->getColElements();
   Thing::ColElement* colEl = static_cast<Thing::ColElement*>(
         colElements->getFirst());
   for(int i = 0; i < colElements->getTotal(); i++)
   {
      colEl->element->square->remove(colEl->element);
      colEl = static_cast<Thing::ColElement*>(colEl->getNext());
   }
   colElements->clear();

   rwLock->unlockForWrite();
}

/***********************************************************************
 *                              getSquare                              *
 ***********************************************************************/
Square* Collision::getSquare(int x, int z)
{
   if((x >= 0) && (x < xSize) && (z >= 0) && (z < zSize))
   {
      return &grid[x][z];
   }

   return NULL;
}

/***********************************************************************
 *                          getRelativeSquare                          *
 ***********************************************************************/
Square* Collision::getRelativeSquare(float x, float z)
{
   int sqX = static_cast<int>(x / squareSize);
   int sqZ = static_cast<int>(z / squareSize);

   return getSquare(sqX, sqZ);
}

/***********************************************************************
 *                          setSquareWalkable                          *
 ***********************************************************************/
void Collision::setSquareWalkable(int x, int z, bool value)
{
   Square* square = getSquare(x, z);
   if(square)
   {
      square->setWalkable(value);
   }
}

/***********************************************************************
 *                          static attributes                          *
 ***********************************************************************/
Square** Collision::grid = NULL;
int Collision::xSize = 0;
int Collision::zSize = 0;
float Collision::squareSize = 0;
Kobold::ReadWriteLock* Collision::rwLock = NULL; 

