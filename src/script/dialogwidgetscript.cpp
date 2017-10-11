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

#include "dialogwidgetscript.h"
#include "scriptmanager.h"
#include "../rules/thing.h"
#include <farso/controller.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
DialogWidgetScriptInstance::DialogWidgetScriptInstance(asIScriptObject* obj, 
      WidgetScript* script, ScriptManager* manager)
   :WidgetScriptInstance(obj, script, manager, NULL)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
DialogWidgetScriptInstance::~DialogWidgetScriptInstance()
{
}

/**************************************************************************
 *                            getRootWidgetId                             *
 **************************************************************************/
Kobold::String DialogWidgetScriptInstance::getRootWidgetId()
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   return getStringFromFunction(dialogScript->getRootWidgetIdFunction());
}

/**************************************************************************
 *                                 clear                                  *
 **************************************************************************/
void DialogWidgetScriptInstance::clear()
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   callProcedureWithoutParams(dialogScript->getClearFunction());
}

/**************************************************************************
 *                              selectOption                              *
 **************************************************************************/
void DialogWidgetScriptInstance::selectOption(int index)
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   passIntByFunction(dialogScript->getSelectOptionFunction(), index);
}

/**************************************************************************
 *                            getSelectOption                             *
 **************************************************************************/
int DialogWidgetScriptInstance::getSelectedOption()
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   return getIntFromFunction(dialogScript->getGetSelectedOptionFunction());
}

/**************************************************************************
 *                               addOption                                *
 **************************************************************************/
void DialogWidgetScriptInstance::addOption(const Kobold::String& text, 
      int index)
{
   Kobold::String txt = text;

   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   if(dialogScript->getAddOptionFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            dialogScript->getAddOptionFunction());
      ctx->SetArgObject(0, &txt);
      ctx->SetArgDWord(1, index);
      ctx->SetObject(getObject());
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      manager->returnContextToPool(ctx);
   }
}

/**************************************************************************
 *                             setOwnerText                               *
 **************************************************************************/
void DialogWidgetScriptInstance::setOwnerText(const Kobold::String& text)
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);
   passStringByFunction(dialogScript->getSetOwnerTextFunction(), text);
}

/**************************************************************************
 *                                setOwner                                *
 **************************************************************************/
void DialogWidgetScriptInstance::setOwner(Thing* owner)
{
   DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(script);

   if(dialogScript->getSetOwnerFunction())
   {
      Kobold::String name = owner->getName();
      Kobold::String portrait = owner->getPortraitFilename();
      asIScriptContext* ctx = manager->prepareContextFromPool(
            dialogScript->getSetOwnerFunction());
      ctx->SetObject(getObject());
      ctx->SetArgObject(0, &name);
      ctx->SetArgObject(1, &portrait);
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);

      manager->returnContextToPool(ctx);
   }
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
DialogWidgetScript::DialogWidgetScript(ScriptManager* manager)
   :WidgetScript(manager, ScriptController::SCRIPT_TYPE_DIALOG_WIDGET),
    rootWidgetIdFunction(NULL),
    clearFunction(NULL),
    addOptionFunction(NULL),
    setOwnerTextFunction(NULL),
    setOwnerFunction(NULL),
    selectOptionFunction(NULL),
    getSelectedOptionFunction(NULL),
    jsonStr("")
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
DialogWidgetScript::~DialogWidgetScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
DialogWidgetScriptInstance* DialogWidgetScript::createInstance()
{
   DialogWidgetScriptInstance* res = NULL;

   /* Call our factory function */
   assert(getFactoryFunction() != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(
         getFactoryFunction());
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new DialogWidgetScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   if(res)
   {
      /* Get our widget Filename and open it */
      Kobold::String jsonFilename = res->getWidgetFilename();
      jsonStr = loadFile(jsonFilename);
   }

   return res;
}

/**************************************************************************
 *                         getRootWidgetIdFunction                        *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getRootWidgetIdFunction()
{
   return rootWidgetIdFunction;
}


/**************************************************************************
 *                            getClearFunction                            *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getClearFunction()
{
   return clearFunction;
}

/**************************************************************************
 *                          getAddOptionFunction                          *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getAddOptionFunction()
{
   return addOptionFunction;
}

/**************************************************************************
 *                        getSetOwnerTextFunction                         *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getSetOwnerTextFunction()
{
   return setOwnerTextFunction;
}

/**************************************************************************
 *                           getSetOwnerFunction                          *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getSetOwnerFunction()
{
   return setOwnerFunction;
}

/**************************************************************************
 *                         getSelectOptionFunction                        *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getSelectOptionFunction()
{
   return selectOptionFunction;
}

/**************************************************************************
 *                       getGetSelectedOptionFunction                     *
 **************************************************************************/
asIScriptFunction* DialogWidgetScript::getGetSelectedOptionFunction()
{
   return getSelectedOptionFunction;
}

/**************************************************************************
 *                           getFunctionPointers                          *
 **************************************************************************/
void DialogWidgetScript::setFunctionPointers()
{
   WidgetScript::setFunctionPointers();

   this->rootWidgetIdFunction = mainType->GetMethodByDecl(
         "string getRootWidgetId()");
   assert(this->rootWidgetIdFunction);
   this->clearFunction = mainType->GetMethodByDecl("void clear()");
   assert(this->clearFunction);
   this->addOptionFunction = mainType->GetMethodByDecl(
         "void addOption(string, int)");
   assert(this->addOptionFunction);
   this->setOwnerTextFunction = mainType->GetMethodByDecl(
         "void setOwnerText(string)");
   assert(this->setOwnerTextFunction);
   this->setOwnerFunction = mainType->GetMethodByDecl(
         "void setOwner(string, string)");
   assert(this->setOwnerFunction);
   this->selectOptionFunction = mainType->GetMethodByDecl(
         "void selectOption(int)");
   assert(this->selectOptionFunction);
   this->getSelectedOptionFunction = mainType->GetMethodByDecl(
         "int getSelectedOption()");
   assert(this->getSelectedOptionFunction);
}


