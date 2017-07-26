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
#include "scriptobject.h"
#include "scriptobjectcharacter.h"
#include "pendingaction.h"
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

   currentOnStep = NULL;
   curRunningInstance = NULL;
   curRunningContext = NULL;

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "Initing ScriptManager...");

   /* Create the Engine and set its message and line callbacks */
   asEngine = asCreateScriptEngine();
   asEngine->SetEngineProperty(asEP_DISALLOW_GLOBAL_VARS, true);
   r = asEngine->SetMessageCallback(asMETHOD(ScriptManager, 
            messageCallback), this, asCALL_THISCALL); 
   assert(r >= 0);

   /* Register the default string type */
   RegisterStdString(asEngine);

   /* Register our classes */
   ScriptObjectCharacter::doScriptRegister(asEngine);

   /* Register our 'global' functions */
   r = asEngine->RegisterGlobalFunction(
         "void playSound(float x, float y, float z, string file)",
         asMETHOD(ScriptManager, playSound), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void sleep(int seconds)",
         asMETHOD(ScriptManager, sleep), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);

   /* Functions to get character from game */
   r = asEngine->RegisterGlobalFunction(
         "Character @+ getCharacter(string file, float x, float y, float z)",
         asMETHOD(ScriptManager, getCharacter), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "Character @+ getCharacter(string file)",
         asMETHOD(ScriptManager, getCharacterByFilename), 
         asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);

   /* Register our base interfaces */
   r = asEngine->RegisterInterface("MapController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("MissionController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("ThingController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("CharacterController");
   assert(r >= 0);
   
   /* Start our thread */
   createThread();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptManager::~ScriptManager()
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "Finishing ScriptManager...");

   /* Stop our thread, if active */
   if(isRunning())
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tWaiting thread to end...");
      endThread();
   }

   /* We should remove all instances and controllers to decrement
    * all references before exiting the AngelScript engine. */
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tClearing active instances...");
   instances.clear();
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tClearing controllers...");
   controllers.clear();
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tClearing ScriptObjects...");
   objects.clear();

   /* Release all created contexts */
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tReleasing context pool...");
   while(contexts.size())
   {
      /* Get avaiable context form pool */
      asIScriptContext* ctx = *contexts.rbegin();
      contexts.pop_back();
      ctx->Release();
   }

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "\tShuting down AngelScript engine...");
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
   currentOnStep = static_cast<ScriptInstance*>(instances.getFirst());
   int total = instances.getTotal();
   managerMutex.unlock();

   /* Note: at worst case, we'll step more than once one script,
    * as we kept the total and the list could had changed. */

   for(int i = 0; i < total; i++)
   {
      currentOnStep->step();

      /* Get next instance to step */
      managerMutex.lock();
      currentOnStep = static_cast<ScriptInstance*>(currentOnStep->getNext());
      managerMutex.unlock();
   }

   /* No more steping any instance, must unset the current. */
   managerMutex.lock();
   currentOnStep = NULL;
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
   Kobold::String strLevel = "ERROR";
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
int ScriptManager::executeCall(asIScriptContext* ctx, 
      ScriptInstance* instance, int maxTime)
{
   /* Note: we are only allowing a single instance to be executed at the
    * same time. This way we can treat and suspend context on pending action
    * functions, without needing to check each thread called the function. */
   managerMutex.lock();

   /* Set current running pointers */
   curRunningInstance = instance;
   curRunningContext = ctx;

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

   /* Clear current running */
   curRunningInstance = NULL;
   curRunningContext = NULL;

   managerMutex.unlock();
   return r;
}

/**************************************************************************
 *                          getOrLoadController                           *
 **************************************************************************/
ScriptController* ScriptManager::getOrLoadController(
      ScriptController::ScriptType type, Kobold::String filename)
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
      Kobold::String filename, Kobold::String mapFilename)
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
   while(currentOnStep == instance);
   
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
   int r = executeCall(ctx, instance);
   if(r == asEXECUTION_SUSPENDED)
   {
      /* Do not end execution, keep the context and resume latter
       * on next step for this instance. */
      instance->addSuspendedContext(ctx, NULL);
   }
   else
   {
      returnContextToPool(ctx);
   }
}


/**************************************************************************
 *                                playSound                               *
 **************************************************************************/
void ScriptManager::playSound(float x, float y, float z, Kobold::String file)
{
   Kosound::Sound::addSoundEffect(x, y, z, -1, file);
}

/**************************************************************************
 *                                  sleep                                 *
 **************************************************************************/
void ScriptManager::sleep(int seconds)
{
   curRunningInstance->addSuspendedContext(curRunningContext, 
         new PendingActionSleep(seconds));

   /* Suspend current execution */
   curRunningContext->Suspend();
}

/**************************************************************************
 *                         getAndDefinePointer                            *
 **************************************************************************/
ScriptObject* ScriptManager::getAndDefinePointer(Kobold::String filename,
      const Ogre::Vector3 pos, void* newPtr)
{
   ScriptObject* res = NULL;
   managerMutex.lock();
   ScriptObject* cur = static_cast<ScriptObject*>(objects.getFirst());
   for(int i = 0; i < objects.getTotal(); i++)
   {
      if(cur->isEquivalent(filename, pos))
      {
         cur->setPointer(newPtr);
         res = cur;
         break;
      }
      cur = static_cast<ScriptObject*>(cur->getNext());
   }
   managerMutex.unlock();
   return res;
}

/**************************************************************************
 *                             getCharacter                               *
 **************************************************************************/
ScriptObjectCharacter* ScriptManager::getCharacter(Kobold::String filename, 
      float x, float y, float z)
{
   ScriptObjectCharacter* res = NULL;
   Ogre::Vector3 pos(x, y, z);
   managerMutex.lock();
   ScriptObject* cur = static_cast<ScriptObject*>(objects.getFirst());
   for(int i = 0; i < objects.getTotal(); i++)
   {
      if((cur->getType() == ScriptObject::TYPE_CHARACTER) &&
         (cur->isEquivalent(filename, pos)))
      {
         /* Found */
         res = static_cast<ScriptObjectCharacter*>(cur);
         break;
      }
      cur = static_cast<ScriptObject*>(cur->getNext());
   }

   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectCharacter(filename, pos);
      objects.insert(res);
   }

   managerMutex.unlock();
   return res;
}

/**************************************************************************
 *                             getCharacter                               *
 **************************************************************************/
ScriptObjectCharacter* ScriptManager::getCharacterByFilename(Kobold::String 
      filename)
{
   ScriptObjectCharacter* res = NULL;
   managerMutex.lock();
   ScriptObject* cur = static_cast<ScriptObject*>(objects.getFirst());
   for(int i = 0; i < objects.getTotal(); i++)
   {
      if((cur->getType() == ScriptObject::TYPE_CHARACTER) &&
         (cur->isEquivalent(filename)))
      {
         /* Found */
         res = static_cast<ScriptObjectCharacter*>(cur);
         break;
      }
      cur = static_cast<ScriptObject*>(cur->getNext());
   }

   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectCharacter(filename);
      objects.insert(res);
   }

   managerMutex.unlock();
   return res;

}

