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

#include "dntconfig.h"
#include "object.h"

namespace DNT
{

/*! A Door is an object that could be opened and closed, usually situaded 
 * between walls, blocking or allowing passing by through a path (or corridor).
 * It could also be locked. */
class Door : public Object
{
   public:
      enum OpenStatus
      {
         DOOR_CLOSED = 0,
         DOOR_OPENED,
         DOOR_LOCKED
      };

      /*! Constructor */
      Door();
      /*! Destructor */
      ~Door();

      /*! \return current Open Status. */
      OpenStatus getOpenStatus();

      /*! Set the door open status, without any checks
       * and without any effects (as sound and animations). */
      void setOpenStatus(OpenStatus openStatus);

      /*! Set yaw angle to use for model's when the door is closed.
       * \param angle -> yaw angle when door is closed */
      void setClosedAngle(Ogre::Real angle);

      /*! Lock a door.
       * \param unlockChallenge -> how much difficult to unlock it
       * \param forceChallenge -> how much difficult to force the door to open
       * \return if locked it.
       * \note only can lock a closed door */
      bool lock(int unlockChallenge, int forceChallenge);

      /* \return true. Doors are interactive. */
      const bool canInteract() const {return true;};

      /* Interact with the door, opening/closing it if unlocked, or 
       * defining to open its conversation to unlock it, if locked. */
      Object::InteractResult interact(Character* actor);

      /*! Overriden from Thing. */
      void update();

   protected:
      /*! Parse specific door attributes */
      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);
      /*! Save specific door attributes */
      bool doObjectSpecializationSave(std::ofstream& file);

   private:

      /*! Open/close the door.
       * \note only works if not locked.
       * \return if action was successfull. */
      bool flip();

      Ogre::Real closedAngle; /**< Angle to use when the door is closed */
      bool shouldResetCollision; /**< If should reset collision after completed
                                      a door orientation flip */
};

}

#endif

