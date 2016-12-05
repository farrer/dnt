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
using namespace DNT;

#define ITEM_KEY_INVENTORY_SIZE   "inventory_sizes"
#define ITEM_KEY_RELATED_INFO     "related_info"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Item::Item()
{
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
 
   return true;
}
      
