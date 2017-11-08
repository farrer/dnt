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
#include <kobold/readwritelock.h>
#include <utility>

namespace DNT
{

   /*! Implements an element that should have collision enabled. */
   class Element : public Kobold::ListElement
   {
      public:
         /*! Constructor
          * \param square square occupied by the box 
          * \param min minimum corner of the box
          * \param max maximum corner of the box 
          * \param thing pointer to the related thing, if any. */
         Element(Square* square, const Ogre::Vector3& min, 
               const Ogre::Vector3& max, Thing* thing);

         /*! \return if the actor can be over the element (walk on it). */
         bool isWalkable(Thing* actor);

         /*! Check, triangle by triangle, if the element's Thing model, 
          * at its current position and orientation collides with the
          * defined actor's bounding box.
          * \param actorBox Bounding box to check. */
         bool depthCollide(const Ogre::AxisAlignedBox& actorBox); 

         Ogre::AxisAlignedBox bounds; /**< Current element bounds */
         Square* square; /**< Square who owns the element */
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
         /*! Constructor */
         Square();
         /*! Destructor */
         ~Square();

         /*! Set square bounding box, to early intersect checks. */
         void setBoundingBox(const Ogre::Vector3& min,
               const Ogre::Vector3& max);
         /*! Add an element to the Square. See Collision::Element 
          * for info */
         void addElement(const Ogre::Vector3& min, 
               const Ogre::Vector3& max, Thing* thing);

         /*! Verify if the bounding box collide with some element within
          * this square.
          * \param actorBox -> translated and rotated bounding box 
          * \param actor -> pointer to the actor, if any (will ignore
          *                 collisions with itself)
          * \param newHeight -> will receive the new height for the actor be 
          *                     at the desired position.
          * \param ignore -> pointer to a thing to ignore collisions to
          * \return pair, first true if collided, false if no 
          * collisions got, second, pointer to the collider element */
         std::pair<bool, Element*> hasCollisions(
               const Ogre::AxisAlignedBox& actorBox,
               Thing* actor, float& newHeight, Thing* ignore = NULL);

         /*! Verify if a Ray collide with some element within this square,
          * always returning the nearest collider got.
          * \param ray -> ray to verify
          * \param dist -> distance from ray origin to accept collisions.
          * \param collidedDist -> will receive the distance along ray
          *               where the collision happened.
          * \param actor -> pointer to the actor, if any (will ignore
          *                 collisions with itself)
          * \param ignore -> pointer to a thing to ignore collisions to
          * \return pair, first true if collided, false if no 
          * collisions got, second, pointer to the nearest collider 
          * element */
         std::pair<bool, Element*> hasCollisions(
               const Ogre::Ray& ray, const float dist,
               float& collidedDist,
               Thing* actor = NULL, Thing* ignore = NULL);
         /*! Verify if a Ray collide with some element within this square.
          * \param ray -> ray to verify
          * \param dist -> distance from ray origin to accept collisions.
          * \param actor -> pointer to the actor, if any (will ignore
          *                 collisions with itself)
          * \param ignore -> pointer to a thing to ignore collisions to
          * \return true if collided */
         bool hasCollisions(const Ogre::Ray& ray, const float dist,
               Thing* actor = NULL, Thing* ignore = NULL);

         /*! Remove a collision element from the square */
         void remove(Element* element);

         /*! Set if the square is walkable or not (default: false).
          * A map is defined as a grid of nXm squares, but some could
          * be unused, allowing maps of distinct shapes. This is useful
          * for indoor maps, like houses do not look always as rectangles. 
          * \param value if walkable or not */
         void setWalkable(bool value);

         /*! \return if the square is walkable or not */
         const bool getWalkable() const { return walkable; };

      private:
         Kobold::List elements; /**< Current CollisionElements */
         Ogre::AxisAlignedBox bounds; /**< Square bounding box */
         bool walkable; /**< If could walk on this square or not */

   };



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
          * \param newHeight -> will receive the new height for the actor be 
          *                     at the desired position.
          * \param aStarCall -> if was called by A* search or not.
          * \return true if can move, false if will collide with something
          *         else and can't */
         static bool canMove(Thing* actor, const Ogre::Vector3& varPos,
               const float varOri, float& newHeight, bool aStarCall);

         /*! Check if a Thing can move from a defined position to another
          * one. This check will be made by using rays from and to 
          * origin/destiny, and usually are made for A* in-between targets 
          * checks (for the targets themselves, a canOccupy check is made) 
          * \param actor Thing to check its movement
          * \param origin origin of the movement
          * \param destiny destiny of the movement 
          * \param aStarCall -> if was called by A* search or not.
          * \return true if can move, false if can't. */
         static bool canMove(Thing* actor, const Ogre::Vector3& origin,
               const Ogre::Vector3& destiny, bool aStarCall);

         /*! Check if a thing can occupy an specific position on the map.
          * \param actor to check if can occupy
          * \param pos position to check if can occupy
          * \param newHeight -> will receive the new height for the actor be 
          *                     at the desired position.
          * \return true if can, false otherwise. */
         static bool canOccupy(Thing* actor, const Ogre::Vector3& pos, 
               float& newHeight);

         /*! Check if a Thing is at sight of another one.
          * \param actor to check if target is at its sight
          * \param target to check if is at actor's sight
          * \param reverseCheck if should check the reverse too (used for
          *         recursion control).
          * \return true if target is at actor sight, and false if something
          *         is between them. */
         static bool isAtSight(Thing* actor, Thing* target, 
               bool reverseCheck=true);

         /*! Add an element, without a related Thing.
          * \param min minumum corner values
          * \param max maximum corner values */
         static void addElement(const Ogre::Vector3& min, 
               const Ogre::Vector3& max);

         /*! Add an element with a related Thing */ 
         static void addElement(Thing* thing);

         /*! Remove all elements related to a Thing from the collisions */
         static void removeElement(Thing* thing);

         /*! Set if a collision square is walkable or not. If not, it will
          * simplify the collision check, as won't need to verify with each
          * of its elements 
          * \param x X coordinate
          * \param z Z coordinate
          * \param value if walkable or not */
         static void setSquareWalkable(int x, int z, bool value);

      private:

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
               const Ogre::Vector3& max, Thing* actor, float& newHeight);

         /*! Internal at sight check */
         static bool isAtSight(Thing* actor, Thing* target, 
               const Ogre::Ray& ray, const float& dist, 
               int initSqX, int initSqZ, int endSqX, int endSqZ);

         static Square** grid; /**< The grid matrix */
         static int xSize; /**< Grid X axis size */
         static int zSize; /**< Grid Z axis size */
         static float squareSize; /**< Size of each square side on world */
         /*! Read-Write-Lock for collision structures access */
         static Kobold::ReadWriteLock* rwLock; 

   };

}

#endif

