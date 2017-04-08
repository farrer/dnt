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

#ifndef _dnt_collision_h
#define _dnt_collision_h

#include "dntconfig.h"
#include <OGRE/OgreAxisAlignedBox.h>
#include <kobold/list.h>

namespace DNT
{

   /*! The class which controls the collision system for DNT, checking for
    * collisions, both on walk tentatives and on sigh and free-area ahead 
    * checks. */
   class Collision
   {
      public:
         /*! Init the Collision system for a Map, creating the needed
          * internal structures.
          * \note should be called every time changed current opened map.
          * \param x number of squares on X axis
          * \param z number of squares on Z axis 
          * \param squareSize size of each square side, in world units. */
         static void init(int x, int z, float squareSize);

         /*! Finish the collision system for a Map, freeing its structures.
          * \note should be called every time exited from a map. */
         static void finish();

         /*! Check if a Thing can move from its current position (and 
          * orientation) to a new position (and orientarion).
          * \param actor thing to move
          * \param varPos delta to move on its position
          * \param varOri delta to move on its orientation
          * \return true if can move, false if will collide with something
          *         else and can't */
         static bool canMove(Thing* actor, const Ogre::Vector3& varPos,
                             const float varOri);

         /*! Check if a Thing can move from a defined position to another
          * one. This check will be made by using rays from and to 
          * origin/destiny, and usually are made for A* in-between targets 
          * checks (for the targets themselves, a canOccupy check is made) 
          * \param actor Thing to check its movement
          * \param origin origin of the movement
          * \param destiny destiny of the movement 
          * \return true if can move, false if can't. */
         static bool canMove(Thing* actor, const Ogre::Vector3& origin,
                             const Ogre::Vector3& destiny);

         /*! Check if a thing can occupy an specific position on the map.
          * \param actor to check if can occupy
          * \param pos position to check if can occupy
          * \return true if can, false otherwise. */
         static bool canOccupy(Thing* actor, const Ogre::Vector3& pos);

         /*! Check if a Thing is at sight of another one.
          * \param actor to check if target is at its sight
          * \param target to check if is at actor's sight
          * \return true if target is at actor sight, and false if something
          *         is between them. */
         static bool isAtSight(Thing* actor, Thing* target);

         /*! Add an element, without a related Thing.
          * \param min minumum corner values
          * \param max maximum corner values */
         static void addElement(const Ogre::Vector3& min, 
               const Ogre::Vector3& max);

         /*! Add an element with a related Thing */ 
         static void addElement(Thing* thing);

      private:

         /* This subclass implements an element that should have 
          * collision enabled. */
         class Element : public Kobold::ListElement
         {
            public:
               /*! Constructor
                * \param min minimum corner of the box
                * \param max maximum corner of the box 
                * \param thing pointer to the related thing, if any. */
               Element(const Ogre::Vector3& min, const Ogre::Vector3& max,
                     Thing* thing);

               Ogre::AxisAlignedBox bounds; /**< Current element bounds */
               Thing* thing;  /**< Pointer to its related Thing, if any */
         };

         /*! The World is equally subdivided in a grid of squares for collision
          * detection. This subclass represents a single square in this grid,
          * containing all static objects that are currently on it.
          * \note An object could be on more than a single square at the same
          *       time. */
         class Square
         {
            public:
               /*! Set square bounding box, to early intersect checks. */
               void setBoundingBox(const Ogre::Vector3& min,
                     const Ogre::Vector3& max);
               /*! Add an element to the Square. See Collision::Element 
                * for info */
               void addElement(const Ogre::Vector3& min, 
                     const Ogre::Vector3& max, Thing* thing);

               /*! Remove an element related to a thing from the Square */
               void removeElement(Thing* thing);

               /*! Verify if the bounding box collide with some element within
                * this square.
                * \param actorBox -> translated and rotated bounding box 
                * \return true if collided, false if no collisions got */
               bool hasCollisions(const Ogre::AxisAlignedBox& actorBox);

               /*! Verify if a Ray collide with some element within this square
                * \param ray -> ray to verify
                * \param dist -> distance from ray origin to accept collisions.
                * \return true if collided, false if got no collisions */
               bool hasCollisions(const Ogre::Ray& ray, const float dist);

            private:
               Kobold::List elements; /**< Current CollisionElements */
               Ogre::AxisAlignedBox bounds; /**< Square bounding box */
            
         };

         /*! Common function for addElement */
         static void addElement(const Ogre::Vector3& min, 
               const Ogre::Vector3& max, Thing* thing);

         /*! \return Square relative to [x, z] index. NULL if none. */
         static Square* getSquare(int x, int z);
         
         /*! \return Square realtive to x,z world position. NULL if none. */
         static Square* getRelativeSquare(float x, float z);
         
         /*! Verify if bounding box defined by min,max doesn't collide with 
          * any square element */
         static bool canBeAt(const Ogre::Vector3& min, 
               const Ogre::Vector3& max);


         static Square** grid; /**< The grid matrix */
         static int xSize; /**< Grid X axis size */
         static int zSize; /**< Grid Z axis size */
         static float squareSize; /**< Size of each square side on world */
   };

}

#endif
