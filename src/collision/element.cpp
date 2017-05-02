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
#include "overlaps.h"
#include "../rules/thing.h"
#include "../core/util.h"
#include <OGRE/OgreMath.h>

#include <assert.h>

using namespace DNT;

#define MAX_WALKABLE_DIFF_HEIGHT  1.5f

/***********************************************************************
 *                          Constructor                                *
 ***********************************************************************/
Element::Element(Square* square, 
      const Ogre::Vector3& min, const Ogre::Vector3& max, Thing* thing)
   :bounds(min, max)
{
   this->thing = thing;
   this->square = square;
}

/***********************************************************************
 *                          isWalkable                                 *
 ***********************************************************************/
bool Element::isWalkable(Thing* actor)
{
   return (bounds.getMaximum().y - actor->getModel()->getPosition().y) 
                  <= MAX_WALKABLE_DIFF_HEIGHT;
}

/***********************************************************************
 *                         depthCollide                                *
 ***********************************************************************/
bool Element::depthCollide(const Ogre::AxisAlignedBox& actorBox)
{
   assert(thing != NULL);

   Ogre::Vector3 pos = thing->getModel()->getPosition();
   size_t vertexCount = 0;
   size_t indexCount = 0;
   Ogre::Vector3* vertices;
   Ogre::uint32* indices;

   thing->getModel()->getCachedMesh(vertexCount, vertices, 
         indexCount, indices);

   Ogre::Radian angleX = Ogre::Radian(Ogre::Degree(
            thing->getModel()->getPitch()));
   Ogre::Radian angleY = Ogre::Radian(Ogre::Degree(
            thing->getModel()->getOrientation()));
   Ogre::Radian angleZ = Ogre::Radian(Ogre::Degree(
            thing->getModel()->getRoll()));

   /* Precalculate the sin and cos of the angles, to do less calculations */
   float sinAngleX = Ogre::Math::Sin(angleX);
   float cosAngleX = Ogre::Math::Cos(angleX);
   float sinAngleY = Ogre::Math::Sin(angleY);
   float cosAngleY = Ogre::Math::Cos(angleY);
   float sinAngleZ = Ogre::Math::Sin(angleZ);
   float cosAngleZ = Ogre::Math::Cos(angleZ);

   float V0[3], V1[3], V2[3];
   float U0[3], U1[3], U2[3];
   int index0=0, index1=0, index2=0;

   /* Let's check each triangle */
   for(size_t i = 0; i < indexCount; i+=3)
   {
      /* Define Triangle indices */
      index0 = indices[i];
      index1 = indices[i+1];
      index2 = indices[i+2];

      /* Rotate and translate it to the desired position */
      Util::rotatePoint(vertices[index0][0], vertices[index0][1],
            vertices[index0][2], angleX, angleY, angleZ, 
            sinAngleX, cosAngleX, sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
            V0[0], V0[1], V0[2]);
      V0[0] += pos.x;
      V0[1] += pos.y;
      V0[2] += pos.z;

      Util::rotatePoint(vertices[index1][0], vertices[index1][1],
            vertices[index1][2], angleX, angleY, angleZ, 
            sinAngleX, cosAngleX, sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
            V1[0], V1[1], V1[2]);
      V1[0] += pos.x;
      V1[1] += pos.y;
      V1[2] += pos.z;

      Util::rotatePoint(vertices[index2][0], vertices[index2][1],
            vertices[index2][2],
            angleX, angleY, angleZ, sinAngleX, cosAngleX,
            sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
            V2[0], V2[1], V2[2]);
      V2[0] += pos.x;
      V2[1] += pos.y;
      V2[2] += pos.z;

      /* Bounding Box Faces */

      /* Upper Face A */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMinimum().z;

      U1[0] = actorBox.getMaximum().x;
      U1[1] = actorBox.getMaximum().y;
      U1[2] = actorBox.getMinimum().z;

      U2[0] = actorBox.getMinimum().x;
      U2[1] = actorBox.getMaximum().y;
      U2[2] = actorBox.getMaximum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Upper Face A! */
         return true;
      }

      /* Upper Face B */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMaximum().z;
      //U1 = same as upper face A
      //U2 = same as upper face A


      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Upper Face B! */
         return true;
      }

      /* Lower Face A */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMinimum().z;

      U1[0] = actorBox.getMaximum().x;
      U1[1] = actorBox.getMinimum().y;
      U1[2] = actorBox.getMinimum().z;

      U2[0] = actorBox.getMinimum().x;
      U2[1] = actorBox.getMinimum().y;
      U2[2] = actorBox.getMaximum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Lower Face A! */
         return true;
      }

      /* Lower Face B */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMaximum().z;
      //U1 = same as lower face A
      //U2 = same as lower face A

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Lower Face B! */
         return true;
      }

      /* Left Face A */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMinimum().z;

      U1[0] = actorBox.getMinimum().x;
      U1[1] = actorBox.getMinimum().y;
      U1[2] = actorBox.getMaximum().z;

      U2[0] = actorBox.getMinimum().x;
      U2[1] = actorBox.getMaximum().y;
      U2[2] = actorBox.getMinimum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Left Face A! */
         return true;
      }

      /* Left Face B */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMaximum().z;
      //U1 = same as left face A
      //U2 = same as left face A

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Left Face B! */
         return true;
      }

      /* Right Face A */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMinimum().z;

      U1[0] = actorBox.getMaximum().x;
      U1[1] = actorBox.getMinimum().y;
      U1[2] = actorBox.getMaximum().z;

      U2[0] = actorBox.getMaximum().x;
      U2[1] = actorBox.getMaximum().y;
      U2[2] = actorBox.getMinimum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Right Face A! */
         return true;
      }

      /* Right Face B */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMaximum().z;
      //U1 = same as right face A
      //U2 = same as right face A

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Right Face B! */
         return true;
      }

      /* Front Face A */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMinimum().z;

      U1[0] = actorBox.getMaximum().x;
      U1[1] = actorBox.getMinimum().y;
      U1[2] = actorBox.getMinimum().z;

      U2[0] = actorBox.getMinimum().x;
      U2[1] = actorBox.getMaximum().y;
      U2[2] = actorBox.getMinimum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Front Face A! */
         return true;
      }

      /* Front Face B */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMinimum().z;
      //U1 = same as front face A
      //U2 = same as front face A

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Front Face B! */
         return true;
      }

      /* Back Face A */
      U0[0] = actorBox.getMinimum().x;
      U0[1] = actorBox.getMinimum().y;
      U0[2] = actorBox.getMaximum().z;

      U1[0] = actorBox.getMaximum().x;
      U1[1] = actorBox.getMinimum().y;
      U1[2] = actorBox.getMaximum().z;

      U2[0] = actorBox.getMinimum().x;
      U2[1] = actorBox.getMaximum().y;
      U2[2] = actorBox.getMaximum().z;

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Back Face A! */
         return true;
      }

      /* Back Face B */
      U0[0] = actorBox.getMaximum().x;
      U0[1] = actorBox.getMaximum().y;
      U0[2] = actorBox.getMaximum().z;
      //U1 = same as back face A
      //U2 = same as back face A

      if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
      {
         /* Detected Collision at Back Face B! */
         return true;
      }
   }

   /* No collision happened */
   return false;
}

