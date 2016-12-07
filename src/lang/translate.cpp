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


#include "translate.h"

namespace DNT
{

/************************************************************
 *                     translateDataString                  *
 ************************************************************/
Kobold::String translateDataString(Kobold::String dataString)
{
   if( (dataString.length() > 10) && (dataString[0] == 'g') &&
       (dataString[1] == 'e') && (dataString[2] == 't') && 
       (dataString[3] == 't') && (dataString[4] == 'e') &&
       (dataString[5] == 'x') && (dataString[6] == 't') ) 
   {
      /* Is a translatable string */

      /* Verify and remove \n or DOS carriage at the end */
      if( (dataString[dataString.length()-1] == '\n') ||
          (dataString[dataString.length()-1] == 13) )
      {
         dataString.erase(dataString.length()-1);
      }

      /* Remove gettext(" */
      dataString.erase(0,9);
      /* Remove ") */
      dataString.erase(dataString.length()-2);

      return gettext(dataString.c_str());
   }

   /* Not a translatable one */
   return dataString;
}

}

