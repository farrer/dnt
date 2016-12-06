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

#ifndef _dnt_item_h
#define _dnt_item_h

#include "dntconfig.h"
#include "object.h"

namespace DNT
{

/*! The Item is an interactive object. Could be pickable or not by 
 * PlayableCharacters. */
class Item : public Object
{
   public:
      /*! Types of Items */
      enum ItemType
      {
         ITEM_TYPE_GENERIC = 0,
         ITEM_TYPE_WEAPON,
         ITEM_TYPE_AMMO,
         ITEM_TYPE_ARMOR,
         ITEM_TYPE_HEAL,
         ITEM_TYPE_EXPLOSIVE,
         ITEM_TYPE_NARCOTIC,
         ITEM_TYPE_BOOK,
         ITEM_TYPE_MONEY,
         ITEM_TOTAL_TYPES
      };

      /*! Constructor */
      Item();
      /*! Destructor */
      ~Item();

      /*! \return Item's size on inventory */
      Ogre::Vector2 getInventorySize();

      /*! \return related information about the Item */
      Kobold::String getRelatedInfo();

      /*! \return item type */
      ItemType getType();

      /*! \return if the item is usable or not (based on its type). */
      bool isUsable();

      /* \return true. Itens are interactive. */
      bool canInteract();

      /* Interact with the item: pick it. */
      Object::InteractResult interact(Character* actor);

   protected:
     
      /*! Parse specific Item definitions from file */
      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);
      /*! Add specific Item definitions to file */
      bool doObjectSpecializationSave(std::ofstream& file);
      
   private:

      /*! Set current  ItemType based on a string */
      void setTypeByString(Kobold::String typeStr);
      /*! \return String representation of current item type */
      Kobold::String getTypeAsString();

      Ogre::Vector2 inventorySize; /**< Total size needed when on inventory */
      Kobold::String relatedInfo; /**< Related Item information */
      ItemType type; /**< Type of the item */

      static Kobold::String typeStrings[ITEM_TOTAL_TYPES];
};

}

#endif

