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

#include "rulescript.h"
#include "scriptinstance.h"
#include "scriptmanager.h"
#include "scriptobjectruledef.h"
#include "../rules/ruledef.h"
#include <kobold/log.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
RuleScriptInstance::RuleScriptInstance(asIScriptObject* obj, RuleScript* script,
      ScriptManager* manager)
                  :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
RuleScriptInstance::~RuleScriptInstance()
{
}

/**************************************************************************
 *                               callRoll                                 *
 **************************************************************************/
bool RuleScriptInstance::callRoll(RuleDefinitionValue* testRule, 
      RuleDefinitionValue* againstRule)
{
   bool res = false;
   RuleScript* ruleScript = static_cast<RuleScript*>(script);
   if(ruleScript->getRollFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            ruleScript->getRollFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, testRule->getScriptObject());
      ctx->SetArgObject(1, againstRule->getScriptObject());
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      if(r == asEXECUTION_FINISHED)
      {
         res = ctx->GetReturnByte(); 
      }
      manager->returnContextToPool(ctx);
   }

   return res;
}

/**************************************************************************
 *                            callRollValue                               *
 **************************************************************************/
bool RuleScriptInstance::callRollValue(RuleDefinitionValue* testRule, 
      int againstValue)
{
   assert(testRule != NULL);
   assert(testRule->getScriptObject() != NULL);

   bool res = false;
   RuleScript* ruleScript = static_cast<RuleScript*>(script);
   if(ruleScript->getRollValueFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            ruleScript->getRollValueFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, testRule->getScriptObject());
      ctx->SetArgDWord(1, againstValue);
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      if(r == asEXECUTION_FINISHED)
      {
         res = ctx->GetReturnByte(); 
      }
      manager->returnContextToPool(ctx);
   }

   return res;
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
RuleScript::RuleScript(ScriptManager* manager)
          :ScriptController(SCRIPT_TYPE_RULE, manager)
{
   this->factoryFunction = NULL;
   this->stepFunction = NULL;
   this->rollFunction = NULL;
   this->rollValueFunction = NULL;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
RuleScript::~RuleScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
RuleScriptInstance* RuleScript::createInstance()
{
   RuleScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new RuleScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void RuleScript::setFunctionPointers()
{
   Ogre::String factoryName = Ogre::String(mainType->GetName()) + "@" +
      Ogre::String(mainType->GetName()) + "()";
   this->factoryFunction = mainType->GetFactoryByDecl(factoryName.c_str());
   if(!factoryFunction)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Script '%s' couldn't find factory '%s'",
            getFilename().c_str(), factoryName.c_str());
   }
   this->stepFunction = mainType->GetMethodByDecl("void step()");
   this->rollFunction = mainType->GetMethodByDecl(
         "bool roll(RuleDefinition @+, RuleDefinition @+)");
   assert(this->rollFunction);
   this->rollValueFunction = mainType->GetMethodByDecl(
         "bool roll(RuleDefinition @+, int)");
   assert(this->rollValueFunction);
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* RuleScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getStepFunction                           *
 **************************************************************************/
asIScriptFunction* RuleScript::getStepFunction()
{
   return stepFunction;
}

/**************************************************************************
 *                             getRollFunction                            *
 **************************************************************************/
asIScriptFunction* RuleScript::getRollFunction()
{
   return rollFunction;
}

/**************************************************************************
 *                           getRollValueFunction                         *
 **************************************************************************/
asIScriptFunction* RuleScript::getRollValueFunction()
{
   return rollValueFunction;
}

