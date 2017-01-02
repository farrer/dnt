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
Wall::Wall()
{
   count++;
   mesh = new MapMesh(Kobold::String("wall") + 
         Kobold::StringUtil::toString(count));

   /* Define the upper wall texture */
   mesh->createSubMesh(MAP_UPPER_WALL_MATERIAL);

}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
Wall::~Wall()
{
   delete mesh;
}

/**************************************************************************
 *                                  addFace                               *
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
}

/**************************************************************************
 *                             updateMeshIfDirty                          *
 **************************************************************************/
void Wall::updateMeshIfDirty()
{
   mesh->updateAllDirty();
}

Ogre::uint32 Wall::count = 0;

