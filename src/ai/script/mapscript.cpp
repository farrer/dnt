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

#include "mapscript.h"
#include "scriptinstance.h"
#include "scriptmanager.h"
#include <kobold/log.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
MapScriptInstance::MapScriptInstance(asIScriptObject* obj, MapScript* script,
      ScriptManager* manager)
                  :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
MapScriptInstance::~MapScriptInstance()
{
}

/**************************************************************************
 *                              callOnEnter                               *
 **************************************************************************/
void MapScriptInstance::callOnEnter()
{
   MapScript* mapScript = static_cast<MapScript*>(script);
   if(mapScript->getOnEnterFunction())
   {
      manager->callFunction(this, mapScript->getOnEnterFunction()); 
   }
}

/**************************************************************************
 *                              callOnLoad                                *
 **************************************************************************/
void MapScriptInstance::callOnLoad()
{
   MapScript* mapScript = static_cast<MapScript*>(script);
   if(mapScript->getOnLoadFunction())
   {
      manager->callFunction(this, mapScript->getOnLoadFunction()); 
   }
}

/**************************************************************************
 *                              callOnExit                                *
 **************************************************************************/
void MapScriptInstance::callOnExit()
{
   MapScript* mapScript = static_cast<MapScript*>(script);
   if(mapScript->getOnExitFunction())
   {
      manager->callFunction(this, mapScript->getOnExitFunction()); 
   }
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
MapScript::MapScript(ScriptManager* manager)
          :ScriptController(SCRIPT_TYPE_MAP, manager)
{
   this->factoryFunction = NULL;
   this->updateFunction = NULL;
   this->onLoadFunction = NULL;
   this->onEnterFunction = NULL;
   this->onExitFunction = NULL;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
MapScript::~MapScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
MapScriptInstance* MapScript::createInstance(Ogre::String mapname)
{
   MapScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   ctx->SetArgObject(0, &mapname);
   int r = manager->executeCall(ctx);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new MapScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void MapScript::setFunctionPointers()
{
   Ogre::String factoryName = Ogre::String(mainType->GetName()) + "@" +
      Ogre::String(mainType->GetName()) + "(string)";
   this->factoryFunction = mainType->GetFactoryByDecl(factoryName.c_str());
   if(!factoryFunction)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Script '%s' couldn't find factory '%s'",
            getFilename().c_str(), factoryName.c_str());
   }
   this->updateFunction = mainType->GetMethodByDecl("void update()");
   this->onLoadFunction = mainType->GetMethodByDecl("void onLoad()");
   this->onEnterFunction = mainType->GetMethodByDecl("void onEnter()");
   this->onExitFunction = mainType->GetMethodByDecl("void onExit()");
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* MapScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getUpdateFunction                           *
 **************************************************************************/
asIScriptFunction* MapScript::getUpdateFunction()
{
   return updateFunction;
}

/**************************************************************************
 *                            getOnLoadFunction                           *
 **************************************************************************/
asIScriptFunction* MapScript::getOnLoadFunction()
{
   return onLoadFunction;
}

/**************************************************************************
 *                            getOnEnterFunction                          *
 **************************************************************************/
asIScriptFunction* MapScript::getOnEnterFunction()
{
   return onEnterFunction;
}

/**************************************************************************
 *                            getOnExitFunction                           *
 **************************************************************************/
asIScriptFunction* MapScript::getOnExitFunction()
{
   return onExitFunction;
}

