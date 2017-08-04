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

#include "wall.h"

using namespace DNT;

/**************************************************************************
 *                                Constructor                             *
 **************************************************************************/
Wall::Wall(bool editMode)
{
   count++;
   mesh = new MapMesh(Kobold::String("wall") + 
         Kobold::StringUtil::toString(count));

   /* Define the upper wall texture */
   mesh->createSubMesh(MAP_UPPER_WALL_MATERIAL);
   if(editMode)
   {
      wallInfo = new Ogre::Vector3[2];
      matInfo = new Ogre::String[4];
   }
   else
   {
      wallInfo = NULL;
   }
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
Wall::~Wall()
{
   delete mesh;
   if(wallInfo)
   {
      delete[] wallInfo;
   }
   if(matInfo)
   {
      delete[] matInfo;
   }
}

/**************************************************************************
 *                                setInfo                                 *
 **************************************************************************/
void Wall::setInfo(Ogre::Vector3 min, Ogre::Vector3 max)
{
   wallInfo[0] = min;
   wallInfo[1] = max;
}

/**************************************************************************
 *                                addFace                                 *
 **************************************************************************/
void Wall::addFace(Kobold::String material, 
      Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
      Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
      Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ)
{
   MapSubMesh* subMesh = mesh->getSubMesh(material);
   if(!subMesh)
   {
      subMesh = mesh->createSubMesh(material);
   }
   subMesh->addSquare(x1, y1, z1, x2, y2, z2, normX, normY, normZ);
   if(matInfo)
   {
      if(normZ == -1.0f)
      {
         /* Front texture */
         matInfo[0] = material;
      }
      else if(normZ == 1.0f)
      {
         /* Back texture */
         matInfo[1] = material;
      }
      else if(normX == -1.0f)
      {
         /* Left texture */
         matInfo[2] = material;
      }
      else if(normX == 1.0f)
      {
         /* Right texture */
         matInfo[3] = material;
      }
   }
}

/**************************************************************************
 *                             updateMeshIfDirty                          *
 **************************************************************************/
void Wall::updateMeshIfDirty()
{
   mesh->updateAllDirty();
}

Ogre::uint32 Wall::count = 0;

