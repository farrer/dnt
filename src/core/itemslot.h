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

#ifndef _dnt_itemslot_h
#define _dnt_itemslot_h

#include "item.h"

namespace DNT
{

/*! ItemSlot Class. A ItemSlot is just some Spaces to have itens into, to be
 * carried by a Thing or traded with them. */
class ItemSlot
{
   public:

      /*! Definition of each Slot Space */
      class Space
      {
         public:
            /*! Constructor */
            Space();
            /*! Destructor */
            ~Space();

            /*! Set item at the space
             * \param item item's pointer
             * \param orig its origin coordinate on ItemSlot */
            void setItem(Item* item, Ogre::Vector2 orig);
            /*! Get current item at the Space
             * \return pointer to current Item or NULL if none */
            Item* getItem();
            /*! \return current item origin at ItemSlot. Only valid if have
             * an Item at the space actually. */
            Ogre::Vector2 getOrigin();

         private:

            Item* item;    /**< Current item in the Space */
            Ogre::Vector2 origin; /**< Current origin on ItemSlot */
      };


      /*! Constructor
       * \param xSize -> x size of the slot
       * \param ySize -> y size of the slot */
      ItemSlot(int xSize, int ySize);
      /*! Destruc */
      ~ItemSlot();

      /*! Add, if can, the itemect to the ItemSlot at position x,y.
       * \param item -> pointer to the itemect to add
       * \param x -> x position on ItemSlot
       * \param y -> y position on ItemSlot
       * \return -> true if added, false otherwise. */
      bool addItem(Item* item, int x, int y);
      /*! Add the item to the first avaible slot
       * \param item -> pointer to itemect to add
       * \return -> true if addes, false otherwise. */
      bool addItem(Item* item);
      /*! Verify if can add the item to the position x,y
       * \param item -> item to verify
       * \param x -> x position on slot
       * \param y -> y position on slot 
       * \return true if can add, false otherwise. */
      bool canAdd(Item* item, int x, int y);

      /*! Get itemect that is on the position x,y. The function will not remove
       * it from the position. 
       * \param x -> x slot coordinate
       * \param y -> y slot coordinate 
       * \return -> pointer to the item, or NULL. */
      Item* getFromPosition(int x, int y);

      /*! Get the first item founded on the slot. Usefull when removing all
       * itens from the slot, like case of death or cancelled barter.
       * \param x -> x position of the item got
       * \param y -> y position of the item got
       * \return -> pointer to the first founded item, or NULL. */
      Item* getFirstItem(int& x, int& y);

      /*! Get next item on the ItemSlot.
       * \note -> this function is usually called after a getFirstItem,
       *          with the result x,y coordinate got from that 
       * \param x -> x coordinate of the previous item got (will have 
       *             the x coordinate of the current after the function)
       * \param y -> y coordinate of the previos item got (will have the
       *             y coordinate of the current after the function)
       * \return -> pointer to the itemect found or NULL, if no more itemects */
      Item* getNextItem(int& x, int& y);

      /*! Remove All references to the itemect from the ItemSlot
       * \param item -> pointer to itemect to remove
       * \return -> true if can remove (itemect existed there) 
       * \note this function won't delete the Item. The caller is responsible 
       *       for that Item pointer. */
      bool removeItem(Item* item);
      /*! Remove Item from the ItemSlot position x,y
       * \param x -> x ItemSlot position
       * \param y -> y ItemSlot position 
       * \note this function won't delete the Item. The caller is responsible 
       *       for that Item pointer. */
      void removeItem(int x, int y);

      /*! Set the slot to only accepts one item on it per time. Usefull for
       *  equipped itemects slots. 
       *  \note when setting it, even if the item occupy less than total 
       *        ItemSlot Spaces, it won't allow any other item to be there. */
      void setAsOneItemPerTime();

      /*! Get a item from the slots with the desired filename, if exists.
       * \param filename -> filename of the item
       * \return -> pointer to the found itemect of NULL */
      Item* getItemByFilename(Kobold::String filename);

      /*! Get a item from the slots with the desired relatedInfo, if exists.
       * \param relatedInfo -> info of the item
       * \return -> pointer to the found itemect of NULL */
      Item* getItemByInfo(Kobold::String relatedInfo);

      /*! Get the number of items with relatedInfo in slot */
      int countItemByInfo(Kobold::String relatedInfo);


      /*! Get the number of items at the ItemSlot
       * \return -> number of items at the ItemSlot */
      int getTotalItems();

   private:
      int width,   /**< Size X of the slot */
          height;  /**< Size Y of the slot */

      int totalItems;  /**< Number of Items at the ItemSlot */

      Space** spaces;  /**< Each Space of the Slot */
      bool oneItemPerTime; /**< If only accepts one item at the slot per time,
                                usually used at the equipped item slots.
                                When only one item per time, it will ALLWAYS
                                at the 0,0 position of the slot. */

};

}


#endif

