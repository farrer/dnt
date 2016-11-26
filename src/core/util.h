
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



#ifndef _dnt_util_h
#define _dnt_util_h

namespace DNT
{

#define DNT_TO_METER   0.2f /**< Constant to multiply to convert DNT to meter.*/
#define METER_TO_DNT   1.0f / DNT_TO_METER /**< Constant to Meter to DNT value*/

   class Util
   {
      public:
         /*! Calculate angle between two points (p1 and p2) */
         static float getAngle(float p1X, float p1Z, float p2X, float p2Z);
   };

};

#endif

