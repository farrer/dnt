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

#include "scriptobjectwidget.h"
#include "scriptmanager.h"
#include <assert.h>
#include <kobold/log.h>
#include "../core/game.h"

using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectWidget::ScriptObjectWidget(const Kobold::String& filename)
                   :ScriptObject(TYPE_WIDGET)
{
   printf("Will create instance for: %s\n", filename.c_str());
   widgetScript = Game::getScriptManager()->createWidgetScriptInstance(
         filename);
   printf("Done creating instance for: %s\n", filename.c_str());
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectWidget::~ScriptObjectWidget()
{
   if(widgetScript)
   {
      Game::getScriptManager()->removeInstance(widgetScript);
   }
}

/**************************************************************************
 *                               setPointer                               *
 **************************************************************************/
void ScriptObjectWidget::setPointer(void* newPtr)
{
   /* Should not be called! */
   assert(false);
}

/**************************************************************************
 *                                 isValid                                *
 **************************************************************************/
const bool ScriptObjectWidget::isValid()
{
   return !widgetScript->shouldQuit();
}

/**************************************************************************
 *                            factoryWidget                                 *
 **************************************************************************/
ScriptObjectWidget* ScriptObjectWidget::factoryWidget(
               const Kobold::String& filename)
{
   ScriptObjectWidget* obj = new ScriptObjectWidget(filename);
   obj->addReference();
   return obj;
}

/**************************************************************************
 *                             registerClass                              *
 **************************************************************************/
void ScriptObjectWidget::registerClass(asIScriptEngine* asEngine)
{
   int r;

   r = asEngine->RegisterObjectType("Widget", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Widget", asBEHAVE_FACTORY,
         "Widget@ f(string)", asFUNCTION(ScriptObjectWidget::factoryWidget), 
         asCALL_CDECL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Widget", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectWidget, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Widget", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectWidget, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}


/**************************************************************************
 *                           registerFunctions                            *
 **************************************************************************/
void ScriptObjectWidget::registerFunctions(asIScriptEngine* asEngine)
{
}

