/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
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

#include "agent.h"
using namespace DNT;

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
Agent::Agent(bool oriented)
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
   withOrientation = oriented;
   orientation = 0;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
Agent::~Agent()
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
}

/********************************************************************
 *                     Define Step Size                             *
 ********************************************************************/
void Agent::defineStepSize(float size)
{
   stepSize = size;
}

/********************************************************************
 *                          Define Destiny                          *
 ********************************************************************/
void Agent::defineDestiny(float x, float z)
{
   destinyX = x;
   destinyZ = z;
}

/********************************************************************
 *                          Define Position                         *
 ********************************************************************/
void Agent::definePosition(float x, float z)
{
   actualX = x;
   actualZ = z;
}

/********************************************************************
 *                           Get Position                           *
 ********************************************************************/
void Agent::getPosition(float& x, float& z)
{
   x = actualX;
   z = actualZ;
}

/********************************************************************
 *                             update                               *
 ********************************************************************/
void Agent::update()
{
   defineNextPosition(false, 1.0f);
}

/********************************************************************
 *                             doAngle                              *
 ********************************************************************/
bool Agent::doAngle()
{
   /* Must put at range before check */
   while(desiredAngle < 0)
   {
      desiredAngle += 360;
   }
   while(orientation < 0)
   {
      orientation += 360;
   }

   /* Now recalculate, if are differents */
   if(orientation != desiredAngle)
   {
      float d1;
      float d2;
      if(orientation > desiredAngle)
      {
         d1 = (orientation - desiredAngle);
         d2 = (360 - orientation + desiredAngle);
      }
      else
      {
         d2 = (desiredAngle - orientation);
         d1 = (360 - desiredAngle + orientation);
      }

      if(d1 < d2)
      {
         orientation = desiredAngle;
      }
      else
      {
         orientation = desiredAngle;
      }

      while(orientation < 0)
      {
         orientation += 360;
      }
      
      return true;
   }
   return false;
}

/********************************************************************
 *                        define Bounding Box                       *
 ********************************************************************/
void Agent::defineBoundingBox(float xa, float za, float xb, float zb)
{
   x1 = xa;
   z1 = za;
   x2 = xb;
   z2 = zb;
}

/********************************************************************
 *                          get Bounding Box                        *
 ********************************************************************/
void Agent::getBoundingBox(float &xa, float &za, float &xb, float &zb)
{
   xa = x1;
   za = z1;
   xb = x2;
   zb = z2;
}

