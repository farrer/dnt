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

#ifndef _dnt_module_script_h
#define _dnt_module_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"

namespace DNT
{
   /*! Instance of a ModuleScript */
   class ModuleScriptInstance : public ScriptInstance 
   {
      public:
         /*! Constructor */
         ModuleScriptInstance(asIScriptObject* obj, 
               ModuleScript* script, ScriptManager* manager); 
         /*! Destructor */
         ~ModuleScriptInstance();

         /*! \return semicolon separated string with all folders to init */
         Kobold::String getBaseFolders();

         /*! \return filename of the SkinController script to use */
         Kobold::String getSkinFilename();
         /*! \return filename of the Rule definition to use */
         Kobold::String getRulesFilename();
         /*! \return filename of the initial map to load */
         Kobold::String getInitialMap();
   };

   /*! A script for controlling Farso's widgets defined by a JSON file. */
   class ModuleScript : public ScriptController
   {
      public:
         ModuleScript(ScriptManager* manager);
         ~ModuleScript();

         /*! \return a new instance of the script */
         ModuleScriptInstance* createInstance();
         
         asIScriptFunction* getFactoryFunction();
         asIScriptFunction* getStepFunction();

         asIScriptFunction* getBaseFoldersFunction();
         asIScriptFunction* getSkinFilenameFunction();
         asIScriptFunction* getRulesFilenameFunction();
         asIScriptFunction* getInitialMapFunction();

      protected:
         void setFunctionPointers();
         const Ogre::String getMainTypeInterface() const 
         { 
            return "ModuleController";
         };

      private:
         Kobold::String loadFile(const Kobold::String& filename);
         asIScriptFunction* factoryFunction;
         asIScriptFunction* stepFunction;
         asIScriptFunction* baseFoldersFunction;
         asIScriptFunction* rulesFilenameFunction;
         asIScriptFunction* skinFilenameFunction;
         asIScriptFunction* initialMapFunction;
   };

}

#endif
