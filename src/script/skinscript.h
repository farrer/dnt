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

#ifndef _dnt_skin_script_h
#define _dnt_skin_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"

namespace DNT
{
   /*! Instance of a SkinScript */
   class SkinScriptInstance : public ScriptInstance
   {
      public:
         /*! Constructor */
         SkinScriptInstance(asIScriptObject* obj, 
               SkinScript* script, ScriptManager* manager);
         /*! Destructor */
         ~SkinScriptInstance();

         /*! \return the .skin definition filename to use */
         Kobold::String getSkinFilename();

         /*! \return string identifier of the ScrollText to be populated
          * with game log. */
         Kobold::String getLogScrollTextId();

         /*! \return total SkinElements defined by the script */
         int getTotalElements();
         /*! \return index of the SkinElement [0, total) for the element 
          *  of defined name */
         int getElementType(const Kobold::String& name);
   };

   /*! A script for GUI skin definitions. */
   class SkinScript : public ScriptController
   {
      public:
         SkinScript(ScriptManager* manager);
         ~SkinScript();

         /*! \return a new instance of the script */
         SkinScriptInstance* createInstance();
         
         /*! \return pointer to the constructor function */
         asIScriptFunction* getFactoryFunction();
         /*! \return pointer to the function called on every cycle */
         asIScriptFunction* getStepFunction();
         /*! \return pointer to be receive the total new Skin elements 
          * defined */
         asIScriptFunction* getTotalElementsFunction();
         /*! \return pointer to the function called to parse a skin element
          * defined by this skin */
         asIScriptFunction* getElementTypeFunction();
         /*! \return pointer to the function called to get the skin filename
          * to use */
         asIScriptFunction* getSkinFilenameFunction();

         /*! \return pointer to the function called to get the identifier
          * of the game log ScrollText */
         asIScriptFunction* getLogScrollTextIdFunction();

      protected:
         void setFunctionPointers();
         const Ogre::String getMainTypeInterface() const 
         { 
            return "SkinController";
         };

      private:
         asIScriptFunction* factoryFunction;
         asIScriptFunction* stepFunction;
         asIScriptFunction* totalElementsFunction;
         asIScriptFunction* elementTypeFunction;
         asIScriptFunction* skinFilenameFunction;
         asIScriptFunction* logScrollTextIdFunction;
   };

}

#endif

