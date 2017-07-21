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

#ifndef _dnt_map_script_h
#define _dnt_map_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"


namespace DNT
{

/*! Instance of a MapScript */
class MapScriptInstance: public ScriptInstance
{
   public:
      MapScriptInstance(asIScriptObject* obj, MapScript* script, 
            ScriptManager* manager);
      ~MapScriptInstance();

      void callOnEnter();

      void callOnLoad();

      void callOnExit();
};

/*! Script specific for maps. */
class MapScript : public ScriptController
{
   public:
      MapScript(ScriptManager* manager);
      ~MapScript();
      
      MapScriptInstance* createInstance(Ogre::String mapname);

      /*! \return pointer to the constructor function */
      asIScriptFunction* getFactoryFunction();
      /*! \return pointer to the function called on every cycle */
      asIScriptFunction* getUpdateFunction();
      /*! \return pointer to the function called when map finished to load */
      asIScriptFunction* getOnLoadFunction();
      /*! \return pointer to the function called when PC entered the map */
      asIScriptFunction* getOnEnterFunction();
      /*! \return pointer to the function called when PC exited the map */
      asIScriptFunction* getOnExitFunction();

   protected:
      void setFunctionPointers();
      const Ogre::String getMainTypeInterface() const {return "MapController";};

   private:

      asIScriptFunction* factoryFunction;
      asIScriptFunction* updateFunction;
      asIScriptFunction* onLoadFunction;
      asIScriptFunction* onEnterFunction;
      asIScriptFunction* onExitFunction;
};

}

#endif


