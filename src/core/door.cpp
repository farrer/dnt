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

#include "door.h"
#include "../collision/collision.h"
#include <kosound/sound.h>
#include <assert.h>

using namespace DNT;

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Door::Door()
     :Object(Thing::THING_TYPE_DOOR)
{
   closedAngle = 0.0f;
   shouldResetCollision = false;
   /* Default to closed door, but not locked. */
   setOpenStatus(DOOR_CLOSED);
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Door::~Door()
{
}

/**************************************************************************
 *                              setClosedAngle                            *
 **************************************************************************/
void Door::setClosedAngle(Ogre::Real angle)
{
   closedAngle = angle;
}

/**************************************************************************
 *                              getOpenStatus                             *
 **************************************************************************/
Door::OpenStatus Door::getOpenStatus()
{
   return static_cast<OpenStatus>(getState());
}

/**************************************************************************
 *                              setOpenState                              *
 **************************************************************************/
void Door::setOpenStatus(OpenStatus openStatus)
{
   setState(openStatus);
}

/**************************************************************************
 *                                   flip                                 *
 **************************************************************************/
bool Door::flip()
{
   assert(getOpenStatus() != DOOR_LOCKED);
   Ogre::Vector3 pos = getModel()->getPosition();

   if(getOpenStatus() == DOOR_CLOSED)
   {
      /* Must open it */
      setOpenStatus(DOOR_OPENED);
      shouldResetCollision = true;

      /* Set opening animation */
      getModel()->setTargetOrientation(0.0f, closedAngle + 90.0f, 0.0f);

      /* Call opening sound */
      Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z, SOUND_NO_LOOP,
            "objects/door_open.ogg");

      return true;
   } 
   else if(getOpenStatus() == DOOR_OPENED)
   {
      /* Must close it */
      setOpenStatus(DOOR_CLOSED);
      shouldResetCollision = true;

      /* Opened, we can always close the door */
      getModel()->setTargetOrientation(0.0f, closedAngle, 0.0f);

      /* Call closing sound */
      Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z, SOUND_NO_LOOP,
            "objects/door_close.ogg");

      return true;
   }

   /* Couldn't open or close (it's locked). */
   return false;
}
 
/**************************************************************************
 *                                   lock                                 *
 **************************************************************************/    bool Door::lock(int unlockChallenge, int forceChallenge)
{
   if(getOpenStatus() != DOOR_OPENED)
   {
      /* Lock it (or re-lock it), and define its 'unlock' and 'forced open' 
       * challenge values. */
      setOpenStatus(DOOR_LOCKED);
      getCurBonusAndSaves()->setIAmNotAFool(unlockChallenge);
      getCurBonusAndSaves()->setFortitude(forceChallenge);

      return true;
   }

   /* Door is opened, and couldn't be locked! */
   return false;
}

/**************************************************************************
 *                                  unlock                                *
 **************************************************************************/
void Door::unlock()
{
   assert(getOpenStatus() == DOOR_LOCKED);
   setOpenStatus(DOOR_CLOSED);
}

/**************************************************************************
 *                                   open                                 *
 **************************************************************************/
void Door::open()
{
   if(getOpenStatus() == DOOR_CLOSED)
   {
      flip();
   }
}

/**************************************************************************
 *                                  close                                 *
 **************************************************************************/
void Door::close()
{
   if(getOpenStatus() == DOOR_OPENED)
   {
      flip();
   }
}

/**************************************************************************
 *                                 interact                               *
 **************************************************************************/
Object::InteractResult Door::interact(Character* actor)
{
   if(getOpenStatus() == DOOR_LOCKED)
   {
      /* Locked: should open unlock dialog. */
      return Object::INTERACTION_OPEN_CONVERSATION; 
   }
   else 
   {
      /* Just flip our open/close status */
      flip();
      return Object::INTERACTION_DONE;
   }
}

/**************************************************************************
 *                                update                                  *
 **************************************************************************/
void Door::update()
{
   if((!getModel()->update()) && (shouldResetCollision))
   {
      Collision::removeElement(this);
      Collision::addElement(this);
      shouldResetCollision = false;
   }
}

/**************************************************************************
 *                      doObjectSpecializationParse                       *
 **************************************************************************/
bool Door::doObjectSpecializationParse(Ogre::String key, Ogre::String value)
{
   /* Nothing to do: locked status is defined at each map. */
   return false;
}

/**************************************************************************
 *                      doObjectSpecializationSave                        *
 **************************************************************************/
bool Door::doObjectSpecializationSave(std::ofstream& file)
{
   return true;
}

