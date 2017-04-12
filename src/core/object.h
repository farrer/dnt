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

#ifndef _dnt_object_h
#define _dnt_object_h

#include "dntconfig.h"
#include "../rules/thing.h"

namespace DNT
{

/*! The Object is a non living Thing. It's the base for interactive items,
 * static sceneries, weapons and any other non-living thing. */
class Object : public Thing
{
   public:
      /*! Constructor 
       * \param thingType type of the object */
      Object(ThingType type);
      /*! Destructor */
      virtual ~Object();

      /*! \return Surface that represents the Object in 2D.
       * \note could be NULL. */
      Farso::Surface* getImage();

      /*! \ return current object's cost */
      int getCost();
      /*! Set current object's cost 
       * \param value new cost value */
      void setCost(int value);

      /*! \return current object's weight */
      float getWeight();
      /*! Set object's weight
       * \param value new object's weight value */
      void setWeight(float value);

   protected:

      /*! Parse common Object key/values pairs. */
      bool doSpecificParse(Ogre::String key, Ogre::String value);
      /*! Save common Object key/values pairs */
      bool doSpecificSave(std::ofstream& file);

      /*! Must implement the parse of key/values related to the specialized
       * class (not threated by the Object itself). */
      virtual bool doObjectSpecializationParse(Ogre::String key, 
            Ogre::String value) = 0;

      /*! Must implement the save of key/values specific related to the
       * specialized class  */
      virtual bool doObjectSpecializationSave(std::ofstream& file) = 0;

   private:
      int cost;      /**< Current object's cost */
      float weight;  /**< Current object's weight */
      Farso::Surface* image; /**< Image representing the object in 2d */

};

}

#endif

