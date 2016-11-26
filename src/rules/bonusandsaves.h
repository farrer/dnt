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

#ifndef _dnt_bonus_and_saves_h
#define _dnt_bonus_and_saves_h

#include <kobold/kstring.h>

namespace DNT
{

/*! Maximum number of attacks at a single act */
#define MAX_ATTACKS 6 

/*! The AttackBonus class represents a single base attack bonus */
class AttackBonus
{
   public:
      /*! Constructor */
      AttackBonus();

      /*! Constructor
       * \total -> total bonus value
       * \note -> usually, the total is relative to the 
       *          first attack bonus.
       * \note -> you can get the total value if the toInt() function */
      AttackBonus(int total);

      /*! Destructor */
      ~AttackBonus();

      /*! Clear the bonus values */
      void clear();

      /*! + Overloaded operator -> will sum AttackBonus
       * \param atBonus -> AttackBonus to sum
       * \return -> sum result */
      AttackBonus operator+(const AttackBonus& atBonus);

      /*! = Overloaded operator
       * \param atBonus -> AttackBonus to use as base */
      AttackBonus& operator=(const AttackBonus& atBonus);

      /*! \return -> string representing the attack bonus */
      Kobold::String toString();

      /*! Convert the attack bonus to a representative integer
       * \return -> int with the attack bonus */
      int toInt();

      /*! Get bonus value for an attack in a single act.
       * \param attackNumber -> number of the attack (1 for first) 
       * \return -> base attack bonus for the attack */
      int getBonus(int attackNumber);

      /*! Set bonus value for an attack in a single act.
       * \param attackNumbner -> number of the attack (1 for first)
       * \param value -> bonus value for the attack */
      void setBonus(int attackNumber, int value);

    protected:
      int bonus[MAX_ATTACKS];  /**< The Bonuses itself */
};

#define DNT_BS_LEVEL           "LEVEL"
#define DNT_BS_FORTITUDE       "FORTITUDE"
#define DNT_BS_REFLEXES        "REFLEXES"
#define DNT_BS_I_AM_NOT_A_FOOL "I_AM_NOT_A_FOOL"
#define DNT_BS_WILL            "WILL"

/*! Per Level Bonus and Saves modifiers. Those attributes are used
 * for almost every check made on the game (for example, how much 
 * pain suffered on a failed trap disarm, etc.). */
class BonusAndSaves
{
   public:
      
      enum CheckType
      {
         CHECK_ROLL = 1,  /**< Roll a dice check */
         CHECK_COMPARE    /**< Compare values check */
      };

      /*! Constructor */
      BonusAndSaves();
      /*! Destructor */
      ~BonusAndSaves();

      /*! Parse value from definition string of type "%d,%d,%d/%d/%d" */
      bool parse(Kobold::String value);

      /*! Clear Values */
      void clear();

      /*! Do a check of a bonus or a save.
       * \param stateToCheck -> state to check constant
       * \param difficulty -> difficulty of the test
       * \param couldCheck -> will be true if the stateToCheck is valid here,
       *                      or false if it is invalid (not a bonus or a save)
       * \return true if success, false if failed. */
      bool doCheck(Kobold::String stateToCheck, int difficulty,
                   bool* couldCheck=NULL);
      /*! Get the value of a bonus or save
       * \param state -> bonus or save to get value 
       * \return bonus or save value */
      int getValue(Kobold::String state);

      /*! = Operator */
      BonusAndSaves& operator=(const BonusAndSaves& b);

      /*! + Operator */
      BonusAndSaves operator+(const BonusAndSaves& b);

      /*! Set the I am Not a fool value*/
      void setIAmNotAFool(int i);
      /*! Set reflexes value */
      void setReflexes(int i);
      /*! Set the fortitude value */
      void setFortitude(int i);
      /*! Set the base attack value */
      void setBaseAttack(int i);

      /*! \return base attack bonus pointer */
      AttackBonus* getBaseAttackBonus();

   protected:

      int level;  /**< Class Level when this BonusAndSaves are valid. */
      AttackBonus baseAttackBonus; /**< Base Attack Bonus */
      int fortitude;               /**< Fortitude Save Bonus */
      int reflexes;                /**< Reflexes Save Bonus */
      int iAmNotAFool;             /**< I Am Not A Fool Save Bonus */
};

}

#endif
