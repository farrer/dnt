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

#include "widgetscript.h"
#include "scriptmanager.h"
#include <farso/controller.h>
#include <kobold/log.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
WidgetScriptInstance::WidgetScriptInstance(asIScriptObject* obj, 
      WidgetScript* script, ScriptManager* manager)
                  :ScriptInstance(obj, script, manager)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
WidgetScriptInstance::~WidgetScriptInstance()
{
}

/**************************************************************************
 *                          getWidgetFilename                             *
 **************************************************************************/
Kobold::String WidgetScriptInstance::getWidgetFilename()
{
   Kobold::String res = "";

   WidgetScript* widgetScript = static_cast<WidgetScript*>(script);
   if(widgetScript->getWidgetFilenameFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            widgetScript->getWidgetFilenameFunction());
      ctx->SetObject(getObject());
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      if(r == asEXECUTION_FINISHED)
      {
         res = *static_cast<Kobold::String*>(ctx->GetReturnObject()); 
      }
      manager->returnContextToPool(ctx);
   }

   return res;
}

/**************************************************************************
 *                              treatEvent                                *
 **************************************************************************/
void WidgetScriptInstance::treatEvent(int eventType, 
      const Kobold::String& widgetId)
{
   Kobold::String id = widgetId;

   WidgetScript* widgetScript = static_cast<WidgetScript*>(script);
   if(widgetScript->getWidgetFilenameFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            widgetScript->getWidgetFilenameFunction());
      ctx->SetObject(getObject());
      ctx->SetArgWord(0, eventType);
      ctx->SetArgObject(1, &id);
      int r = manager->executeCall(ctx, this);
      assert(r == asEXECUTION_FINISHED);
      manager->returnContextToPool(ctx);
   }
}

/**************************************************************************
 *                                onEvent                                 *
 **************************************************************************/
void WidgetScriptInstance::onEvent(const Farso::EventType& eventType, 
      Farso::Widget* widget)
{
   if(widget)
   {
      treatEvent(eventType, widget->getId());
   }
}

/**************************************************************************
 *                              shouldQuit                                *
 **************************************************************************/
bool WidgetScriptInstance::shouldQuit()
{
   bool res = false;

   WidgetScript* widgetScript = static_cast<WidgetScript*>(script);
   if(widgetScript->getShouldQuitFunction())
   {
      asIScriptContext* ctx = manager->prepareContextFromPool(
            widgetScript->getWidgetFilenameFunction());
      ctx->SetObject(getObject());
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
WidgetScript::WidgetScript(ScriptManager* manager)
           :ScriptController(SCRIPT_TYPE_SKIN, manager),
            factoryFunction(NULL),
            stepFunction(NULL),
            widgetFilenameFunction(NULL),
            treatEventFunction(NULL)
{
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
WidgetScript::~WidgetScript()
{
}

/**************************************************************************
 *                             createInstance                             *
 **************************************************************************/
WidgetScriptInstance* WidgetScript::createInstance()
{
   WidgetScriptInstance* res = NULL;

   /* Call our factory function */
   assert(factoryFunction != NULL);
   asIScriptContext* ctx = manager->prepareContextFromPool(factoryFunction);
   int r = manager->executeCall(ctx, NULL, 10000);
   assert(r == asEXECUTION_FINISHED);
   if(r == asEXECUTION_FINISHED)
   {
      asIScriptObject* obj = *((asIScriptObject**) 
            ctx->GetAddressOfReturnValue());
      res = new WidgetScriptInstance(obj, this, manager);
   }
   manager->returnContextToPool(ctx);

   if(res)
   {
      /* Get our widget Filename and open it */
      Kobold::String jsonFilename = res->getWidgetFilename();
      Farso::Controller::insertFromJson(loadFile(jsonFilename), res);
   }

   return res;
}

/**************************************************************************
 *                          setFunctionPointers                           *
 **************************************************************************/
void WidgetScript::setFunctionPointers()
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
   this->widgetFilenameFunction = mainType->GetMethodByDecl(
         "string getWidgetFilename()");
   assert(this->widgetFilenameFunction);
   this->treatEventFunction = mainType->GetMethodByDecl(
         "void onEvent(int, string)");
   assert(this->treatEventFunction);
   this->shouldQuitFunction = mainType->GetMethodByDecl(
         "bool shouldQuit()");
}

/**************************************************************************
 *                            getFactoryFunction                          *
 **************************************************************************/
asIScriptFunction* WidgetScript::getFactoryFunction()
{
   return factoryFunction;
}

/**************************************************************************
 *                            getStepFunction                           *
 **************************************************************************/
asIScriptFunction* WidgetScript::getStepFunction()
{
   return stepFunction;
}

/**************************************************************************
 *                          getTreatEventFunction                         *
 **************************************************************************/
asIScriptFunction* WidgetScript::getTreatEventFunction()
{
   return treatEventFunction;
}

/**************************************************************************
 *                          getWidgetFilenameFunction                     *
 **************************************************************************/
asIScriptFunction* WidgetScript::getWidgetFilenameFunction()
{
   return widgetFilenameFunction;
}

/**************************************************************************
 *                         getShouldQuitFunction                          *
 **************************************************************************/
asIScriptFunction* WidgetScript::getShouldQuitFunction()
{
   return shouldQuitFunction;
}

/************************************************************************
 *                                loadFile                              *
 ************************************************************************/
Kobold::String WidgetScript::loadFile(const Kobold::String& filename)
{
   struct stat tagStat;
   FILE* pFile = fopen(filename.c_str(), "rb");
   if(!pFile)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: couldn't open file '%s'", filename.c_str());
      return "";
   }
   stat(filename.c_str(), &tagStat);
   char* buf = new char[tagStat.st_size];
   size_t result = fread((void*)buf, 1, tagStat.st_size, pFile);
   if(result != (size_t)tagStat.st_size)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: couldn't load file '%s'", filename.c_str());
      return "";
   }

   buf[tagStat.st_size-1] = '\0';
   Kobold::String res = buf;
   delete[] buf;

   return res;
}

