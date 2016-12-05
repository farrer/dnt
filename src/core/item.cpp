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

#include "item.h"
#include <kobold/log.h>
using namespace DNT;

#define ITEM_KEY_INVENTORY_SIZE   "inventory_sizes"
#define ITEM_KEY_RELATED_INFO     "related_info"
#define ITEM_KEY_TYPE     "type"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Item::Item()
{
   type = ITEM_TYPE_GENERIC;
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Item::~Item()
{
}

/**************************************************************************
 *                           getInventorySize                             *
 **************************************************************************/
Ogre::Vector2 Item::getInventorySize()
{
   return inventorySize;
}

/**************************************************************************
 *                            getRelatedInfo                              *
 **************************************************************************/
Kobold::String Item::getRelatedInfo()
{
   return relatedInfo;
}

/**************************************************************************
 *                               getType                                  *
 **************************************************************************/
Item::ItemType Item::getType()
{
   return type;
}

/**************************************************************************
 *                              isUsable                                  *
 **************************************************************************/
bool Item::isUsable()
{
   return( (type == ITEM_TYPE_WEAPON) ||
           (type == ITEM_TYPE_ARMOR) ||
           (type == ITEM_TYPE_HEAL) ||
           (type == ITEM_TYPE_EXPLOSIVE) ||
           (type == ITEM_TYPE_BOOK) ||
           (type == ITEM_TYPE_AMMO) ||
           (type == ITEM_TYPE_NARCOTIC));
}

/**************************************************************************
 *                          setTypeByString                               *
 **************************************************************************/
void Item::setTypeByString(Kobold::String typeStr)
{
   for(int i = 0; i < ITEM_TOTAL_TYPES; i++)
   {
      if(typeStrings[i] == typeStr)
      {
         type = (ItemType) i;
         return;
      }
   }

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Warning: unknown item type '%s' was ignored", typeStr.c_str());
   type = ITEM_TYPE_GENERIC;
}

/**************************************************************************
 *                          getTypeAsString                               *
 **************************************************************************/
Kobold::String Item::getTypeAsString()
{
   return typeStrings[type];
}

/**************************************************************************
 *                     doObjectSpecializationParse                        *
 **************************************************************************/
bool Item::doObjectSpecializationParse(Ogre::String key, Ogre::String value)
{
   if(key == ITEM_KEY_INVENTORY_SIZE)
   {
      int sX=0, sY=0;
      sscanf(value.c_str(), "%d %d", &sX, &sY);
      inventorySize = Ogre::Vector2(sX, sY);
   }
   else if(key == ITEM_KEY_RELATED_INFO)
   {
      relatedInfo = value;
   }
   else if(key == ITEM_KEY_TYPE)
   {
      setTypeByString(value);
   }
   else
   {
      return false;
   }
   
   return true;
}

/**************************************************************************
 *                     doObjectSpecializationSave                         *
 **************************************************************************/
bool Item::doObjectSpecializationSave(std::ofstream& file)
{
   file << ITEM_KEY_INVENTORY_SIZE << " = " << ((int)inventorySize.x)
        << " " << ((int)inventorySize.y) << std::endl;
   file << ITEM_KEY_RELATED_INFO << " = " << relatedInfo << std::endl;
   file << ITEM_KEY_TYPE << " = " << getTypeAsString() << std::endl;
 
   return true;
}

/**************************************************************************
 *                           Static Members                               *
 **************************************************************************/
Kobold::String Item::typeStrings[ITEM_TOTAL_TYPES] = {
         "generic", "weapon", "ammo", "armor", "heal", "explosive", 
         "narcotic", "book", "money"}; 

