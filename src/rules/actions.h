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

#ifndef _dnt_actions_h
#define _dnt_actions_h

#include <kobold/kstring.h>
#include <OGRE/OgrePrerequisites.h>

#include "thing.h"
#include "dices.h"
#include "modifier.h"

#define ACT_STR_OPEN                 "ACT_OPEN"
#define ACT_STR_ATTACK               "ACT_ATTACK"
#define ACT_STR_DRINK                "ACT_DRINK"
#define ACT_STR_EAT                  "ACT_EAT"
#define ACT_STR_DEFEND               "ACT_DEFEND"
#define ACT_STR_TALK                 "ACT_TALK"
#define ACT_STR_CLOSE                "ACT_CLOSE"
#define ACT_STR_MOVE                 "ACT_MOVE"
#define ACT_STR_TAKE                 "ACT_TAKE"
#define ACT_STR_FREE                 "ACT_FREE"
#define ACT_STR_MODIFY_OWN_CONCEPT   "ACT_MODIFY_OWN_CONCEPT"
#define ACT_STR_MODIFY_OTHER_CONCEPT "ACT_MODIFY_OTHER_CONCEPT"
#define ACT_STR_CONJURE              "ACT_CONJURE"
#define ACT_STR_MODIFY_OBJECT        "ACT_MODIFY_OBJECT"
#define ACT_STR_HEAL                 "ACT_HEAL"
#define ACT_STR_FIX                  "ACT_FIX"
#define ACT_STR_BREAK                "ACT_BREAK"
#define ACT_STR_WAIT                 "ACT_WAIT"

#define ACT_TYPE_STR_FREE_ACTION       "ACT_TYPE_FREE_ACTION"
#define ACT_TYPE_STR_NORMAL_ACTION     "ACT_TYPE_NORMAL_ACTION"
#define ACT_TYPE_STR_SECUNDARY_ACTION  "ACT_TYPE_SECUNDARY_ACTION"
#define ACT_TYPE_STR_NO_ACTION         "ACT_TYPE_NO_ACTION"

#define ACT_MOVE_DELTA 5

namespace DNT
{

   class Action
   {
      public:

         enum ActionId
         {
            ACT_NONE,
            ACT_OPEN,
            ACT_ATTACK,
            ACT_DRINK,
            ACT_EAT,
            ACT_DEFEND,
            ACT_TALK,
            ACT_CLOSE,
            ACT_MOVE,
            ACT_TAKE,
            ACT_FREE,
            ACT_MODIFY_OWN_CONCEPT,
            ACT_MODIFY_OTHER_CONCEPT,
            ACT_CONJURE,
            ACT_MODIFY_OBJECT,
            ACT_HEAL,
            ACT_FIX,
            ACT_BREAK,
            ACT_WAIT
         };

         enum ActionType
         {
            ACT_TYPE_FREE_ACTION,
            ACT_TYPE_NORMAL_ACTION,
            ACT_TYPE_SECUNDARY_ACTION,
            ACT_TYPE_NO_ACTION
         };

         /*!
          ************************************************************* 
          * Return the action identifier number from its string identifier
          * \param stringId -> string with the name of the concept
          * \return correspondent action integer identifier.
          ***************************************************************/
         static ActionId getId(Kobold::String stringId);

         /*!
          ************************************************************** 
          * Return the number of the Action Type with name buffer
          * \param buffer -> string with the name of the concept
          * \return correspondent action Type number.
          ***************************************************************/
         static ActionType getType(Kobold::String buffer);

         /*!
          **************************************************************
          * Check if an action is within range.
          * \param actorPos actor position
          * \param targetPos target position
          * \param range current action range
          * \return if in range or not.
          **************************************************************/ 
         static bool isInRange(Ogre::Vector3 actorPos, Ogre::Vector3 targetPos,
                               int range);

         /*!
          **************************************************************** 
          * Do an action that will affect the Life Points of the Target
          * \param actor -> Thing that will act
          * \param target -> target of the action
          * \param diceInfo -> the action dice
          * \param conceptBonus -> bonus to the action.
          * \param range -> action range (==0 to not verify range: 
          *                 usually when already verified)
          * \param heal -> true if is a heal/fix feat, 
          *                false if is an attack/break one.
          * \note -> this wil use armatureClass as against concept
          * \return \c true if could attack
          ***************************************************************/
         static bool doHealOrAttack(Thing* actor, Thing* target, 
               DiceInfo& diceInfo, Factor& conceptBonus, int range, bool heal);

         /*! Do an action that will affect the Life Points of the Target
          * \param actor -> Thing that will act
          * \param target -> target of the action
          * \param diceInfo -> the action dice
          * \param conceptBonus -> bonus to the action 
          * \param conceptAgainst -> the target's concept to roll against 
          * \param range -> action range (==0 to not verify range: usually
          *                 when already verified)
          * \param heal -> true if is a heal/fix feat, 
          *                false if is an attack/break one.
          * \return \c true if could attack */
         static bool doHealOrAttack(Thing* actor, Thing* target, 
               DiceInfo& diceInfo, Factor& conceptBonus,
               Factor& conceptAgainst, int range, bool heal);

         /*! Do an action that will negatively affect the life points of
          * characters  in the area.
          * \note : TODO: affect objects too.
          * \param actor -> Thing that will act 
          * \param targetX -> central X target
          * \param targetZ -> central Z target
          * \param diceInfo -> the action dice
          * \param conceptBonus -> bonus to the action 
          * \param conceptAgainst -> the target's concept to roll against 
          * \param radius -> radius of target circle
          * \param range -> action range 
          * \param eng -> pointer to current DNT engine */
         static bool doAreaAttack(Thing* actor, float targetX, float targetZ,
               int radius, DiceInfo& diceInfo, Factor& conceptBonus,
               Factor& conceptAgainst, int range);
   };
}

#endif

