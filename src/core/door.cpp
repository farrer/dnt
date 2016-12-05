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
#include <kosound/sound.h>

using namespace DNT;

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Door::Door()
{
   closedAngle = 0;
   openStatus = DOOR_OPENED;
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
void Door::setClosedAngle(int angle)
{
   closedAngle = angle;
}

/**************************************************************************
 *                              getOpenStatus                             *
 **************************************************************************/
Door::OpenStatus Door::getOpenStatus()
{
   return openStatus;
}

/**************************************************************************
 *                              setOpenState                              *
 **************************************************************************/
void Door::setOpenStatus(OpenStatus openStatus)
{
   this->openStatus = openStatus;
}

/**************************************************************************
 *                                   flip                                 *
 **************************************************************************/
bool Door::flip()
{
   Ogre::Vector3 pos = getModel()->getPosition();

   if(openStatus == DOOR_CLOSED)
   {
      if(getState() == DOOR_UNLOCKED)
      {
         openStatus = DOOR_OPENED;
         /* Set opening animation */
         getModel()->setTargetOrientation(0.0f, closedAngle + 90.0f, 0.0f);
         /* Call opening sound */
         Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z, SOUND_NO_LOOP,
               "sndfx/objects/door_open.ogg");
         return true;
      }
   } 
   else
   {
      openStatus = DOOR_CLOSED;
      /* Opened, we can always close the door */
      getModel()->setTargetOrientation(0.0f, closedAngle, 0.0f);
      /* Call closing sound */
      Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z, SOUND_NO_LOOP,
            "sndfx/objects/door_close.ogg");
      return true;
   }

   /* Couldn't open or close. */
   return false;
}

