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

#ifndef _dnt_dices_h
#define _dnt_dices_h

namespace DNT
{

/*! This class defines a single dice of 'x' faces, that could be rolled
 *  to a value between 1 and 'x', 'n' times and with its final value
 *  added to sum number. */
class Dice
{
   public:
      enum DiceType
      {
        DICE_TYPE_D2  =  2,     /**< Two Faces Dice */
        DICE_TYPE_D4  =  4,     /**< Four Faces Dice */
        DICE_TYPE_D6  =  6,     /**< Six Faces Dice */
        DICE_TYPE_D8  =  8,     /**< Eight Faces Dice */
        DICE_TYPE_D10 = 10,     /**< Ten Faces Dice */
        DICE_TYPE_D12 = 12,     /**< Twelve Faces Dice */
        DICE_TYPE_D20 = 20      /**< Twenty Faces Dice */
      };

      /*! Constructor: Create a dice of the type
       * \param type -> the dice type constant */
      Dice(DiceType type = DICE_TYPE_D20);
      /*! Destructor */
      ~Dice();

      /* Dice assign from another instance. */
      Dice& operator=(const Dice& d);

      /*! Roll the dice
       * \param critical -> true if is a critical roll
       * \note -> at critical rolls, the dice is rolled 
       *          criticalMultiplier times
       * \return -> sum of value got by the dices */
      int roll(bool critical=false);

      /*! Set the dice type
       * \param type -> dice type constant */
      void setType(DiceType type);
      /*! Get the dice type
       * \return -> dice type */
      DiceType getType();

      /*! Set the number of dices (default: 1)
       * \param num -> number of dices value */
      void setNumberOfDices(int num);
      /*! Get the current number of dices
       * \return -> number of dices */
      int getNumberOfDices();

      /*! Set the sum number (default: 0)
       * \param num -> sum number value */
      void setSumNumber(int num);
      /*! Get the current sum number
       * \return -> sum number */
      int getSumNumber();

      /*! Set the critical multiplier (default: 1)
       * \param num -> critical multiplier value */
      void setCriticalMultiplier(int num);
      /*! Get the current critical multiplier
       * \return -> critical multiplier */
      int getCriticalMultiplier();

   protected:
 
      /*! Set initial dice values
       * \param type -> dice type constant */
      void init(DiceType type);

      DiceType diceType;  /**< Type of Dice. Represents the number of faces.*/
      int numberOfDices;      /**< Number of Dices to roll. */
      int sumNumber;          /**< Base Number to Sum to value.*/
      int criticalMultiplier; /**< The critical Multiplier Value */
};

/*! At this class all informations of a typical dice related, for example, to
 * an attack type are stored.
 * \note the number on the dice is calculated by: \par
 * \e baseDice+SUM((level-initialLevel)/aditionalLevels)*aditionalDice \par
 * And Level can be anything that is needed, from level itself to 
 * skills and base attributes. */
class DiceInfo
{
   public:

      /*! Constructor */
      DiceInfo();

      /*! Populate with values from another instance.
       * \param d -> pointer to another instance where will copy values from. */
      DiceInfo& operator=(const DiceInfo& d);

      Dice baseDice;         /**< Basic Dice. */
      Dice aditionalDice;    /**< Aditional Dice per Level. */
      int aditionalLevels;   /**< Number of Levels per Aditional Dices. */
      int initialLevel;      /**< Initial Level to use this diceThing. */
};

}

#endif

