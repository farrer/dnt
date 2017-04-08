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

   return canBeAt(walkableAabb.getMinimum(), walkableAabb.getMaximum());
}

/***********************************************************************
 *                                canBeAt                              *
 ***********************************************************************/
bool Collision::canBeAt(const Ogre::Vector3& min, const Ogre::Vector3& max)
{
   Ogre::AxisAlignedBox actorBox(min, max);

   /* Check all potentially occuped squares */
   int minqx = (int)((min.x) / squareSize);
   int minqz = (int)((min.z) / squareSize);
   int maxqx = (int)((max.x) / squareSize);
   int maxqz = (int)((max.z) / squareSize);

   for(int x = minqx; x <= maxqx; x++)
   {
      for(int z = minqz; z <= maxqz; z++)
      {
         Square* square = getSquare(x, z);
         if(square)
         {
            if(square->hasCollisions(actorBox))
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
   
   return canBeAt(aabb.getMinimum(), aabb.getMaximum());
}

/***********************************************************************
 *                              isAtSight                              *
 ***********************************************************************/
bool Collision::isAtSight(Thing* actor, Thing* target)
{
   assert(actor != NULL);
   assert(target != NULL);

   //TODO
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
   int minqx = (int)((min.x) / squareSize);
   int minqz = (int)((min.z) / squareSize);
   int maxqx = (int)((max.x) / squareSize);
   int maxqz = (int)((max.z) / squareSize);

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
   int sqX = (int)x / squareSize;
   int sqZ = (int)z / squareSize;

   return getSquare(sqX, sqZ);
}

/***********************************************************************
 *                          static attributes                          *
 ***********************************************************************/
Collision::Square** Collision::grid = NULL;
int Collision::xSize = 0;
int Collision::zSize = 0;
float Collision::squareSize = 0;

