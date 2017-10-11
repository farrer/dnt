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

#include "modulescript.h"
#include "scriptmanager.h"
#include <kobold/log.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ModuleScriptInstance::ModuleScriptInstance(asIScriptObject* obj, 
      ModuleScript* script, ScriptManager* manager)
                     :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ModuleScriptInstance::~ModuleScriptInstance()
{
}

/**************************************************************************
 *                            getSkinFolder                               *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getSkinFolder()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getSkinFolderFunction());
}

/**************************************************************************
 *                           getBaseFolders                               *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getBaseFolders()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getBaseFoldersFunction());
}

/**************************************************************************
 *                           getSkinFilename                              *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getSkinFilename()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getSkinFilenameFunction());
}

/**************************************************************************
 *                          getRulesFilename                              *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getRulesFilename()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getRulesFilenameFunction());
}

/**************************************************************************
 *                            getInitialMap                               *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getInitialMap()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getInitialMapFunction());
}

/**************************************************************************
 *                           getDialogWidget                              *
 **************************************************************************/
Kobold::String ModuleScriptInstance::getDialogWidget()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   
   return getStringFromFunction(moduleScript->getDialogWidgetFunction());
}

/**************************************************************************
 *                            callOnInitGame                              *
 **************************************************************************/
void ModuleScriptInstance::callOnInitGame()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   if(moduleScript->getOnInitGameFunction())
   {
      manager->callFunction(this, moduleScript->getOnInitGameFunction()); 
   }
}

/**************************************************************************
 *                            callOnExitGame                              *
 **************************************************************************/
void ModuleScriptInstance::callOnExitGame()
{
   ModuleScript* moduleScript = static_cast<ModuleScript*>(script);
   if(moduleScript->getOnExitGameFunction())
   {
      manager->callFunction(this, moduleScript->getOnExitGameFunction()); 
   }
}


/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ModuleScript::ModuleScript(ScriptManager* manager)
             :ScriptController(SCRIPT_TYPE_MODULE, manager),
              factoryFunction(NULL),
              stepFunction(NULL),
              skinFolderFunction(NULL),
              baseFoldersFunction(NULL),
              rulesFilenameFunction(NULL),
              skinFilenameFunction(NULL),
              initialMapFunction(NULL),
              dialogWidgetFunction(NULL),
              onInitGameFunction(NULL),
              onExitGameFunction(NULL)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ModuleScript::~ModuleScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
ModuleScriptInstance* ModuleScript::createInstance()
{
   ModuleScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new ModuleScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void ModuleScript::setFunctionPointers()
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
   this->skinFilenameFunction = mainType->GetMethodByDecl(
         "string getSkinFilename()");
   assert(this->skinFilenameFunction);
   this->rulesFilenameFunction = mainType->GetMethodByDecl(
         "string getRulesFilename()");
   assert(this->rulesFilenameFunction);
   this->initialMapFunction = mainType->GetMethodByDecl(
         "string getInitialMap()");
   assert(this->initialMapFunction);
   this->dialogWidgetFunction = mainType->GetMethodByDecl(
         "string getDialogWidget()");
   assert(this->dialogWidgetFunction);
   this->baseFoldersFunction = mainType->GetMethodByDecl(
         "string getBaseFolders()");
   assert(this->baseFoldersFunction);
   this->skinFolderFunction = mainType->GetMethodByDecl(
         "string getSkinFolder()");
   this->onInitGameFunction = mainType->GetMethodByDecl("void onInitGame()");
   this->onExitGameFunction = mainType->GetMethodByDecl("void onExitGame()");
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* ModuleScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getStepFunction                           *
 **************************************************************************/
asIScriptFunction* ModuleScript::getStepFunction()
{
   return stepFunction;
}

/**************************************************************************
 *                           getSkinFolderFunction                        *
 **************************************************************************/
asIScriptFunction* ModuleScript::getSkinFolderFunction()
{
   return skinFolderFunction;
}

/**************************************************************************
 *                           getBaseFoldersFunction                       *
 **************************************************************************/
asIScriptFunction* ModuleScript::getBaseFoldersFunction()
{
   return baseFoldersFunction;
}

/**************************************************************************
 *                           getSkinFilenameFunction                      *
 **************************************************************************/
asIScriptFunction* ModuleScript::getSkinFilenameFunction()
{
   return skinFilenameFunction;
}

/**************************************************************************
 *                           getRulesFilenameFunction                     *
 **************************************************************************/
asIScriptFunction* ModuleScript::getRulesFilenameFunction()
{
   return rulesFilenameFunction;
}

/**************************************************************************
 *                            getInitialMapFunction                       *
 **************************************************************************/
asIScriptFunction* ModuleScript::getInitialMapFunction()
{
   return initialMapFunction;
}

/**************************************************************************
 *                           getDialogWidgetFunction                      *
 **************************************************************************/
asIScriptFunction* ModuleScript::getDialogWidgetFunction()
{
   return dialogWidgetFunction;
}

/**************************************************************************
 *                            getOnInitGameFunction                       *
 **************************************************************************/
asIScriptFunction* ModuleScript::getOnInitGameFunction()
{
   return onInitGameFunction;
}

/**************************************************************************
 *                            getOnExitGameFunction                       *
 **************************************************************************/
asIScriptFunction* ModuleScript::getOnExitGameFunction()
{
   return onExitGameFunction;
}

