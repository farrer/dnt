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
void Square::setBoundingBox(const Ogre::Vector3& min,
      const Ogre::Vector3& max)
{
   this->bounds = Ogre::AxisAlignedBox(min, max);
}

/***********************************************************************
 *                             addElement                              *
 ***********************************************************************/
void Square::addElement(const Ogre::Vector3& min, 
      const Ogre::Vector3& max, Thing* thing)
{
   Element* el = new Element(this, min, max, thing);
   elements.insert(el);
   if(thing)
   {
      Thing::ColElement* colEl = new Thing::ColElement();
      colEl->element = el;
      thing->getColElements()->insert(colEl);
   }
}

/***********************************************************************
 *                          hasCollisions                              *
 ***********************************************************************/
std::pair<bool, Element*> Square::hasCollisions(
      const Ogre::AxisAlignedBox& actorBox, Thing* actor, 
      float& newHeight, Thing* ignore)
{
   /* Check if have some intersection with the square */
   if(!actorBox.intersects(bounds))
   {
      return std::pair<bool, Element*>(false, NULL);
   }

   /* Check each element for collision */
   Element* el = static_cast<Element*>(
         elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(((actor == NULL) || (el->thing != actor)) &&
         ((ignore == NULL) || (el->thing != ignore)))
      {
         /* Only check if not the actor itself or a ignore (target) thing */
         if(actorBox.intersects(el->bounds))
         {
            /* When verifying if can walk, sometimes the element could be
             * 'walkable', for example, when its not so taller (a wood plank 
             * on the floor, a rug, etc). */
            if(el->isWalkable(actor))
            {
               /* Note: always using the higher delta */
               newHeight = (el->bounds.getMaximum().y > newHeight) ? 
                  el->bounds.getMaximum().y : newHeight;
            }
            else
            {
               if((el->thing != NULL) && 
                  (el->thing->shouldDepthCollisionCheck()))
               {
                  /* Only collided if depth collided */
                  if(el->depthCollide(actorBox))
                  {
                     return std::pair<bool, Element*>(true, el);
                  }
               }
               else
               {
                  /* Collided */
                  return std::pair<bool, Element*>(true, el);
               }
            }
         }
      }
      el = static_cast<Element*>(el->getNext());
   }

   /* No collisions  */
   return std::pair<bool, Element*>(false, NULL);
}

/***********************************************************************
 *                          hasCollisions                              *
 ***********************************************************************/
std::pair<bool, Element*> Square::hasCollisions(
      const Ogre::Ray& ray, const float dist, float& collidedDist, 
      Thing* actor, Thing* ignore)
{
   std::pair<bool, Element*> got(false, NULL);
   std::pair<bool, Ogre::Real> res;
   /* Check collision with the square bounds */
   res = ray.intersects(bounds);
   if(!res.first)
   {
      /* No collisions within the square, should not check inner 
       * elements. */
      return got;
   }

   /* Note; we must check all, as collidedDist is relevant. */
   Element* el = static_cast<Element*>(
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
               got = std::pair<bool, Element*>(true, el);
            }
         }
      }
      el = static_cast<Element*>(el->getNext());
   }

   return got;
}

/***********************************************************************
 *                             remove                                  *
 ***********************************************************************/
void Square::remove(Element* element)
{
   assert(element != NULL);
   assert(element->square == this);

   elements.remove(element);
}


/***********************************************************************
 *                          hasCollisions                              *
 ***********************************************************************/
bool Square::hasCollisions(const Ogre::Ray& ray, const float dist,
      Thing* actor, Thing* ignore)
{
   std::pair<bool, Ogre::Real> res;
   
   /* Check collision with the square bounds */
   res = ray.intersects(bounds);
   if(!res.first)
   {
      return false;
   }

   Element* el = static_cast<Element*>(
         elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(((actor == NULL) || (el->thing != actor)) &&
         ((ignore == NULL) || (el->thing != ignore)))
      {
         res = ray.intersects(el->bounds);
         if((res.first) && (res.second <= dist))
         {
            /* Collided */
            return true;
         }
      }
      el = static_cast<Element*>(el->getNext());
   }

   return false;
}

