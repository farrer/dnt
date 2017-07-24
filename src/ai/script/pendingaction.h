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

#ifndef _dnt_pending_action_h
#define _dnt_pending_action_h

#include <kobold/timer.h>

namespace DNT
{

/*! A pending action is an action called on the script that requires more
 * than a simple moment to execute. For example, could be a character movement
 * alog the map (which requires A* search and the movement through multiple
 * frames until the target destination is reached) or a sleep function (which
 * have to wait until the desired amount of time passes). */
class PendingAction
{
   public:
      enum PendingActionType
      {
         TYPE_SLEEP,
         TYPE_MOVE_CHARACTER
      };

      /*! Constructor 
       * \param type of the pending action. */
      PendingAction(PendingActionType type) {this->type = type;};
      /*! Destructor */
      virtual ~PendingAction(){};

      /*! Update the pending action state.
       * \return false if continue to wait, true if finished */
      virtual bool update() = 0;

      /*! \return type of the action pending */
      const PendingActionType getType() const { return type;};

   private:
      PendingActionType type;
};

/*! A sleep pending action. Will wait until an amount of time passes */
class PendingActionSleep : public PendingAction
{
   public:
      /*! Constructor
       * \param seconds amount of seconds to wait */
      PendingActionSleep(unsigned int seconds);
      /*! Destructor */
      ~PendingActionSleep();

      /*! Update the sleep time.
       * \return if done and should awake the script */
      bool update();

   private:
      unsigned int seconds; /**< amount of seconds to sleep */
      Kobold::Timer timer;  /**< Timer used to controll sleep time */

};


}

#endif

