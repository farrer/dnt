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

#include "skinscript.h"
#include "scriptmanager.h"
#include <farso/skin.h>
#include <kobold/log.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
SkinScriptInstance::SkinScriptInstance(asIScriptObject* obj, 
      SkinScript* script, ScriptManager* manager)
                  :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
SkinScriptInstance::~SkinScriptInstance()
{
}

/**************************************************************************
 *                           getElementType                               *
 **************************************************************************/
int SkinScriptInstance::getElementType(const Kobold::String& name)
{
   int res = Farso::Skin::SKIN_TYPE_UNKNOWN;
   Kobold::String typeName = name;

   SkinScript* skinScript = static_cast<SkinScript*>(script);
   if(skinScript->getElementTypeFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            skinScript->getElementTypeFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, &typeName);
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      if(r == asEXECUTION_FINISHED)
      {
         res = ctx->GetReturnWord(); 
      }
      manager->returnContextToPool(ctx);
   }

   return res;
}

/**************************************************************************
 *                          getTotalElements                              *
 **************************************************************************/
int SkinScriptInstance::getTotalElements()
{
   int res = 0;

   SkinScript* skinScript = static_cast<SkinScript*>(script);
   if(skinScript->getTotalElementsFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            skinScript->getTotalElementsFunction());
      ctx->SetObject(getObject());
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      if(r == asEXECUTION_FINISHED)
      {
         res = ctx->GetReturnWord(); 
      }
      manager->returnContextToPool(ctx);
   }

   return res;
}


/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
SkinScript::SkinScript(ScriptManager* manager)
           :ScriptController(SCRIPT_TYPE_RULE, manager),
            factoryFunction(NULL),
            stepFunction(NULL),
            totalElementsFunction(NULL),
            elementTypeFunction(NULL)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
SkinScript::~SkinScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
SkinScriptInstance* SkinScript::createInstance()
{
   SkinScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new SkinScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void SkinScript::setFunctionPointers()
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
   this->totalElementsFunction = mainType->GetMethodByDecl(
         "int getTotalElements()");
   assert(this->totalElementsFunction);
   this->elementTypeFunction = mainType->GetMethodByDecl(
         "int getElementType(string)");
   assert(this->elementTypeFunction);
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* SkinScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getStepFunction                           *
 **************************************************************************/
asIScriptFunction* SkinScript::getStepFunction()
{
   return stepFunction;
}

/**************************************************************************
 *                        getTotalElementsFunction                        *
 **************************************************************************/
asIScriptFunction* SkinScript::getTotalElementsFunction()
{
   return totalElementsFunction;
}

/**************************************************************************
 *                          getElementTypeFunction                        *
 **************************************************************************/
asIScriptFunction* SkinScript::getElementTypeFunction()
{
   return elementTypeFunction;
}


