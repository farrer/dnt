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

#ifndef _dnt_money_h
#define _dnt_money_h

#include "item.h"

namespace DNT
{

//FIXME: this should be defined by the Module!
#define DNT_MONEY_OBJECT "pickable/other/money.dcc"

/*! Monetary objecy representation. Money is a distinct type of object, due
 * to its special monetary means. */
class Money: public Item
{
   public:
      /*! Constructor  */
      Money();

      /*! Destructor */
      ~Money();

      /*! Get the current Money quantity (value)
       * \return -> current Money value */
      int getQuantity();

      /*! Remove quantity from current Money
       * \param qty -> quantity to remove 
       * \return -> true if can remove, false if not enought Money */
      bool removeQuantity(int qty);

      /*! Add a Money quantity to the current Money
       * \param qty -> quantity to add to current */
      void addQuantity(int qty);

      /*! Set a Money quantity to the current Money
       * \param qty -> quantity to set as current */
      void setQuantity(int qty);

   protected:
      
      bool doObjectSpecializationParse(const Kobold::String& key, 
            const Kobold::String& value);
      bool doObjectSpecializationSave(std::ofstream& file);

   private:

      /*! Calculate the weight */
      void calculateWeight();

};

}

#endif

