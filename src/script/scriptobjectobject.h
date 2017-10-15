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

#ifndef _dnt_script_object_object_h
#define _dnt_script_object_object_h

#include "dntconfig.h"
#include "scriptobject.h"
#include "angelscript.h"
#include <kobold/mutex.h>

namespace DNT
{

   /*! A ScriptObject implementation for Objects (ok, the name became
    * something horrible, but... on script it's only Object and this way
    * they have the same pattern as other ScriptObjects) */
   class ScriptObjectObject : public ScriptObject
   {
      public:

         /*! Register the related class */
         static void registerClass(asIScriptEngine* asEngine);
         /*! Register the related functions */
         static void registerFunctions(asIScriptEngine* asEngine);

         /*! Constructor */
         ScriptObjectObject(Kobold::String filename, 
               Ogre::Vector3 originalPos);
         /*! Constructor */
         ScriptObjectObject(Kobold::String filename);
         /*! Destructor */
         ~ScriptObjectObject();
        
         /*! Set the object pointer.
          * \param newPtr pointer to the object this object 
          *               represents, or NULL. */
         void setPointer(void* newPtr);
         
         /*! \return if the related pointer is valid */
         const bool isValid();
         
         /*! Get the current position */
         const Ogre::Vector3 getPosition();
         /*! Set position of the object on next frame */
         void setPosition(float x, float y, float z);

         /*! Set the orientation of the object on next frame */
         void setOrientation(float ori);


     private:
         Object* object; /**< referenced object */
         Kobold::Mutex mutex; /**< For pointer access changes */

   };

}

#endif
