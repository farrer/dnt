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

#include "bonusandsaves.h"
#include "../gui/briefing.h"
#include "../lang/translate.h"
#include "dices.h"
using namespace DNT;

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             AttackBonus                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
AttackBonus::AttackBonus()
{
   clear();
}

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
AttackBonus::AttackBonus(int total)
{
   int i;
   int curVal = total;

   /* Distribute the total value between each base bonus the following way:
    * If the current reachs 6, a new possible attack is achieved,
    * up to MAX_ATTACKS. */
   for(i = 0; (i < MAX_ATTACKS); i++)
   {
      if(curVal > 0)
      {
         bonus[i] = curVal;
      }
      else
      {
         bonus[i] = 0;
      }

      /* For the next base attack the bonus will be 5 lesser */
      curVal -= 5;
   }

}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
AttackBonus::~AttackBonus()
{
}

/***********************************************************************
 *                                clear                                *
 ***********************************************************************/
void AttackBonus::clear()
{
   int i;
   for(i=0; i <MAX_ATTACKS; i++)
   {
      bonus[i] = 0;
   }
}

/***********************************************************************
 *                                toString                             *
 ***********************************************************************/
Kobold::String AttackBonus::toString()
{
   Kobold::String res = "";
   char val[16];
   int i;

   for(i=0; i < MAX_ATTACKS; i++)
   {
      /* Only Convert non 0 bonus */
      if(bonus[i] != 0)
      {
         /* Put a separator, if needed */
         if(i > 0)
         {
            res += "/";
         }
         sprintf(val,"%d", bonus[i]);
         res += "+";
         res += val;
      }
   }

   /* If empty, no bonus, so 0 is defined */
   if(res.empty())
   {
      res = "0";
   }

   return res;
}

/***********************************************************************
 *                                toInt                                *
 ***********************************************************************/
int AttackBonus::toInt()
{
   return bonus[0];
}

/***********************************************************************
 *                               getBonus                              *
 ***********************************************************************/
int AttackBonus::getBonus(int attackNumber)
{
   if( (attackNumber > 0) && (attackNumber <= MAX_ATTACKS))
   {
      return bonus[attackNumber-1];
   }

   return 0;
}

/***********************************************************************
 *                               setBonus                              *
 ***********************************************************************/
void AttackBonus::setBonus(int attackNumber, int value)
{
   if( (attackNumber > 0) && (attackNumber <= MAX_ATTACKS))
   {
      bonus[attackNumber-1] = value;
   }
}

/***********************************************************************
 *                               operator+                             *
 ***********************************************************************/
AttackBonus AttackBonus::operator+(const AttackBonus& atBonus)
{
   return(AttackBonus(toInt()+atBonus.bonus[0]));
}

/***********************************************************************
 *                               operator=                             *
 ***********************************************************************/
AttackBonus& AttackBonus::operator=(const AttackBonus& atBonus)
{
   int i;
   for(i=0; i < MAX_ATTACKS; i++)
   {
      bonus[i] = atBonus.bonus[i];
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            BonusAndSaves                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
BonusAndSaves::BonusAndSaves()
{
   clear();
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
BonusAndSaves::~BonusAndSaves()
{
}

/***********************************************************************
 *                                 parse                               *
 ***********************************************************************/
bool BonusAndSaves::parse(Kobold::String value)
{
   Ogre::vector<Ogre::String>::type res = Ogre::StringUtil::split(value, ",/");
   if(res.size() != 5)
   {
      return false;
   }

   sscanf(res[0].c_str(), "%d", &level);
   int tmpBonus = 0;
   sscanf(res[1].c_str(), "%d", &tmpBonus);
   baseAttackBonus = AttackBonus(tmpBonus);
   sscanf(res[2].c_str(), "%d", &fortitude);
   sscanf(res[3].c_str(), "%d", &reflexes);
   sscanf(res[4].c_str(), "%d", &iAmNotAFool);

   return true;
}

/***********************************************************************
 *                                 clear                               *
 ***********************************************************************/
void BonusAndSaves::clear()
{
   level = 0;
   iAmNotAFool = 0;
   reflexes = 0;
   fortitude = 0;
   baseAttackBonus.clear();
}

/***********************************************************************
 *                               operator+                             *
 ***********************************************************************/
BonusAndSaves BonusAndSaves::operator+(const BonusAndSaves& b)
{
   BonusAndSaves res;
   res.level = level + b.level;
   res.iAmNotAFool = iAmNotAFool + b.iAmNotAFool;
   res.reflexes = reflexes + b.reflexes;
   res.fortitude = fortitude + b.fortitude;
   res.baseAttackBonus = baseAttackBonus + b.baseAttackBonus;

   return res;
}

/***********************************************************************
 *                               operator=                             *
 ***********************************************************************/
BonusAndSaves& BonusAndSaves::operator=(const BonusAndSaves& b)
{
   level = b.level;
   iAmNotAFool = b.iAmNotAFool;
   reflexes = b.reflexes;
   fortitude = b.fortitude;
   baseAttackBonus = b.baseAttackBonus;
   return *this;
}

/***********************************************************************
 *                              getValue                               *
 ***********************************************************************/
int BonusAndSaves::getValue(Kobold::String state)
{
   if(state == DNT_BS_LEVEL)
   {
      return level;
   }
   else if(state == DNT_BS_FORTITUDE)
   {
      return fortitude;
   }
   else if(state == DNT_BS_REFLEXES)
   {
      return reflexes;
   }
   else if( (state == DNT_BS_I_AM_NOT_A_FOOL) ||
            (state == DNT_BS_WILL))
   {
      return iAmNotAFool;
   }

   return 0;
}

/***********************************************************************
 *                               doCheck                               *
 ***********************************************************************/
bool BonusAndSaves::doCheck(Kobold::String stateToCheck, int difficulty,
      bool* couldCheck)
{
   bool canCheck = false;
   bool checkRes = false;
   int value = 0;
   CheckType checkType = CHECK_ROLL;
   Kobold::String stateName = "";
   Dice d20(Dice::DICE_TYPE_D20);

   /* Let's get the state (if defined) */
   if(stateToCheck == DNT_BS_LEVEL)
   {
      checkType = CHECK_COMPARE;
      canCheck = true;
      value = level;
      stateName = gettext("Level");
   }
   else if(stateToCheck == DNT_BS_FORTITUDE)
   {
      value = fortitude;
      canCheck = true;
      stateName = gettext("Fortitude");
   }
   else if(stateToCheck == DNT_BS_REFLEXES)
   {
      value = reflexes;
      canCheck = true;
      stateName = gettext("Reflexes");
   }
   else if( (stateToCheck == DNT_BS_I_AM_NOT_A_FOOL) ||
            (stateToCheck == DNT_BS_WILL))
   {
      value = iAmNotAFool;
      canCheck = true;
      stateName = gettext("I am Not a Fool");
   }

   /* Do the check, if defined */
   if(canCheck)
   {
      if(checkType == CHECK_ROLL)
      {
         value += d20.roll();
      }
      Briefing::addCheckText(stateName, value, difficulty);
      checkRes = (value >= difficulty);
   }

   /* Set the flag */
   if(couldCheck != NULL)
   {
      *couldCheck = canCheck;
   }
   return(checkRes);
}

/***********************************************************************
 *                            setIAmNotAFool                           *
 ***********************************************************************/
void BonusAndSaves::setIAmNotAFool(int i)
{
   iAmNotAFool = i;
}

/***********************************************************************
 *                              setReflexes                            *
 ***********************************************************************/
void BonusAndSaves::setReflexes(int i)
{
   reflexes = i;
}

/***********************************************************************
*                              setFortitude                            *
 ***********************************************************************/
void BonusAndSaves::setFortitude(int i)
{
   fortitude = i;
}

/***********************************************************************
 *                             setBaseAttack                           *
 ***********************************************************************/
void BonusAndSaves::setBaseAttack(int i)
{
   int aux;

   for(aux = 1; aux <= MAX_ATTACKS; aux++)
   {
      baseAttackBonus.setBonus(aux, i);
   }
}

/***********************************************************************
 *                             getBaseAttack                           *
 ***********************************************************************/
AttackBonus* BonusAndSaves::getBaseAttackBonus()
{
   return &baseAttackBonus;
}

/***********************************************************************
 *                               isDefined                             *
 ***********************************************************************/
bool BonusAndSaves::isDefined()
{
   return level != 0;
}

