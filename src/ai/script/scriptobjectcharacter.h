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

#ifndef _dnt_script_object_character_h
#define _dnt_script_object_character_h

#include "dntconfig.h"
#include "scriptobject.h"
#include "angelscript.h"

namespace DNT
{

   /*! A ScriptObject implementation for characters */
   class ScriptObjectCharacter : public ScriptObject
   {
      public:

         /*! Register the object and its functions for script usage.
          * \note should only be called by ScriptManager */
         static void doScriptRegister(asIScriptEngine* asEngine);

         /*! Constructor */
         ScriptObjectCharacter(Kobold::String filename, 
               Ogre::Vector3 originalPos);
         ScriptObjectCharacter(Kobold::String filename);
         /*! Destructor */
         ~ScriptObjectCharacter();
        
         /*! Set the character pointer.
          * \param newPtr pointer to the character this object 
          *               represents, or NULL. */
         void setPointer(void* newPtr);

         void moveToPosition(float posX, float posZ);
         void moveToCharacter(ScriptObjectCharacter* character);
         void moveFromCharacter(ScriptObjectCharacter* character);
         void callAnimation(int index);
         void setAnimation(int index);
         void openDialog();

     protected:
         /*! \return pointer to the referenced character */
         const void* getPointer() const { return character;};

     private:
         Character* character; /**< referenced character */

   };

}

#endif

