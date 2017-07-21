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


#include "scriptmanager.h"
#include "scriptstdstring.h"
#include <kobold/log.h>
#include <kosound/sound.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptManager::ScriptManager()
{
   int r;

   current = NULL;

   /* Create the Engine and set its message and line callbacks */
   asEngine = asCreateScriptEngine();
   r = asEngine->SetMessageCallback(asMETHOD(ScriptManager, 
            messageCallback), this, asCALL_THISCALL); 
   assert(r >= 0);

   /* Register the default string type */
   RegisterStdString(asEngine);

   /* Register our 'global' functions */
   r = asEngine->RegisterGlobalFunction(
         "void playSound(float x, float y, float z, string file)",
         asMETHOD(ScriptManager, playSound), asCALL_THISCALL_ASGLOBAL, 
         this);
   assert(r >= 0);

   /* TODO: Register our classes */
   r = asEngine->RegisterInterface("MapController");

   /* Start our thread */
   createThread();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptManager::~ScriptManager()
{
   /* Stop our thread, if active */
   if(isRunning())
   {
      endThread();
   }

   /* We should remove all instances and controllers to decrement
    * all references before exiting the AngelScript engine. */
   instances.clear();
   controllers.clear();

   if(asEngine)
   {
      asEngine->ShutDownAndRelease();
   }
}

/**************************************************************************
 *                                  step                                  *
 **************************************************************************/
bool ScriptManager::step()
{
   managerMutex.lock();
   current = static_cast<ScriptInstance*>(instances.getFirst());
   int total = instances.getTotal();
   managerMutex.unlock();

   /* Note: at worst case, we'll step more than once one script,
    * as we kept the total and the list could had changed. */

   for(int i = 0; i < total; i++)
   {
      //TODO: check if script is waiting a pending action,
      //or if is suspended.

      /* Run its step method. */
      if(current->getScript()->getStepFunction())
      {
         callFunction(current, current->getScript()->getStepFunction()); 
      }

      /* Get next instance to step */
      managerMutex.lock();
      current = static_cast<ScriptInstance*>(current->getNext());
      managerMutex.unlock();
   }

   /* No more steping any instance, must unset the current. */
   managerMutex.lock();
   current = NULL;
   managerMutex.unlock();

   return true;
}

/**************************************************************************
 *                             messageCallback                            *
 **************************************************************************/
void ScriptManager::messageCallback(const asSMessageInfo& msg)
{
   /* Define log level type */
   Kobold::Log::LogLevel level = Kobold::Log::LOG_LEVEL_ERROR;
   Ogre::String strLevel = "ERROR";
   if(msg.type == asMSGTYPE_WARNING)
   {
      level = Kobold::Log::LOG_LEVEL_NORMAL;
      strLevel = "WARN";
   }
   else if(msg.type == asMSGTYPE_INFORMATION)
   {
      level = Kobold::Log::LOG_LEVEL_DEBUG;
      strLevel = "INFO";
   }
   
   /* Log the message */
   Kobold::Log::add(level, "script [%s]: %s (%d, %d) : %s", 
         strLevel.c_str(), msg.section, msg.row, msg.col, msg.message);
}

/**************************************************************************
 *                         prepareContextFromPool                         *
 **************************************************************************/
asIScriptContext* ScriptManager::prepareContextFromPool(
      asIScriptFunction* f)
{
   asIScriptContext* ctx = NULL;
   managerMutex.lock();
   if(contexts.size())
   {
      /* Get avaiable context form pool */
      ctx = *contexts.rbegin();
      contexts.pop_back();
   }
   else
   {
      /* Create a new context */
      ctx = asEngine->CreateContext();
   }
   managerMutex.unlock();

   /* Prepare the context */
   int r = ctx->Prepare(f);
   assert(r >= 0);

   return ctx;
}

/**************************************************************************
 *                          returnContextToPool                           *
 **************************************************************************/
void ScriptManager::returnContextToPool(asIScriptContext* ctx)
{
   assert(ctx != NULL);
   managerMutex.lock();
   contexts.push_back(ctx);
   managerMutex.unlock();

   ctx->Unprepare();
}

/**************************************************************************
 *                            lineCallback                                *
 **************************************************************************/
void ScriptManager::lineCallback(asIScriptContext* ctx, Uint8* timeout)
{
   if((*timeout) > SDL_GetTicks())
   {
      ctx->Suspend();
   }
}

/**************************************************************************
 *                               executeCall                              *
 **************************************************************************/
int ScriptManager::executeCall(asIScriptContext* ctx, int maxTime)
{

   /* Set our line-time-out check function */
   Uint8 timeout = SDL_GetTicks() + maxTime;
   if(maxTime > 0)
   {
      ctx->SetLineCallback(asFUNCTION(ScriptManager::lineCallback), &timeout, 
            asCALL_CDECL);
   }

   /* Execute the script */
   int r = ctx->Execute();

   /* Check end result */
   if(r != asEXECUTION_FINISHED)
   {
      if(r == asEXECUTION_EXCEPTION)
      {
         //FIXME: Shouldn't show script name?
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "ERROR: Script Exception: '%s' at function '%s' line %d.",
               ctx->GetExceptionString(), 
               ctx->GetExceptionFunction()->GetDeclaration(),
               ctx->GetExceptionLineNumber());
      }
   }

   return r;
}

/**************************************************************************
 *                          getOrLoadController                           *
 **************************************************************************/
ScriptController* ScriptManager::getOrLoadController(
      ScriptController::ScriptType type, Ogre::String filename)
{
   ScriptController* ctrl = static_cast<ScriptController*>(
         controllers.getFirst());
   for(int i=0; i < controllers.getTotal(); i++)
   {
      if(ctrl->getFilename() == filename)
      {
         /* Found */
         assert(ctrl->getType() == type);
         return ctrl;
      }
      ctrl = static_cast<ScriptController*>(ctrl->getNext());
   }

   /* Not loaded yet, must load it. */
   switch(type)
   {
      case ScriptController::SCRIPT_TYPE_MAP:
         ctrl = new MapScript(this);
      break;
      default:
         ctrl = NULL;
      break;
   }
   assert(ctrl != NULL);
   if(ctrl->loadAndCompile(filename))
   {
      /* Loaded, return it */
      controllers.insert(ctrl);
   }
   else
   {
      /* Failed to load or compile, must not use it */
      delete ctrl;
      ctrl = NULL;
   }
   
   return ctrl;
}
      
/**************************************************************************
 *                        createMapScriptInstance                         *
 **************************************************************************/
MapScriptInstance* ScriptManager::createMapScriptInstance(
      Ogre::String filename, Ogre::String mapFilename)
{
   /* Load or get already loaded script controller */
   MapScript* ctrl = static_cast<MapScript*>(getOrLoadController(
            ScriptController::SCRIPT_TYPE_MAP, filename));
   if(!ctrl)
   {
      /* Couldn't load or compile the script, no instance will be created. */
      return NULL;
   }

   /* Create a new instance of the script */
   MapScriptInstance* res = ctrl->createInstance(mapFilename);
   if(res)
   {
      managerMutex.lock();
      /* Add to our instances list */
      instances.insert(res);
      managerMutex.unlock();
   }

   return res;
}

/**************************************************************************
 *                             removeInstance                             *
 **************************************************************************/
void ScriptManager::removeInstance(ScriptInstance* instance)
{
   /* Must wait until not executing the instance */
   while(current == instance);
   
   managerMutex.lock();
   instances.remove(instance);
   managerMutex.unlock();
}

/**************************************************************************
 *                       removeAllUnusedControllers                       *
 **************************************************************************/
void ScriptManager::removeAllUnusedControllers()
{
   /* Check through the controller's list which ones are no more in use */
   int total = controllers.getTotal();
   ScriptController* ctrl = static_cast<ScriptController*>(
         controllers.getFirst());
   for(int i=0; i < total; i++)
   {
      if(ctrl->getNumberOfInstances() == 0)
      {
         ScriptController* aux = ctrl;
         ctrl = static_cast<ScriptController*>(ctrl->getNext());
         controllers.remove(aux);
      }
      else
      {
         ctrl = static_cast<ScriptController*>(ctrl->getNext());
      }
   }
}

/**************************************************************************
 *                             callFunction                               *
 **************************************************************************/
void ScriptManager::callFunction(ScriptInstance* instance, 
      asIScriptFunction* function)
{
   asIScriptContext* ctx = prepareContextFromPool(function);
   ctx->SetObject(instance->getObject());
   int r = executeCall(ctx);
   if(r == asEXECUTION_SUSPENDED)
   {
      //TODO: must not end execution, keep the context and resume it on next
      //      step.
   }
   returnContextToPool(ctx);
}


/**************************************************************************
 *                                playSound                               *
 **************************************************************************/
void ScriptManager::playSound(float x, float y, float z, Ogre::String file)
{
   Kosound::Sound::addSoundEffect(x, y, z, -1, file);
}

