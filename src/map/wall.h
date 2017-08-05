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

#ifndef _dnt_map_wall_h
#define _dnt_map_wall_h

#include "dntconfig.h"
#include "mapmesh.h"
#include <kobold/kstring.h>
#include <kobold/list.h>

namespace DNT
{

#define MAP_UPPER_WALL_MATERIAL         "upper_wall"

   /*! Representation of a Wall for indoor maps */
   class Wall : public Kobold::ListElement
   {
      public:
         /*! Constructor */
         Wall(bool editMode);
         /*! Destructor */
         ~Wall();

         /*! Add a face to the wall with an specific material */
         void addFace(Kobold::String material, 
               Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
               Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
               Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ);

         /*! Update wall mesh if it is dirty */
         void updateMeshIfDirty();

         /*! Set wall info (only on edit mode) */
         void setInfo(Ogre::Vector3 min, Ogre::Vector3 max);

         /*! \return wall info min (only on edit mode) */
         const Ogre::Vector3& getMin() const { return wallInfo[0]; };
         /*! \return wall info max (only on edit mode) */
         const Ogre::Vector3& getMax() const { return wallInfo[1]; };
         /*! \return material vector, as front, back, left, right 
          * (only on edit mode). */
         const Kobold::String& getMaterialInfo(int i) const 
         { return matInfo[i]; };

      private:
         MapMesh* mesh;  /**< Mesh representation of the wall */
         Ogre::Vector3* wallInfo; /**< Wall vertices info, only on edit mode */
         Kobold::String* matInfo; /**< material info, only on edit mode */
         static Ogre::uint32 count; /**< wall counter */
   };

}

#endif

