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

#include "inventory.h"
#include "game.h"
#include <kosound/sound.h>

#include <assert.h>

using namespace DNT;

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
Inventory::Inventory()
{
   /* Create Slots */
   slots = new ItemSlot*[INVENTORY_PER_CHARACTER];
   for(int i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      slots[i] = new ItemSlot(INVENTORY_SIZE_X, INVENTORY_SIZE_Y);
   }

   /* Create Equipped */
   equippedSlots = new ItemSlot*[INVENTORY_TOTAL_PLACES];
   equippedSlots[INVENTORY_HEAD] = new ItemSlot(2,2);
   equippedSlots[INVENTORY_HEAD]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_LEFT_HAND] = new ItemSlot(2,4);
   equippedSlots[INVENTORY_LEFT_HAND]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_RIGHT_HAND] = new ItemSlot(2,4);
   equippedSlots[INVENTORY_RIGHT_HAND]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_LEFT_FINGER] = new ItemSlot(1,1);
   equippedSlots[INVENTORY_LEFT_FINGER]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_RIGHT_FINGER] = new ItemSlot(1,1);
   equippedSlots[INVENTORY_RIGHT_FINGER]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_NECK] = new ItemSlot(2,1);
   equippedSlots[INVENTORY_NECK]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_FOOT] = new ItemSlot(2,2);
   equippedSlots[INVENTORY_FOOT]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_BODY] = new ItemSlot(3,4);
   equippedSlots[INVENTORY_BODY]->setAsOneItemPerTime();
}

/**************************************************************
 *                           Destructor                       *
 **************************************************************/
Inventory::~Inventory()
{
   int i;

   /* Free Slots */
   for(i =0; i < INVENTORY_PER_CHARACTER;i++)
   {
      delete slots[i];
   }
   delete[] slots;

   for(i = 0; i < INVENTORY_TOTAL_PLACES; i++)
   {
      delete equippedSlots[i];
   }
   delete[] equippedSlots;
}

/**************************************************************
 *                         getTotalItems                      *
 **************************************************************/
int Inventory::getTotalItems()
{
   int total = 0;
   for(int i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      total += slots[i]->getTotalItems();
   }
   return total;
}

/**************************************************************
 *                           addItem                        *
 **************************************************************/
bool Inventory::addItem(Item* item, int x, int y, int curInv)
{
   bool res = false;

   /* Add the itemect */
   res = slots[curInv]->addItem(item, x, y);

   if(res)
   {
      /* Added to Inventory: no more need to render at scene! */
      item->hide();
   }

   return res;
}

/**************************************************************
 *                          equipItem                       *
 **************************************************************/
bool Inventory::equipItem(Item* item, int where)
{
   assert(item != NULL);
   assert((where >= 0) && (where < INVENTORY_TOTAL_PLACES));

   //TODO verify if item Use Type is compatible with other places

   if( (item->getType() == Item::ITEM_TYPE_WEAPON) && 
       ( (where == INVENTORY_LEFT_HAND) ||
         (where == INVENTORY_RIGHT_HAND)))
   {
      if(equippedSlots[where]->addItem(item, 0, 0))
      {
         /* TODO: must put its sceneNode to be at hand. */
         return true;
      }
   }
   return false;
}

/***********************************************************************
 *                              dropItem                             *
 ***********************************************************************/
void Inventory::dropItem(Item* item, int x, int y, int inv, 
      float mapX, float mapZ)
{
   /* Only can drop if it is on the Inventory */
   removeFromInventory(x, y, inv);

   /* Add it to the map */
//TODO: map->getHeight! ModState!
#if 0
   item->getModel3d()->setPosition(mapX, 
         Game::getMap()->getHeight(mapX, mapZ), mapZ);
   item->getModel3d()->show();
   actualMap->insertItem(X, actualMap->getHeight(X,Z), Z, 
         0.0f, 0.0f, 0.0f, item, 0);
   modifState.mapItemAddAction(MODSTATE_ACTION_OBJECT_ADD,
         item->getFilename(), actualMap->getFilename(),
         X, actualMap->getHeight(X,Z), Z);

   /* Save its state (to avoid, for example, ammo reload
    * after dropping-leaving-return to the map) */
   modifState.mapItemAddAction(MODSTATE_ACTION_OBJECT_CHANGE_STATE,
         item->getFilename(), actualMap->getFilename(),
         X, actualMap->getHeight(X,Z), Z, item->getState());

   /* Play Drop Sound */
   Kosound::Sound::addSoundEffect(mapX, 
         actualMap->getHeight(X,Z), mapZ, SOUND_NO_LOOP,
         "sndfx/itemects/drop_item.ogg");
#endif
}

/**************************************************************
 *                           getMoney                         *
 **************************************************************/
int Inventory::getMoneyQuantity()
{
   Money* m = getMoney();
   if(m)
   {
      return m->getQuantity();
   }

   return 0;
}

/**************************************************************
 *                           getMoney                         *
 **************************************************************/
Money* Inventory::getMoney()
{
   Money* m = NULL;

   /* search inventory slots */
   for(int inv = 0; inv < INVENTORY_PER_CHARACTER; inv++)
   {
      m = static_cast<Money*>(slots[inv]->getItemByFilename(DNT_MONEY_OBJECT));
      if(m != NULL)
      {
         /* Found! */
         return m;
      }
   }

   /* Not found */
   return NULL;
}

/**************************************************************
 *                           addMoney                         *
 **************************************************************/
bool Inventory::addMoney(int qty)
{
   /* Search for Money already on the Inventory.
    * If have, just add value, otherwise, create Money itemect */
   Money* m = getMoney();
   if(m)
   {
      /* add quantity and quit */
      m->addQuantity(qty);
      return true;
   }

   /* Couldn't found... must create a Money item and add it to Inventory */
   m = static_cast<Money*>(Game::createObject(DNT_MONEY_OBJECT));
   m->setQuantity(qty);

   /* Try to add it to Inventory */
   if(addItem(m))
   {
      return true;
   }

   /* Couldn't add, must delete the Money created and false! */
   delete m;
   return false;
}

/**************************************************************
 *                           decMoney                         *
 **************************************************************/
bool Inventory::decMoney(int qty)
{
   Money* m;

   /* Search for Money already on the Inventory.
    * If have, just add value, otherwise, create Money itemect */
   m = getMoney();
   if(m)
   {
      /* add quantity and quit */
      if(m->removeQuantity(qty))
      {
         if(m->getQuantity() == 0)
         {
            /* Must delete Money! */
            removeFromInventory(m);
            delete m;
         }
         /* subtracted! */
         return true;
      }
   }

   return false;
}


/**************************************************************
 *                           addItem                        *
 **************************************************************/
bool Inventory::addItem(Item* item)
{
   int inv = 0;

   inv = 0;
   while( (!slots[inv]->addItem(item) && (inv < INVENTORY_PER_CHARACTER)))
   {
      inv++;
   }

   if(inv < INVENTORY_PER_CHARACTER)
   {
      /* Added to inventory, must hide its 3d model. */
      item->hide(); 
   }

   return (inv < INVENTORY_PER_CHARACTER);
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool Inventory::canAdd(Item* item, int x, int y, int curInv)
{
   return slots[curInv]->canAdd(item, x, y);
}

/**************************************************************
 *                         getFromPlace                       *
 **************************************************************/
Item* Inventory::getFromPlace(int where)
{
   assert((where >= 0) && (where < INVENTORY_TOTAL_PLACES));

   return equippedSlots[where]->getFromPosition(0, 0);
}

/**************************************************************
 *                       getFromPosition                      *
 **************************************************************/
Item* Inventory::getFromPosition(int x, int y, int curInv)
{
   assert((x >= 0) && (y >=0) && (x < INVENTORY_SIZE_X) &&
         (y < INVENTORY_SIZE_Y));
   
   return slots[curInv]->getFromPosition(x,y);
}

/**************************************************************
 *                        getFirstItem                      *
 **************************************************************/
Item* Inventory::getFirstItem(int& x, int& y, int curInv)
{
   return slots[curInv]->getFirstItem(x,y);
}

/**************************************************************
 *                         getNextItem                      *
 **************************************************************/
Item* Inventory::getNextItem(int& x, int& y, int curInv)
{
   return slots[curInv]->getNextItem(x,y);
}

/**************************************************************
 *                       removeFromPlace                      *
 **************************************************************/
void Inventory::removeFromPlace(int where)
{
   assert((where >= 0) && (where < INVENTORY_TOTAL_PLACES));
   
   equippedSlots[where]->removeItem(0, 0);
}

/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void Inventory::removeFromInventory(Item* item)
{
   /* Remove the first item equal to this one */
   for(int i=0; i < INVENTORY_PER_CHARACTER; i++)
   {
      if(slots[i]->removeItem(item))
      {
         return;
      }
   }
}

/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void Inventory::removeFromInventory(int x, int y, int curInv)
{
   slots[curInv]->removeItem(x,y);
}

/**************************************************************
 *                     getItemByFilename                      *
 **************************************************************/
Item* Inventory::getItemByFilename(Kobold::String filename)
{
   Item* res = NULL;

   /* First, search at the normal slots */
   for(int i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      res = slots[i]->getItemByFilename(filename);
      if(res != NULL)
      {
         return res;
      }
   }

   /* Next search at the equipped ones */
   for(int i = 0; i < INVENTORY_TOTAL_PLACES; i++)
   {
      res = equippedSlots[i]->getItemByFilename(filename);
      if(res != NULL)
      {
         return res;
      }
   }

   return NULL;
}

/**************************************************************
 *                       getItemByInfo                        *
 **************************************************************/
Item* Inventory::getItemByInfo(Kobold::String relatedInfo)
{
   Item* res = NULL;

   /* First, search at the normal slots */
   for(int i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      res = slots[i]->getItemByInfo(relatedInfo);
      if(res != NULL)
      {
         return res;
      }
   }

   /* Next search at the equipped ones */
   for(int i = 0; i < INVENTORY_TOTAL_PLACES; i++)
   {
      res = equippedSlots[i]->getItemByInfo(relatedInfo);
      if(res != NULL)
      {
         return res;
      }
   }
   
   return NULL;
}

/**************************************************************
 *                      countItemByInfo                       *
 **************************************************************/
int Inventory::countItemByInfo(Kobold::String relatedInfo)
{
   int res = 0;

   /* First, search at the normal slots */
   for(int i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      res += slots[i]->countItemByInfo(relatedInfo);
   }

   /* Next search at the equipped ones */
   for(int i = 0; i < INVENTORY_TOTAL_PLACES; i++)
   {
      res += equippedSlots[i]->countItemByInfo(relatedInfo);
   }
   
   return res;
}

