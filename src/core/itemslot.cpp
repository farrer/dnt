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

#include "itemslot.h"
#include <assert.h>
using namespace DNT;

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                                Space                                //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/****************************************************************
 *                            Space                             *
 ****************************************************************/
ItemSlot::Space::Space()
{
   item = NULL;
}

/****************************************************************
 *                           ~Space                             *
 ****************************************************************/
ItemSlot::Space::~Space()
{
}

/****************************************************************
 *                           setItem                            *
 ****************************************************************/
void ItemSlot::Space::setItem(Item* item, Ogre::Vector2 orig)
{
   this->item = item;
   this->origin = orig;
}

/****************************************************************
 *                           getItem                            *
 ****************************************************************/
Item* ItemSlot::Space::getItem()
{
   return item;
}

/****************************************************************
 *                          getOrigin                           *
 ****************************************************************/
Ogre::Vector2 ItemSlot::Space::getOrigin()
{
   return origin;
}

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                             ItemSlot                                //
//                                                                     //
/////////////////////////////////////////////////////////////////////////


/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
ItemSlot::ItemSlot(int xSize, int ySize)
{
   /* Set initial values */
   width = xSize;
   height = ySize;
   oneItemPerTime = false;
   totalItems = 0;

   /* Create our Spaces */
   spaces = new Space*[width];
   for(int i = 0; i < width; i++)
   {
      spaces[i] = new Space[height];
   }
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
ItemSlot::~ItemSlot()
{
   /* Mark as Unused Items */
   for(int x = 0; x < width; x++)
   {
      for(int y = 0; y < height; y++)
      {
         Ogre::Vector2 orig = Ogre::Vector2(x, y);
         if( (spaces[x][y].getItem() != NULL) &&
             (spaces[x][y].getOrigin() == orig) )
         {
            /* Delete the item */
            delete spaces[x][y].getItem();
         }
         spaces[x][y].setItem(NULL, orig);
      }
      /* Delete spaces */
      delete[] spaces[x];
   }

   /* Delete our enter lines */
   delete[] spaces;
}

/**************************************************************
 *                           addItem                        *
 **************************************************************/
bool ItemSlot::addItem(Item* item, int x, int y)
{
   if(!item)
   {
      return false;
   }

   Ogre::Vector2 size = item->getInventorySize();

   if(canAdd(item, x, y))
   {
      int endX = x + size.x;
      int endY = y + size.y;
      Ogre::Vector2 orig(x, y);

      /* Occupy all needed spaces */
      for(int j = x; j < endX; j++)
      {
         for(int k = y; k < endY; k++)
         {
            spaces[j][k].setItem(item, orig);
         }
      }

      totalItems++;
      return true;
   }
   return false;
}

/**************************************************************
 *                           addItem                        *
 **************************************************************/
bool ItemSlot::addItem(Item* item)
{
   int x,y;

   if(!item)
   {
      return(false);
   }

   for(x=0; x < width; x++)
   {
      for(y=0; y < height; y++)
      {
         if(addItem(item, x, y))
         {
            totalItems++;
            return(true);
         }
      }
   }
   return(false);
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool ItemSlot::canAdd(Item* item, int x, int y)
{
   assert(item != NULL);

   Ogre::Vector2 size = item->getInventorySize();
   int endX = x + size.x;
   int endY = y + size.y;

   if( (x < 0) || (y < 0) || (endX > width) || (endY > height))
   {
      /* Won't fit or without limits */
      return false;
   }

   if( (oneItemPerTime) && (spaces[0][0].getItem() != NULL))
   {
      /* Defined to only have a single item, and already have it. */
      return false;
   }

   /* Check slots that will use if they are empty */
   for(int j = x; j < endX; j++)
   {
      for(int k = y; k < endY; k++)
      {
         if(spaces[j][k].getItem() != NULL)
         {
            return false;
         }
      }
   }

   /* Passed all tests: can add. */
   return true;
}

/**************************************************************
 *                       getFromPosition                      *
 **************************************************************/
Item* ItemSlot::getFromPosition(int x, int y)
{
   assert((x >= 0) && (y >=0) && (x < width) && (y < height));
   
   return spaces[x][y].getItem();
}

/**************************************************************
 *                         removeItem                       *
 **************************************************************/
bool ItemSlot::removeItem(Item* item)
{
   assert(item != NULL);
  
   /* Search for the item */
   for(int x = 0; x < width; x++)
   {
      for(int y = 0; y < height; y++)
      {

         if(spaces[x][y].getItem() == item)
         {
            /* Found it: remove only from the inventory (as will 
             * use it elsewhere) */
            removeItem(spaces[x][y].getOrigin().x, spaces[x][y].getOrigin().y);
            totalItems--;
            return true;
         }
      }
   }

   /* Not Found */
   return false;
}

/**************************************************************
 *                         removeItem                       *
 **************************************************************/
void ItemSlot::removeItem(int x, int y)
{
   assert((x >= 0) && (y >= 0) && (x < width) && (y < height));


   Item* item = spaces[x][y].getItem();
   assert(item != NULL);

   Ogre::Vector2 size = item->getInventorySize();
   Ogre::Vector2 origin = spaces[x][y].getOrigin();
   Ogre::Vector2 end = origin + size;

   assert((end.x >= 0) && (end.y >= 0) && 
          (end.x < width) && (end.y < height));

   /* Empty all occupied spaces */
   for(int j= origin.x; j < end.x; j++)
   {
      for(int k = origin.y; k < end.y; k++)
      {
         spaces[j][k].setItem(NULL, Ogre::Vector2(j, k));
      }
   }

   /* Decrement our totals */
   totalItems--;
}

/**************************************************************
 *                     setAsOneItemPerTime                    *
 **************************************************************/
void ItemSlot::setAsOneItemPerTime()
{
   oneItemPerTime = true;
}

/**************************************************************
 *                         getFirstItem                       *
 **************************************************************/
Item* ItemSlot::getFirstItem(int& x, int& y)
{
   x = 0;
   y = -1;
   return getNextItem(x,y);
}

/**************************************************************
 *                          getNextItem                       *
 **************************************************************/
Item* ItemSlot::getNextItem(int& x, int& y)
{
   int j,k;
 
   k = y + 1; /* To get next element from previous */
   for(j = x; j < width; j++)
   {
      for(;k < height; k++)
      {
         if( (spaces[j][k].getItem() != NULL) && 
             (spaces[j][k].getOrigin() == Ogre::Vector2(j, k)) )
         {
            x = j;
            y = k;
            return spaces[j][k].getItem();
         }
      }
      k = 0; /* To continue the search at first y position */
   }

   x = -1;
   y = -2;
   return NULL;
}

/**************************************************************
 *                       getItemByFilename                    *
 **************************************************************/
Item* ItemSlot::getItemByFilename(Kobold::String filename)
{
   for(int j = 0; j < width; j++)
   {
      for(int k = 0; k < height; k++)
      {
         if( (spaces[j][k].getItem() != NULL) &&
             (spaces[j][k].getItem()->getFilename() == filename) )
         {
            /* Found One! */
            return spaces[j][k].getItem();
         }
      }
   }
   return NULL;
}

/**************************************************************
 *                         getItemByInfo                      *
 **************************************************************/
Item* ItemSlot::getItemByInfo(Kobold::String relatedInfo)
{
   for(int j = 0; j < width; j++)
   {
      for(int k = 0; k < height; k++)
      {
         if( (spaces[j][k].getItem() != NULL) &&
             (spaces[j][k].getItem()->getRelatedInfo() == relatedInfo) )
         {
            /* Found One! */
            return spaces[j][k].getItem();
         }
      }
   }
   return NULL;
}

/**************************************************************
 *                        countItemByInfo                     *
 **************************************************************/
int ItemSlot::countItemByInfo(Kobold::String relatedInfo)
{
   int count = 0;
   for(int j = 0; j < width; j++)
   {
      for(int k = 0; k < height; k++)
      {
         if( (spaces[j][k].getItem() != NULL) &&
             (spaces[j][k].getItem()->getRelatedInfo() == relatedInfo) &&
             (spaces[j][k].getOrigin() == Ogre::Vector2(j, k)) )
         {
            /* Found One! */
            count++;
         }
      }
   }
   return count;
}

/**************************************************************
 *                         getTotalItems                      *
 **************************************************************/
int ItemSlot::getTotalItems()
{
   return totalItems;
}

