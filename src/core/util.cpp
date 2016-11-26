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

#include "util.h"
#include <OGRE/OgreMath.h>
using namespace DNT;

/*************************************************************************
 *                              getAngle                                 *
 *************************************************************************/
float Util::getAngle(float p1X, float p1Z, float p2X, float p2Z)
{
   float ax = Ogre::Math::Abs(p1X - p2X);
   float az = Ogre::Math::Abs(p1Z - p2Z);
   float alpha;

   if( (ax != 0) )
   {
      alpha = ( (atan(fabs(az / ax)) / M_PI) * 180);
      if( (p1X > p2X) && (p1Z < p2Z) )
      {
         alpha += 180;
      }
      if( (p1X < p2X) && (p1Z < p2Z) )
      {
         alpha = -alpha;
      }
      if( (p1X > p2X) && (p1Z > p2Z) )
      {
         alpha = 180-alpha;
      }
      if(az == 0)
      {
         if(p1X < p2X)
         {
            alpha = 0;
         }
         else
         {
            alpha = 180;
         }
      }

      return (alpha - 90); /* -90 to correct model orientation*/

   }
   else
   {
      /* alpha == 0 or 180 or no one!*/
      if(p1Z < p2Z)
      {
         return 180;
      }
      else
      {
         return 0;
      }
   }

   /* Shouldn't here */
   return 0;
}

