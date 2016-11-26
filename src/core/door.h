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

#ifndef _dnt_door_h
#define _dnt_door_h

#include "object.h"

namespace DNT
{

/*! A Door is an object that could be opened and closed, at the Character's
 * way from some room to another one. It could be initially locked. */
class Door : public Object
{
   public:
      enum OpenStatus
      {
         DOOR_CLOSED = 0,
         DOOR_OPENED
      };

      /* #state meaning for doors: Unlocked or locked. */
      enum LockedState
      {
         DOOR_UNLOCKED = 0,
         DOOR_LOCKED
      };

      /*! Constructor */
      Door();
      /*! Destructor */
      ~Door();

      /*! Open/close the door.
       * \return if action was successfull. */
      bool flip();

      /*! \return current Open Status: if door is opened or closed */
      OpenStatus getOpenStatus();

      /*! Set if door is locked or unlocked, without any checks
       * and without any effects (as sound and animations) */
      void setOpenStatus(OpenStatus openStatus);

      /*! Set model's yaw angle.
       * \param angle -> yaw angle when door is closed */
      void setClosedAngle(int angle);

   protected:
      /*! Parse specific door attributes */
      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);

      int closedAngle; /**< Model's yaw angle when closed */

      OpenStatus openStatus; /**< If actually locked or unlocked */


};

}

#endif

