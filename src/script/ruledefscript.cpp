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

#include "ruledefscript.h"
#include "scriptinstance.h"
#include "scriptmanager.h"
#include "scriptobjectcharacter.h"
#include <kobold/log.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
RuleDefinitionScriptInstance::RuleDefinitionScriptInstance(
      asIScriptObject* obj, RuleDefinitionScript* script,
      ScriptManager* manager)
                  :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
RuleDefinitionScriptInstance::~RuleDefinitionScriptInstance()
{
}

/**************************************************************************
 *                                callUse                                 *
 **************************************************************************/
void RuleDefinitionScriptInstance::callUse(ScriptObjectCharacter* owner, 
      ScriptObjectCharacter* target)
{
   assert(owner != NULL);
   assert(target != NULL);

   RuleDefinitionScript* ruleScript = static_cast<RuleDefinitionScript*>(
         script);

   if(ruleScript->getUseFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            ruleScript->getOnInsertFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, owner);
      ctx->SetArgObject(1, target);
      manager->executeWithSuspend(this, ctx);
   }
}

/**************************************************************************
 *                            callOnInsert                                *
 **************************************************************************/
void RuleDefinitionScriptInstance::callOnInsert(ScriptObjectCharacter* owner)
{
   assert(owner != NULL);

   RuleDefinitionScript* ruleScript = static_cast<RuleDefinitionScript*>(
         script);

   if(ruleScript->getOnInsertFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            ruleScript->getOnInsertFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, owner);
      manager->executeWithSuspend(this, ctx);
   }
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
RuleDefinitionScript::RuleDefinitionScript(ScriptManager* manager)
          :ScriptController(SCRIPT_TYPE_RULE_DEFINITION, manager)
{
   this->factoryFunction = NULL;
   this->stepFunction = NULL;
   this->useFunction = NULL;
   this->onInsertFunction = NULL;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
RuleDefinitionScript::~RuleDefinitionScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
RuleDefinitionScriptInstance* RuleDefinitionScript::createInstance()
{
   RuleDefinitionScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new RuleDefinitionScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void RuleDefinitionScript::setFunctionPointers()
{
   Ogre::String factoryName = Ogre::String(mainType->GetName()) + "@" +
      Ogre::String(mainType->GetName()) + "()";
   this->factoryFunction = mainType->GetFactoryByDecl(factoryName.c_str());
   if(!factoryFunction)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
            "Error: Script '%s' couldn't find factory '%s'",
            getFilename().c_str(), factoryName.c_str());
   }
   this->stepFunction = mainType->GetMethodByDecl("void step()");
   this->useFunction = mainType->GetMethodByDecl(
         "void use(Character @+, Character @+)");
   this->onInsertFunction = mainType->GetMethodByDecl(
         "void onInsert(Character @+)");
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* RuleDefinitionScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getStepFunction                           *
 **************************************************************************/
asIScriptFunction* RuleDefinitionScript::getStepFunction()
{
   return stepFunction;
}

/**************************************************************************
 *                              getUseFunction                            *
 **************************************************************************/
asIScriptFunction* RuleDefinitionScript::getUseFunction()
{
   return useFunction;
}

/**************************************************************************
 *                           getOnInsertFunction                          *
 **************************************************************************/
asIScriptFunction* RuleDefinitionScript::getOnInsertFunction()
{
   return onInsertFunction;
}


