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
#include "scriptobjectdice.h"
#include "scriptobjectobject.h"
#include "scriptobjectruledef.h"
#include "scriptobjectrulegroup.h"
#include "scriptobjectwidget.h"
#include "pendingaction.h"
#include "farsofunctions.h"
#include "../gui/briefing.h"
#include "../lang/translate.h"
#include "../core/game.h"
#include "../rules/character.h"

#include <kobold/log.h>
#include <kobold/ogre3d/ogrefilereader.h>
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

   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "Initing ScriptManager...");

   asPrepareMultithread();
   /* Create the Engine and set its message and line callbacks */
   asEngine = asCreateScriptEngine();
   asEngine->SetEngineProperty(asEP_DISALLOW_GLOBAL_VARS, true);
   asEngine->SetEngineProperty(asEP_AUTO_GARBAGE_COLLECT, false);
   r = asEngine->SetMessageCallback(asMETHOD(ScriptManager, 
            messageCallback), this, asCALL_THISCALL); 
   assert(r >= 0);

   /* Register the default string type */
   RegisterStdString(asEngine);

   /* Register our classes */
   ScriptObjectCharacter::registerClass(asEngine);
   ScriptObjectDice::registerClass(asEngine);
   ScriptObjectObject::registerClass(asEngine);
   ScriptObjectRuleDefinition::registerClass(asEngine);
   ScriptObjectRuleGroup::registerClass(asEngine);
   ScriptObjectWidget::registerClass(asEngine);

   /* Register classes functions */
   ScriptObjectCharacter::registerFunctions(asEngine);
   ScriptObjectDice::registerFunctions(asEngine);
   ScriptObjectObject::registerFunctions(asEngine);
   ScriptObjectRuleDefinition::registerFunctions(asEngine);
   ScriptObjectRuleGroup::registerFunctions(asEngine);
   ScriptObjectWidget::registerFunctions(asEngine);

   /* Register our 'global' functions */
   r = asEngine->RegisterGlobalFunction(
         "void playSound(float x, float y, float z, string file)",
         asMETHOD(ScriptManager, playSound), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r= asEngine->RegisterGlobalFunction(
        "void disablePcMoves()", asMETHOD(ScriptManager, disablePcMoves), 
        asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r= asEngine->RegisterGlobalFunction(
        "void enablePcMoves()", asMETHOD(ScriptManager, enablePcMoves), 
        asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void sleep(int seconds)",
         asMETHOD(ScriptManager, sleep), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void print(string s)",
         asMETHOD(ScriptManager, print), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void brief(string s)",
         asMETHOD(ScriptManager, brief), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "void brief(string s, int r, int g, int b)",
         asMETHOD(ScriptManager, briefColor), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "void briefLineBreak()",
         asMETHOD(ScriptManager, briefBreak), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "string gettext(string s)",
         asMETHOD(ScriptManager, translate), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);

   /* Functions to get character and object from game */
   r = asEngine->RegisterGlobalFunction(
         "Character @+ getCharacter(string file, float x, float y, float z)",
         asMETHOD(ScriptManager, getCharacter), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "Character @+ getCharacter(string file)",
         asMETHOD(ScriptManager, getCharacterByFilename), 
         asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "Character @+ getPlayableCharacter()",
         asMETHOD(ScriptManager, getFirstPlayableCharacter), 
         asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "Object @+ getObject(string file, float x, float y, float z)",
         asMETHOD(ScriptManager, getObject), asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "Object @+ getObject(string file)",
         asMETHOD(ScriptManager, getObjectByFilename), 
         asCALL_THISCALL_ASGLOBAL, this);
   assert(r >= 0);

   FarsoFunctions::registerFunctions(asEngine);

   /* Register our base interfaces */
   r = asEngine->RegisterInterface("MapController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("ModuleController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("RuleController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("RuleDefinitionController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("MissionController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("ObjectController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("CharacterController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("SkinController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("WidgetController");
   assert(r >= 0);
   r = asEngine->RegisterInterface("DialogWidgetController");
   assert(r >= 0);
   
   /* Start our thread */
   createThread();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptManager::~ScriptManager()
{
   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "Finishing ScriptManager...");

   /* Stop our thread, if active */
   if(isRunning())
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tWaiting thread to end...");
      endThread();
   }

   /* We should remove all instances and controllers to decrement
    * all references before exiting the AngelScript engine. */
   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tClearing active instances...");
   instances.clear();
   
   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tClearing controllers...");
   controllers.clear();

   /* Release all created contexts */
   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tReleasing context pool...");
   while(contexts.size() > 0)
   {
      /* Get avaiable context from pool */
      asIScriptContext* ctx = *contexts.rbegin();
      contexts.pop_back();
      ctx->Release();
   }

   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tRunning the garbage collector...");
   asEngine->GarbageCollect(asGC_FULL_CYCLE);

   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tClearing ScriptObjects...");
   objects.clear();

   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
         "\tShuting down AngelScript engine...");
   if(asEngine)
   {
      asEngine->ShutDownAndRelease();
   }
   asUnprepareMultithread();
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
   asEngine->GarbageCollect(asGC_ONE_STEP);
   managerMutex.unlock();

   return true;
}

/**************************************************************************
 *                             messageCallback                            *
 **************************************************************************/
void ScriptManager::messageCallback(const asSMessageInfo& msg)
{
   /* Define log level type */
   Kobold::LogLevel level = Kobold::LOG_LEVEL_ERROR;
   Kobold::String strLevel = "ERROR";
   if(msg.type == asMSGTYPE_WARNING)
   {
      level = Kobold::LOG_LEVEL_NORMAL;
      strLevel = "WARN";
   }
   else if(msg.type == asMSGTYPE_INFORMATION)
   {
      level = Kobold::LOG_LEVEL_NORMAL;
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
   assert(f != NULL);
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

   /* Unprepare the context before returning to pool, to avoid
    * someone get it before it was really 'freed'. */
   ctx->Unprepare();
   
   managerMutex.lock();
   contexts.push_back(ctx);
   managerMutex.unlock();
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
 *                           printVariables                               *
 **************************************************************************/
void ScriptManager::printVariables(asIScriptContext* ctx, asUINT stackLevel)
{
   Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, "\t\tVariables:"); 
   asIScriptEngine *engine = ctx->GetEngine();
   
   /* First print the this pointer if this is a class method */
   int typeId = ctx->GetThisTypeId(stackLevel);
   void *varPointer = ctx->GetThisPointer(stackLevel);
   if(typeId)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
            "\t\t this = 0x%x", varPointer);
   }

   /* Print the value of each variable, including parameters */
   int numVars = ctx->GetVarCount(stackLevel);
   for( int n = 0; n < numVars; n++ )
   {
      int typeId = ctx->GetVarTypeId(n, stackLevel); 
      void *varPointer = ctx->GetAddressOfVar(n, stackLevel);
      if(typeId == asTYPEID_INT32)
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
               "\t\t %s = %d", ctx->GetVarDeclaration(n, stackLevel), 
               *(int*)varPointer);
      }
      else if(typeId == asTYPEID_FLOAT)
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
               "\t\t %s = %f", ctx->GetVarDeclaration(n, stackLevel), 
               *(float*)varPointer);
      }
      else if( typeId & asTYPEID_SCRIPTOBJECT )
      {
         asIScriptObject *obj = (asIScriptObject*)varPointer;
         if(obj)
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
                  "\t\t %s = {...}", ctx->GetVarDeclaration(n, stackLevel));
         }
         else
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
                  "\t\t %s = <null>", ctx->GetVarDeclaration(n, stackLevel));
         }
      }
      else if(typeId == engine->GetTypeIdByDecl("string"))
      {
         Kobold::String* str = static_cast<Kobold::String*>(varPointer);
         if(str)
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
                  "\t\t %s = '%s'", ctx->GetVarDeclaration(n, stackLevel), 
                  str->c_str());
         }
         else
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
                  "\t\t %s = <null>", ctx->GetVarDeclaration(n, stackLevel));
         }
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
               "\t\t %s = {...}", ctx->GetVarDeclaration(n, stackLevel));
      }
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
   runningInfo.push(new RunningInfo(instance, ctx));

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
         const asIScriptFunction *function = ctx->GetExceptionFunction();
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
               "ERROR: Script '%s' Exception: '%s' at function '%s' line %d.",
               function->GetModuleName(),
               ctx->GetExceptionString(), 
               function->GetDeclaration(),
               ctx->GetExceptionLineNumber());
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, "\tCallstack:");
         for( asUINT n = 0; n < ctx->GetCallstackSize(); n++ )
         {
            asIScriptFunction *func;
            const char *scriptSection;
            int line, column;
            func = ctx->GetFunction(n);
            line = ctx->GetLineNumber(n, &column, &scriptSection);
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
                  "\t%s:%d,%d", func->GetDeclaration(),
                  line, column);
            printVariables(ctx, n);
         }
      }
   }
   assert(r >= 0);

   /* Clear current running */
   RunningInfo* info = runningInfo.top();
   runningInfo.pop();
   delete info;

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
      case ScriptController::SCRIPT_TYPE_MODULE:
         ctrl = new ModuleScript(this);
      break;
      case ScriptController::SCRIPT_TYPE_RULE:
         ctrl = new RuleScript(this);
      break;
      case ScriptController::SCRIPT_TYPE_RULE_DEFINITION:
         ctrl = new RuleDefinitionScript(this);
      break;
      case ScriptController::SCRIPT_TYPE_SKIN:
         ctrl = new SkinScript(this);
      break;
      case ScriptController::SCRIPT_TYPE_WIDGET:
         ctrl = new WidgetScript(this);
      break;
      case ScriptController::SCRIPT_TYPE_DIALOG_WIDGET:
         ctrl = new DialogWidgetScript(this);
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
      const Kobold::String& filename, const Kobold::String& mapFilename)
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
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                       createModuleScriptInstance                       *
 **************************************************************************/
ModuleScriptInstance* ScriptManager::createModuleScriptInstance(
      const Kobold::String& filename)
{
   /* Load or get already loaded script controller */
   ModuleScript* ctrl = static_cast<ModuleScript*>(getOrLoadController(
            ScriptController::SCRIPT_TYPE_MODULE, filename));
   if(!ctrl)
   {
      /* Couldn't load or compile the script, no instance will be created. */
      return NULL;
   }

   /* Create a new instance of the script */
   ModuleScriptInstance* res = ctrl->createInstance();
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                        createRuleScriptInstance                        *
 **************************************************************************/
RuleScriptInstance* ScriptManager::createRuleScriptInstance(
      const Kobold::String& filename)
{
   /* Load or get already loaded script controller */
   RuleScript* ctrl = static_cast<RuleScript*>(getOrLoadController(
            ScriptController::SCRIPT_TYPE_RULE, filename));
   if(!ctrl)
   {
      /* Couldn't load or compile the script, no instance will be created. */
      return NULL;
   }

   /* Create a new instance of the script */
   RuleScriptInstance* res = ctrl->createInstance();
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                   createRuleDefinitionScriptInstance                   *
 **************************************************************************/
RuleDefinitionScriptInstance* ScriptManager::createRuleDefinitionScriptInstance(
      const Kobold::String& filename)
{
   /* Load or get already loaded script controller */
   RuleDefinitionScript* ctrl = static_cast<RuleDefinitionScript*>(
         getOrLoadController(ScriptController::SCRIPT_TYPE_RULE_DEFINITION, 
            filename));
   if(!ctrl)
   {
      /* Couldn't load or compile the script, no instance will be created. */
      return NULL;
   }

   /* Create a new instance of the script */
   RuleDefinitionScriptInstance* res = ctrl->createInstance();
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                        createSkinScriptInstance                        *
 **************************************************************************/
SkinScriptInstance* ScriptManager::createSkinScriptInstance(
      const Kobold::String& filename)
{
   SkinScript* ctrl = static_cast<SkinScript*>(getOrLoadController(
            ScriptController::SCRIPT_TYPE_SKIN, filename));
   if(!ctrl)
   {
      return NULL;
   }

   SkinScriptInstance* res = ctrl->createInstance();
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                       createWidgetScriptInstance                       *
 **************************************************************************/
WidgetScriptInstance* ScriptManager::createWidgetScriptInstance(
      const Kobold::String& filename, ScriptObjectWidget* widget)
{
   WidgetScript* ctrl = static_cast<WidgetScript*>(getOrLoadController(
            ScriptController::SCRIPT_TYPE_WIDGET, filename));
   if(!ctrl)
   {
      return NULL;
   }

   WidgetScriptInstance* res = ctrl->createInstance(widget);
   insertInstance(res);

   return res;
}

/**************************************************************************
 *                    createDialogWidgetScriptInstance                    *
 **************************************************************************/
DialogWidgetScriptInstance* ScriptManager::createDialogWidgetScriptInstance(
      const Kobold::String& filename)
{
   DialogWidgetScript* ctrl = static_cast<DialogWidgetScript*>(
         getOrLoadController(ScriptController::SCRIPT_TYPE_DIALOG_WIDGET, 
            filename));
   if(!ctrl)
   {
      return NULL;
   }

   DialogWidgetScriptInstance* res = ctrl->createInstance();
   insertInstance(res);

   return res;
}


/**************************************************************************
 *                             insertInstance                             *
 **************************************************************************/
void ScriptManager::insertInstance(ScriptInstance* instance)
{
   if(instance)
   {
      managerMutex.lock();
      instances.insert(instance);
      managerMutex.unlock();
   }
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
   managerMutex.lock();
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
   managerMutex.unlock();
}

/**************************************************************************
 *                     removeAllUnusedScriptObjects                       *
 **************************************************************************/
void ScriptManager::removeAllUnusedScriptObjects()
{
   managerMutex.lock();
   int total = objects.getTotal();
   ScriptObject* obj = static_cast<ScriptObject*>(
         controllers.getFirst());
   for(int i=0; i < total; i++)
   {
      if(obj->getTotalReferences() == 0)
      {
         ScriptObject* aux = obj;
         obj = static_cast<ScriptObject*>(obj->getNext());
         objects.remove(aux);
      }
      else
      {
         obj = static_cast<ScriptObject*>(obj->getNext());
      }
   }
   managerMutex.unlock();
}

/**************************************************************************
 *                             callFunction                               *
 **************************************************************************/
void ScriptManager::callFunction(ScriptInstance* instance, 
      asIScriptFunction* function)
{
   asIScriptContext* ctx = prepareContextFromPool(function);
   ctx->SetObject(instance->getObject());
   executeWithSuspend(instance, ctx);
}

/**************************************************************************
 *                          executeWithSuspend                            *
 **************************************************************************/
void ScriptManager::executeWithSuspend(ScriptInstance* instance, 
      asIScriptContext* ctx)
{
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
 *                        suspendByPendingAction                          *
 **************************************************************************/
void ScriptManager::suspendByPendingAction(PendingAction* pendingAction)
{
   //FIXME: what happens when a script called a function which calls
   //another script and this last one is suspended? Probably we'll
   //need to do something different for this case with the running context
   //stack!
   RunningInfo* info = runningInfo.top();
   info->instance->addSuspendedContext(info->context, pendingAction);
   info->context->Suspend();
}

/**************************************************************************
 *                             disablePcMoves                             *
 **************************************************************************/
void ScriptManager::disablePcMoves()
{
   Game::disablePcMoves();
}

/**************************************************************************
 *                             enablePcMoves                              *
 **************************************************************************/
void ScriptManager::enablePcMoves()
{
   Game::enablePcMoves();
}

/**************************************************************************
 *                                playSound                               *
 **************************************************************************/
void ScriptManager::playSound(float x, float y, float z, Kobold::String file)
{
   Kosound::Sound::addSoundEffect(x, y, z, -1, file, 
         new Kobold::OgreFileReader());
}

/**************************************************************************
 *                                  sleep                                 *
 **************************************************************************/
void ScriptManager::sleep(int seconds)
{
   suspendByPendingAction(new PendingActionSleep(seconds));
}

/**************************************************************************
 *                                  print                                 *
 **************************************************************************/
void ScriptManager::print(Kobold::String s)
{
   Kobold::Log::add(s);
}

/**************************************************************************
 *                               brief                                    *
 **************************************************************************/
void ScriptManager::brief(Kobold::String s)
{
   Briefing::addText(s);
}

/**************************************************************************
 *                            briefColor                                  *
 **************************************************************************/
void ScriptManager::briefColor(Kobold::String s, int r, int g, int b)
{
   Briefing::addText(r, g, b, s);
}

/**************************************************************************
 *                            briefBreak                                  *
 **************************************************************************/
void ScriptManager::briefBreak()
{
   Briefing::addLineBreak();
}

/**************************************************************************
 *                             translate                                  *
 **************************************************************************/
Kobold::String ScriptManager::translate(Kobold::String s)
{
   return gettext(s.c_str());
}

/**************************************************************************
 *                         insertScriptObject                             *
 **************************************************************************/
void ScriptManager::insertScriptObject(ScriptObject* obj)
{
   managerMutex.lock();
   objects.insert(obj);
   managerMutex.unlock();
}

/**************************************************************************
 *                         getAndDefinePointer                            *
 **************************************************************************/
ScriptObject* ScriptManager::getAndDefinePointer(Kobold::String filename,
      const Ogre::Vector3 pos, void* newPtr, 
      const ScriptObject::ScriptObjectType type, bool createIfNotFound)
{
   ScriptObject* res = NULL;

   managerMutex.lock();

   /* Search for an already created one. */ 
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


   if((res == NULL) && (createIfNotFound))
   {
      /* Not found, must create one */
      switch(type)
      {
         case ScriptObject::TYPE_CHARACTER:
         {
            res = new ScriptObjectCharacter(filename, pos, 
                  static_cast<Character*>(newPtr));
            insertScriptObject(res);
         }

         break;
         case ScriptObject::TYPE_OBJECT:
         {
            res = new ScriptObjectObject(filename, pos, 
                  static_cast<Object*>(newPtr));
            insertScriptObject(res);
         }
         break;
         default:
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
                  "Error: Unexpected type for scriptObject retrieve: %d\n",
                  type);
         }
         break;
      }
   }

   managerMutex.unlock();

   return res;
}

/**************************************************************************
 *                           getScriptObject                              *
 **************************************************************************/
ScriptObject* ScriptManager::getScriptObject(Kobold::String filename,
               const ScriptObject::ScriptObjectType type)
{
   ScriptObject* res = NULL;
   managerMutex.lock();
   ScriptObject* cur = static_cast<ScriptObject*>(objects.getFirst());
   for(int i = 0; i < objects.getTotal(); i++)
   {
      if((cur->getType() == type) && (cur->isEquivalent(filename)))
      {
         /* Found */
         res = cur;
         break;
      }
      cur = static_cast<ScriptObject*>(cur->getNext());
   }
   managerMutex.unlock();

   return res;
}

/**************************************************************************
 *                           getScriptObject                              *
 **************************************************************************/
ScriptObject* ScriptManager::getScriptObject(Kobold::String filename, 
      float x, float y, float z, const ScriptObject::ScriptObjectType type)
{
   ScriptObject* res = NULL;
   Ogre::Vector3 pos(x, y, z);
   managerMutex.lock();
   ScriptObject* cur = static_cast<ScriptObject*>(objects.getFirst());
   for(int i = 0; i < objects.getTotal(); i++)
   {
      if((cur->getType() == type) &&
         (cur->isEquivalent(filename, pos)))
      {
         /* Found */
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
   ScriptObjectCharacter* res = static_cast<ScriptObjectCharacter*>(
         getScriptObject(filename, x, y, z, ScriptObject::TYPE_CHARACTER));
   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectCharacter(filename, Ogre::Vector3(x, y, z), NULL);
      insertScriptObject(res);
   }
   return res;
}

/**************************************************************************
 *                             getCharacter                               *
 **************************************************************************/
ScriptObjectCharacter* ScriptManager::getCharacterByFilename(Kobold::String 
      filename)
{
   ScriptObjectCharacter* res = static_cast<ScriptObjectCharacter*>(
         getScriptObject(filename, ScriptObject::TYPE_CHARACTER));

   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectCharacter(filename);
      insertScriptObject(res);
   }

   return res;
}

/**************************************************************************
 *                        getFirstPlayableCharacter                       *
 **************************************************************************/
ScriptObjectCharacter* ScriptManager::getFirstPlayableCharacter()
{
   CharacterList* pcs = Game::getPcs();
   Character* c = static_cast<Character*>(pcs->getFirst());
   if(c != NULL)
   {
      ScriptObjectCharacter* res = static_cast<ScriptObjectCharacter*>(
            c->getScriptObject());

      if(res == NULL)
      {
         /* Without script object, must create one */
         res = new ScriptObjectCharacter(c);
         insertScriptObject(res);
      }

      return res;
   }

   return NULL;
}

/**************************************************************************
 *                               getObject                                *
 **************************************************************************/
ScriptObjectObject* ScriptManager::getObjectByFilename(Kobold::String 
      filename)
{
   ScriptObjectObject* res = static_cast<ScriptObjectObject*>(
         getScriptObject(filename, ScriptObject::TYPE_OBJECT));

   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectObject(filename);
      insertScriptObject(res);
   }

   return res;
}

/**************************************************************************
 *                               getObject                                *
 **************************************************************************/
ScriptObjectObject* ScriptManager::getObject(Kobold::String filename, 
      float x, float y, float z)
{
   ScriptObjectObject* res = static_cast<ScriptObjectObject*>(
         getScriptObject(filename, x, y, z, ScriptObject::TYPE_OBJECT));
   if(res == NULL)
   {
      /* Not found, must create one */
      res = new ScriptObjectObject(filename, Ogre::Vector3(x, y, z), NULL);
      insertScriptObject(res);
   }
   return res;
}

