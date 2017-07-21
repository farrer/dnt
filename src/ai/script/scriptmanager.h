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

#ifndef _dnt_script_manager_h
#define _dnt_script_manager_h

#include <angelscript.h>
#include <OGRE/OgreString.h>
#include <vector>
#include <kobold/list.h>
#include <SDL2/SDL.h>
#include "scriptcontroller.h"
#include "scriptinstance.h"
#include "mapscript.h"

namespace DNT
{
   /*! The manager of all DNT scripts. */
   class ScriptManager
   {
      public:
         /*! Constructor */
         ScriptManager();
         /*! Destructor */
         ~ScriptManager();

         /*! Create an instance of a map script.
          * \param type script type to load
          * \param filename name of the script to load
          * \param mapname name of the map
          * \return new instance of the loaded script */
         MapScriptInstance* createMapScriptInstance(
               Ogre::String filename, Ogre::String mapFilename);

         /*! Call an specific instance function.
          * \param instance to call an specific function.
          * \param function function to be called.
          * \note It's not certain that the function will end its execution
          *       until this function returns. Often, specially when 
          *       pending-actions function types are executed, this
          *       will return before the end, which could happen
          *       some cycles latter. */
         void callFunction(ScriptInstance* instance, 
               asIScriptFunction* function); 

         /*! Remove an instance, clearing all its related memmory,
          * but keeping alive its controller for future use */
         void removeInstance(ScriptInstance* instance);

         /*! Remove all unused controllers, to free up some momory.
          * Usually called after a previous area (not map) is unloaded
          * and a new area is loaded on its place. */
         void removeAllUnusedControllers();

         /*! Get a context from the pool to execute a function. */
         asIScriptContext* prepareContextFromPool(asIScriptFunction* f);
         /*! Return a context to the pool, after execution */
         void returnContextToPool(asIScriptContext* ctx);
         /*! Execute a prepared function call from context */
         int executeCall(asIScriptContext* ctx, int maxTime=2000);

         asIScriptEngine* getScriptEngine() { return asEngine; };

         void playSound(float x, float y, float z, Ogre::String file);
        
         //TODO: update function: to call update function on script instances. 
         
         static void lineCallback(asIScriptContext* ctx, Uint8* timeout);

      protected:
         /*! Callback for receiving error messages from the scripts */
         void messageCallback(const asSMessageInfo& msg);

         ScriptController* getOrLoadController(
               ScriptController::ScriptType type, Ogre::String filename);

      private:
         asIScriptEngine* asEngine;

         std::vector<asIScriptContext*> contexts; /**< Context pool */
         Kobold::List controllers; /**< List of ScriptControllers */
         Kobold::List instances; /**< List of ScriptInstances */
   };
}

#endif

