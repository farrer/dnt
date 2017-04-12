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

#include <assert.h>

using namespace DNT;

/***********************************************************************
 *                                   init                              *
 ***********************************************************************/
void Collision::init(int x, int z, float squareSize)
{
   assert(grid == NULL);

   Collision::xSize = x;
   Collision::zSize = z;
   Collision::squareSize = squareSize;

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
                  400.0f, (j + 1) * squareSize));
      }
   }
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void Collision::finish()
{
   assert(grid != NULL);
   if(grid != NULL)
   {
      for(int x = 0; x < xSize; x++)
      {
         delete[] grid[x];
      }
      delete[] grid;
      grid = NULL;
   }
}

/***********************************************************************
 *                                canMove                              *
 ***********************************************************************/
bool Collision::canMove(Thing* actor, const Ogre::Vector3& varPos,
      const float varOri)
{
   assert(actor != NULL);

   /* Apply position change */
   Ogre::Aabb worldAabb = actor->getModel()->getItem()->getWorldAabb();
   Ogre::Aabb walkableAabb = actor->getWalkableBounds();
   walkableAabb.mCenter = worldAabb.mCenter + varPos;

   return canBeAt(walkableAabb.getMinimum(), walkableAabb.getMaximum(), actor);
}

/***********************************************************************
 *                                canBeAt                              *
 ***********************************************************************/
bool Collision::canBeAt(const Ogre::Vector3& min, const Ogre::Vector3& max,
      Thing* actor)
{
   std::pair<bool, Element*> res;
   Ogre::AxisAlignedBox actorBox(min, max);

   /* Check all potentially occuped squares */
   int minqx = static_cast<int>((min.x) / squareSize);
   int minqz = static_cast<int>((min.z) / squareSize);
   int maxqx = static_cast<int>((max.x) / squareSize);
   int maxqz = static_cast<int>((max.z) / squareSize);

   for(int x = minqx; x <= maxqx; x++)
   {
      for(int z = minqz; z <= maxqz; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            res =square->hasCollisions(actorBox, actor);
            if(res.first)
            {
               return false;
            }
         }
      }
   }

   /* No collisions with any square */
   return true;
}

/***********************************************************************
 *                                canMove                              *
 ***********************************************************************/
bool Collision::canMove(Thing* actor, const Ogre::Vector3& origin,
      const Ogre::Vector3& destiny)
{
   assert(actor != NULL);

   //TODO
   return false;
}

/***********************************************************************
 *                              canOccupy                              *
 ***********************************************************************/
bool Collision::canOccupy(Thing* actor, const Ogre::Vector3& pos)
{
   assert(actor != NULL);

   /* Define position */
   Ogre::Aabb aabb = actor->getModel()->getItem()->getWorldAabb();
   aabb.mCenter = pos;
   
   return canBeAt(aabb.getMinimum(), aabb.getMaximum(), actor);
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
#if 0
               if(res.second->thing)
               {
                  printf("Collided with: %s\n", 
                        res.second->thing->getName().c_str());
               }
               else
               {
                  printf("Collided with wall\n");
               }
               printf("CurDist: %.3f\n", curDist);
#endif
            }
         }
      }
   }

   return collidedWithTarget;
}

/***********************************************************************
 *                              isAtSight                              *
 ***********************************************************************/
bool Collision::isAtSight(Thing* actor, Thing* target)
{
   assert(actor != NULL);
   assert(target != NULL);
   assert(actor != target);

   /* Define positions and direction */
   Ogre::Aabb aabb = actor->getWalkableBounds();
   Ogre::Vector3 actorPos = actor->getModel()->getPosition();
   Ogre::Vector3 targetPos = 
      target->getModel()->getItem()->getWorldAabb().mCenter;
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
         actorPos.y + aabb.mCenter.y + aabb.mHalfSize.y, actorPos.z);
   diffPos = targetPos - upperActor;
   diffPos.normalise();
   ray = Ogre::Ray(upperActor, diffPos);
   if(isAtSight(actor, target, ray, dist, initSqX, initSqZ, endSqX, endSqZ))
   {
      return true;
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
   Ogre::Aabb aabb = thing->getModel()->getItem()->getWorldAabbUpdated();
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
}

/***********************************************************************
 *                           removeElement                             *
 ***********************************************************************/
void Collision::removeElement(Thing* thing)
{
   for(int x = 0; x < xSize; x++)
   {
      for(int z = 0; z < zSize; z++)
      {
         grid[x][z].removeElement(thing);
      }
   }
}

/***********************************************************************
 *                              getSquare                              *
 ***********************************************************************/
Collision::Square* Collision::getSquare(int x, int z)
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
Collision::Square* Collision::getRelativeSquare(float x, float z)
{
   int sqX = static_cast<int>(x / squareSize);
   int sqZ = static_cast<int>(z / squareSize);

   return getSquare(sqX, sqZ);
}

/***********************************************************************
 *                          static attributes                          *
 ***********************************************************************/
Collision::Square** Collision::grid = NULL;
int Collision::xSize = 0;
int Collision::zSize = 0;
float Collision::squareSize = 0;

