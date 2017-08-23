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

#ifndef _dnt_skin_h
#define _dnt_skin_h

#include "dntconfig.h"

#include <farso/skin.h>

namespace DNT
{
   /*! A Skin in DNT is a Farso::Skin extended by a script. This way the
    * user interface could be fully modable, even with different screen
    * elements. */
   class Skin : public Farso::Skin
   {
      public:
         /*! Constructor
          * \param scriptFilename filename of the SkinScript to use */
         Skin(const Kobold::String& scriptFilename);
         /*! Destructor */
         ~Skin();

      protected:
         /*! \return from script the total elements defined */
         int getTotalElements();
         /*! \return from script the extended Farso::SkinElement index */
         int getExtendedElementType(Kobold::String typeName);

      private:
         SkinScriptInstance* script; /**< Instance of the Skin Script */
   };

}


#endif

