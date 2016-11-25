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

#ifndef _dnt_translate_h
#define _dnt_translate_h

#if ENABLE_NLS == 1
   #include <libintl.h>
#else
   #ifndef _gettext_func_
      #define _gettext_func_
      /* Define a gettext ignore function */
      inline char* gettext(const char* s){return((char*)s);};
   #endif
#endif

#include <OGRE/OgreString.h>

/*! Translate a data string readed from a data file.
 * \param dataString -> translatable data string readed from a data file 
 * \return -> translated string for the input data. */
Ogre::String translateDataString(Ogre::String dataString);


#endif

