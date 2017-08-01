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

#include "dices.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
using namespace DNT;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                dice                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Dice::Dice(DiceType type)
{
   init(type);
   srand(SDL_GetTicks() + (int)(1 + 1000 * (rand() / (RAND_MAX + 1.0))));
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Dice::~Dice()
{
}

/***********************************************************************
 *                              operator=                              *
 ***********************************************************************/
Dice& Dice::operator=(const Dice& d)
{
   diceType = d.diceType;
   numberOfDices = d.numberOfDices;
   sumNumber = d.sumNumber;
   criticalMultiplier = d.criticalMultiplier;

   return *this;
}

/***********************************************************************
 *                                  init                               *
 ***********************************************************************/
void Dice::init(DiceType type)
{
   diceType = type;
   numberOfDices = 1;
   sumNumber = 0;
   criticalMultiplier = 1;
}

/***********************************************************************
 *                               setType                               *
 ***********************************************************************/
void Dice::setType(DiceType type)
{
   diceType = type;
}

/***********************************************************************
 *                               getType                               *
 ***********************************************************************/
Dice::DiceType Dice::getType()
{
   return diceType;
}

/***********************************************************************
 *                           setNumberOfDices                          *
 ***********************************************************************/
void Dice::setNumberOfDices(int num)
{
   numberOfDices = num;
}

/***********************************************************************
 *                           getNumberOfDices                          *
 ***********************************************************************/
int Dice::getNumberOfDices()
{
   return numberOfDices;
}

/***********************************************************************
 *                             setSumNumber                            *
 ***********************************************************************/
void Dice::setSumNumber(int num)
{
   sumNumber = num;
}

/***********************************************************************
 *                             getSumNumber                            *
 ***********************************************************************/
int Dice::getSumNumber()
{
   return sumNumber;
}

/***********************************************************************
 *                        setCriticalMultiplier                         *
 ***********************************************************************/
void Dice::setCriticalMultiplier(int num)
{
   criticalMultiplier = num;
}

/***********************************************************************
 *                        getCriticalMultiplier                        *
 ***********************************************************************/
int Dice::getCriticalMultiplier()
{
   return criticalMultiplier;
}

/***********************************************************************
 *                                  roll                               *
 ***********************************************************************/
int Dice::roll(bool critical)
{
   int d; /* dice counter */
   int value = 0; /* the value got */

   if(diceType == DICE_TYPE_NONE)
   {
      /* No faces, no roll. */
      return 0;
   }

   /* Throw the dices */
   for(d = 0; d < numberOfDices; d++)
   {
      value += 1 + (int)(diceType * (rand() / (RAND_MAX + 1.0)));
   }

   /* Apply Sum Factor */
   value += sumNumber;

   /* If critical, reroll as many times defined the dices */
   if(critical)
   {
      for(d = 0; d < criticalMultiplier; d++)
      {
         value += roll(false);
      }
   }

   /* Finally, the value */
   return value;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            diceInfo                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
DiceInfo::DiceInfo()
         :baseDice(Dice::DICE_TYPE_NONE),
          aditionalDice(Dice::DICE_TYPE_NONE)

{
   /* Default is no aditional dices */
   aditionalLevels = 0;
   initialLevel = 0;
}

/***********************************************************************
 *                              Populate                               *
 ***********************************************************************/
DiceInfo& DiceInfo::operator=(const DiceInfo& d)
{
   /* Set dices */
   baseDice = d.baseDice;
   aditionalDice = d.aditionalDice;
   
   /* Set aditional info */
   aditionalLevels = d.aditionalLevels;
   initialLevel = d.initialLevel;

   return *this;
}

/***********************************************************************
 *                             setBaseDice                             *
 ***********************************************************************/
void DiceInfo::setBaseDice(Dice dice)
{
   baseDice = dice;
}

/***********************************************************************
 *                           setAditinalDice                           *
 ***********************************************************************/
void DiceInfo::setAditionalDice(Dice dice)
{
   aditionalDice = dice;
}

/***********************************************************************
 *                         setAditionalLevels                          *
 ***********************************************************************/
void DiceInfo::setAditionalLevels(int levels)
{
   aditionalLevels = levels;
}

/***********************************************************************
 *                           setInitialLevel                           *
 ***********************************************************************/
void DiceInfo::setInitialLevel(int level)
{
   initialLevel = level;
}

/***********************************************************************
 *                               roll                                  *
 ***********************************************************************/
int DiceInfo::roll(int level, bool critical)
{
   int value = 0;

   /* Verify if can use the DiceInfo */
   if(level > initialLevel)
   {
      /* Aply base dice */
      value += baseDice.roll(critical);

      /* Apply any aditional dices */
      int aditionals = (level - initialLevel) / aditionalLevels;
      for(int i = 0; i < aditionals; i++)
      {
         value += aditionalDice.roll(critical);
      }
   }

   return value;
}

