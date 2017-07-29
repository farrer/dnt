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
#include <kobold/kstring.h>
#include <kobold/parallelprocess.h>
#include <SDL2/SDL.h>
#include "scriptcontroller.h"
#include "scriptinstance.h"
#include "mapscript.h"

/*! How much (in ms) should sleep before each step on ScriptManager */
#define SCRIPT_UPDATE_TIME   100

namespace DNT
{
   /*! The manager of all DNT scripts. */
   class ScriptManager : public Kobold::ParallelProcess
   {
      public:
         /*! Constructor */
         ScriptManager();
         /*! Destructor */
         ~ScriptManager();

         /*! Create an instance of a map script.
          * \param filename name of the script to load
          * \param mapFilename name of the map
          * \return new instance of the loaded script */
         MapScriptInstance* createMapScriptInstance(
               Kobold::String filename, Kobold::String mapFilename);

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

         /*! Remove all unused script objects */
         void removeAllUnusedScriptObjects();

         /*! Get a context from the pool to execute a function. */
         asIScriptContext* prepareContextFromPool(asIScriptFunction* f);
         /*! Return a context to the pool, after execution */
         void returnContextToPool(asIScriptContext* ctx);
         /*! Execute a prepared function call from context 
          * \param ctx the context to use
          * \param instance script instance where to call. Should only be NULL
          *        when calling the factory function (which will create an 
          *        instance itself)
          * \param maxTime maximum execution time (in ms) before suspend. */
         int executeCall(asIScriptContext* ctx, ScriptInstance* instance, 
               int maxTime=2000);

         /*! \return used script engine */
         asIScriptEngine* getScriptEngine() { return asEngine; };
       
         /*! Callback for each line execution. Used for timeouts */
         static void lineCallback(asIScriptContext* ctx, Uint8* timeout);

         /*! Callback for receiving error messages from the scripts */
         void messageCallback(const asSMessageInfo& msg);

         /*! Do a step on the script manager (should only be called by our
          * script thread) */
         bool step();
         /*! \return time (in ms) to sleep between each cycle */
         unsigned int getSleepTime() { return SCRIPT_UPDATE_TIME; };

         /*! Suspend the current on-procces script until a pending 
          * action finishes.
          * \note Should only be called inside a functions called by the 
          *       running script. It's an erro otherwise. 
          * \param pendingAction pointer to the pending action to wait. */
         void suspendByPendingAction(PendingAction* pendingAction);
 
         /*! Search for a already create ScriptObject with filename and 
          * position and set its related game pointer.
          * \return pointer to the ScriptObject found, if any */
         ScriptObject* getAndDefinePointer(Kobold::String filename,
               const Ogre::Vector3 pos, void* newPtr);

         /* Global Functions */

         /*! Play a sound */
         void playSound(float x, float y, float z, Kobold::String file);
         /*! Sleep for at least some seconds */
         void sleep(int seconds);
         /*! Print a message on screen */
         void print(Kobold::String s);
         /*! \return a character by its name and position */
         ScriptObjectCharacter* getCharacter(Kobold::String filename, 
               float x, float y, float z);
         /*! \return a character by its name */
         ScriptObjectCharacter* getCharacterByFilename(Kobold::String filename);

      protected:
         /*! Get from already loaded controllers or load a new one, based
          * on its type and filename.
          * \param type type of the controller to get/load
          * \param filename filename of the controller to get/load
          * \return pointer to the controller or NULL if couldn't load */
         ScriptController* getOrLoadController(
               ScriptController::ScriptType type, Kobold::String filename);

      private:
         asIScriptEngine* asEngine; /**< The AngelScript engine */

         std::vector<asIScriptContext*> contexts; /**< Context pool */
         Kobold::List controllers; /**< List of ScriptControllers */
         Kobold::List instances; /**< List of ScriptInstances */
         Kobold::List objects; /**< List of ScriptObjects */
         Kobold::Mutex managerMutex; /**< Mutex for access */
         ScriptInstance* currentOnStep; /**< Current onStep instance */
         ScriptInstance* curRunningInstance; /**< Current running instance */
         asIScriptContext* curRunningContext; /**< Current running context */
   };
}

#endif

