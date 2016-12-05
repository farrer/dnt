/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
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

#include "money.h"
using namespace DNT;

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
Money::Money()
{
}

/***********************************************************************
 *                                Destructor                           *
 ***********************************************************************/
Money::~Money()
{
}

/***********************************************************************
 *                                 quantity                            *
 ***********************************************************************/
int Money::quantity()
{
   return getCost();
}

/***********************************************************************
 *                              removeQuantity                         *
 ***********************************************************************/
bool Money::removeQuantity(int qty)
{
   /* Only remove if have the quantity */
   if(getCost() >= qty)
   {
      setCost(getCost() - qty);
      calculateWeight();
      setState(getCost());

      return true;
   }
   return false;
}

/***********************************************************************
 *                              addQuantity                         *
 ***********************************************************************/
void Money::addQuantity(int qty)
{
   setCost(getCost() + qty);
   calculateWeight();
   setState(getCost());
}

/***********************************************************************
 *                               setQuantity                           *
 ***********************************************************************/
void Money::setQuantity(int qty)
{
   setCost(qty);
   calculateWeight();
   setState(getCost());
}

/***********************************************************************
 *                             calculateWeight                         *
 ***********************************************************************/
void Money::calculateWeight()
{
   setWeight(getCost() / 100.0f);
}

/***********************************************************************
 *                      doObjectSpecializationParse                    *
 ***********************************************************************/
bool Money::doObjectSpecializationParse(Ogre::String key, Ogre::String value)
{
   /* Nothing specific to load */
   return false;
}

/***********************************************************************
 *                      doObjectSpecializationSave                     *
 ***********************************************************************/
bool Money::doObjectSpecializationSave(std::ofstream& file)
{
   /* Nothing specific to save */
   return true;
}


