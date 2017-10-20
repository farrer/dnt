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
#include "money.h"

#include "../rules/character.h"
#include "inventory.h"

#include "modstate.h"
#include "game.h"

#include "../map/map.h"

#include "../gui/briefing.h"

#include <kobold/log.h>
#include <kosound/sound.h>

using namespace DNT;

#define ITEM_KEY_INVENTORY_SIZE   "inventory_sizes"
#define ITEM_KEY_RELATED_INFO     "related_info"
#define ITEM_KEY_TYPE             "type"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Item::Item()
     :Object(Thing::THING_TYPE_ITEM)
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
const Kobold::String& Item::getRelatedInfo() const
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
void Item::setTypeByString(const Kobold::String& typeStr)
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
const Kobold::String& Item::getTypeAsString() const
{
   return typeStrings[type];
}

/**************************************************************************
 *                     doObjectSpecializationParse                        *
 **************************************************************************/
bool Item::doObjectSpecializationParse(const Kobold::String& key, 
      const Kobold::String& value)
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
 *                                interact                                *
 **************************************************************************/
Object::InteractResult Item::interact(Character* actor)
{
   /* Pick it. */
   bool picked = false;

   if(getType() == ITEM_TYPE_MONEY)
   {
      /* Just add the money to character's */
      Money* m = static_cast<Money*>(this);
      picked = actor->getInventory()->addMoney(m->getQuantity());
   }
   else
   {
      /* Add the item to inventory. */
      picked = actor->getInventory()->addItem(this);
   }

   if(picked)
   {
      /* Play 'pick' sound */
      Ogre::Vector3 pos = getPosition();
      Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z,
            SOUND_NO_LOOP, "sndfx/objects/take_item.ogg");
      /* Log text on briefing */
      Briefing::addTextWithBreak(gettext("%s taken."), getName().c_str());

      /* Tell ModState */
      ModState::addMapObjectAction(MODSTATE_ACTION_OBJECT_REMOVE,
            getFilename(), Game::getCurrentMap()->getFilename(), pos);

      //TODO: remove item from map!
      //Game::getCurrentMap()::removeObject(this);


      if(getType() == ITEM_TYPE_MONEY)
      {
         /* We should also delete money object, as no more needed */
         //TODO: mark it for deletion on next cycle!
      }
   }
   else
   {
      Briefing::addWarning(gettext("Inventory is full!"));
   }

   return Object::INTERACTION_DONE;
}

/**************************************************************************
 *                           Static Members                               *
 **************************************************************************/
Kobold::String Item::typeStrings[ITEM_TOTAL_TYPES] = {
         "generic", "weapon", "ammo", "armor", "heal", "explosive", 
         "narcotic", "book", "money"}; 

