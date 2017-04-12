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
 *                            Constructor                              *
 ***********************************************************************/
void Collision::Square::setBoundingBox(const Ogre::Vector3& min,
      const Ogre::Vector3& max)
{
   this->bounds = Ogre::AxisAlignedBox(min, max);
}

/***********************************************************************
 *                             addElement                              *
 ***********************************************************************/
void Collision::Square::addElement(const Ogre::Vector3& min, 
      const Ogre::Vector3& max, Thing* thing)
{
   elements.insert(new Collision::Element(min, max, thing));
}

/***********************************************************************
 *                           removeElement                             *
 ***********************************************************************/
void Collision::Square::removeElement(Thing* thing)
{
   Collision::Element* el = static_cast<Collision::Element*>(
         elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(el->thing == thing)
      {
         /* Found the element owner, must remove it and done. */
         elements.remove(el);
         break;
      }
      el = static_cast<Collision::Element*>(el->getNext());
   }
}

/***********************************************************************
 *                          hasCollisions                              *
 ***********************************************************************/
std::pair<bool, Collision::Element*> Collision::Square::hasCollisions(
      const Ogre::AxisAlignedBox& actorBox, Thing* actor, Thing* ignore)
{
   /* Check if have some intersection with the square */
   if(!actorBox.intersects(bounds))
   {
      return std::pair<bool, Collision::Element*>(false, NULL);
   }

   /* Check each element for collision */
   Collision::Element* el = static_cast<Collision::Element*>(
         elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(((actor == NULL) || (el->thing != actor)) &&
         ((ignore == NULL) || (el->thing != ignore)))
      {
         /* Only check if not the actor itself or a ignore (target) thing */
         if(actorBox.intersects(el->bounds))
         {
            //TODO: when verifying if can walk, sometimes the element could be
            //'walkable', for example, when its not so taller (a wood plank 
            //on the floor, for example). 
            //TODO: depth collision check, if desired. 
            /* Collided */
            return std::pair<bool, Collision::Element*>(true, el);
         }
      }
      el = static_cast<Collision::Element*>(el->getNext());
   }

   /* No collisions  */
   return std::pair<bool, Collision::Element*>(false, NULL);
}

/***********************************************************************
 *                          hasCollisions                              *
 ***********************************************************************/
std::pair<bool, Collision::Element*> Collision::Square::hasCollisions(
      const Ogre::Ray& ray, const float dist, float& collidedDist, 
      Thing* actor, Thing* ignore)
{
   //FIXME: for A* we should call another version of this function,
   //where collided dist isn't important (and where we could stop checking
   //as soon as finding a collision).
   std::pair<bool, Collision::Element*> got(false, NULL);
   std::pair<bool, Ogre::Real> res;
   /* Check collision with the square bounds */
   res = ray.intersects(bounds);
   if(!res.first)
   {
      /* No collisions within the square, should not check inner 
       * elements. */
      return got;
   }

   /* Note; we must check if all, as collidedDist is relevant. */
   Collision::Element* el = static_cast<Collision::Element*>(
         elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(((actor == NULL) || (el->thing != actor)) &&
         ((ignore == NULL) || (el->thing != ignore)))
      {
         res = ray.intersects(el->bounds);
         if((res.first) && (res.second <= dist))
         {
            //TODO: depth collision check, if desired. 
            /* Collided */
            if(res.second <= collidedDist)
            {
               collidedDist = res.second;
               got = std::pair<bool, Collision::Element*>(true, el);
            }
         }
      }
      el = static_cast<Collision::Element*>(el->getNext());
   }

   return got;
}

