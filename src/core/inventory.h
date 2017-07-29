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

#ifndef _dnt_inventory_h
#define _dnt_inventory_h

#include "dntconfig.h"

#include "itemslot.h"
#include "money.h"

namespace DNT
{

#define INVENTORY_SIZE_X 13 /**< Number of Spaces on X axis */
#define INVENTORY_SIZE_Y 6  /**< Number of Spaces on Y axis */

#define TRADE_SIZE_X     6  /**< Number of Spaces on X axis for Trades */
#define TRADE_SIZE_Y    10  /**< Number of Spaces on Y axis for Trades */

//#define INVENTORY_INVENTORY     8  /**< is in Inventory */

#define INVENTORY_PER_CHARACTER 4  /**< Number of Inventories per Character */

/*! Character Inventory: a place where a Character keep all its items and 
 * equipments. */
class Inventory
{
   public:
      /*! Possible places at the Inventory */
      enum CharacterPlaces
      {
         INVENTORY_HEAD = 0,     /**< Head Place */
         INVENTORY_LEFT_HAND,    /**< Left Hand Place */
         INVENTORY_RIGHT_HAND,   /**< Right Hand Place */
         INVENTORY_LEFT_FINGER,  /**< Left Finger Place */
         INVENTORY_RIGHT_FINGER, /**< Right Finger Place */
         INVENTORY_NECK,         /**< Neck Place */
         INVENTORY_FOOT,         /**< Foot Place */
         INVENTORY_BODY,         /**< Body Place (Torso) */
         INVENTORY_TOTAL_PLACES  /**< Total Places */
      };


      /*! Constructor */
      Inventory();
      /*! Destructor */
      ~Inventory();

      /*! Add Money to the Inventory
       * \param qty -> money quantity (value) 
       * \return true if could add, false otherwise */
      bool addMoney(int qty);

      /*! Get money quantity */
      int getMoneyQuantity();

      /*! Get money itemect at Inventory, if any */
      Money* getMoney();

      /*! Dec money value from current money itemect
       * \note -> if remaining quantity is == 0, itemect is deleted. 
       * \return true if could subtract, false if quantity insuficient */
      bool decMoney(int qty);

      /*! Add itemect to Inventory space
       * \param item -> pointer to itemect to be added
       * \param x -> x space position to be the orign on Inventory
       * \param y -> y space position to be the orign on Inventory
       * \param curInv -> current Inventory
       * \return -> true if added to Inventory, false if can't add. */
      bool addItem(Item* item, int x, int y, int curInv);

      /*! Add itemect to first free Inventory space
       * \param item -> pointer to itemect to be added
       * \return -> true if added to Inventory, false if can't add. */
      bool addItem(Item* item);

      /*! Equip Item to the Place defined.
       * \param item -> pointer to itemect to be equipped
       * \param where -> ID of the place to equip
       * \return -> true if equipped, false, if can't equip */
      bool equipItem(Item* item, int where);

      /*! Drop itemect from the Inventory (puting it on the floor X,Z)
       * \param item -> itemect to drop from the Inventory
       * \param x -> x position of the itemect on Inventory
       * \param y -> y position of the itemect at Inventory
       * \param inv -> Inventory where itemect lies
       * \param mapX -> x position to drop itemect to
       * \param mapZ -> z position to drop itemect to */
      void dropItem(Item* item, int x, int y, int inv, float mapX, float mapZ);

      /*! Get itemect from iventory position
       * \param x -> x space position on Inventory
       * \param y -> y space position on Inventory
       * \param curInv -> current Inventory
       * \return -> pointer to itemect on position */
      Item* getFromPosition(int x, int y, int curInv);

      /*! Get itemect from Inventory place
       * \param where -> place ID on Inventory
       * \return -> pointer to itemect on position */
      Item* getFromPlace(int where);

      /*! Get the first itemect founded on the Inventory. 
       * \param x -> x position of the item got
       * \param y -> y position of the item got
       * \param curInv -> Inventory number to get itemect from
       * \return -> pointer to the first founded item, or NULL. */
      Item* getFirstItem(int& x, int& y, int curInv);

      /*! Get next itemect on Inventory.
       * \note -> this function is usually called after a getFirstItem,
       *          with the result x,y coordinate got from that 
       * \param x -> x coordinate of the previous item got (will have 
       *             the x coordinate of the current after the function)
       * \param y -> y coordinate of the previos item got (will have the
       *             y coordinate of the current after the function)
       * \param curInv -> Inventory number to get itemect from
       * \return -> pointer to the itemect found or NULL, if no more itemects */
      Item* getNextItem(int& x, int& y, int curInv);

      /*! Remove itemect from Inventory
       * \param item -> pointer to itemect to be removed */
      void removeFromInventory(Item* item);

      /*! Remove itemect from Inventory
       * \param x -> x space position on Inventory
       * \param y -> y space position on Inventory 
       * \param curInv -> current Inventory */
      void removeFromInventory(int x, int y, int curInv);

      /*! Remove Item from Equipped Place
       * \param where -> equipped place ID */
      void removeFromPlace(int where);

      /*! Verify if can Add itemect to Inventory position
       * \param item -> pointer to itemect to be added
       * \param x -> x Inventory position
       * \param y -> y Inventory position
       * \param curInv -> current Inventory
       * \return -> true if can add, false otherwise */
      bool canAdd(Item* item, int x, int y, int curInv);

      /*! Get a item from the Inventory with the desired filename, if exists.
       * \param filename -> filename of the item
       * \return -> pointer to the found itemect of NULL */
      Item* getItemByFilename(Kobold::String filename);

      /*! Get a item from the Inventory with the desired relatedInfo, if exists.
       * \param relatedInfo -> info of the item
       * \return -> pointer to the found itemect of NULL */
      Item* getItemByInfo(Kobold::String relatedInfo);

      /*! Count the items with relatedInfo
       * \param relatedInfo -> info of the item to count
       * \return -> number of items with relatedInfo */
      int countItemByInfo(Kobold::String relatedInfo);

      /*! Get total number of items at the Inventory
       * \return -> total number of items */
      int getTotalItems();

   private:
      ItemSlot** slots;            /**< The Inventory */
      ItemSlot** equippedSlots;    /**< The Equipped Slots */

};

}

#endif

