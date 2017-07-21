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

#include "scriptinstance.h"
#include "scriptmanager.h"
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptInstance::ScriptInstance(asIScriptObject* obj, ScriptController* script,
      ScriptManager* manager)
{
   /* Set the object and add to its reference */
   this->obj = obj;
   this->obj->AddRef();
   /* Set the script controller and manager*/
   this->script = script;
   this->manager = manager;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptInstance::~ScriptInstance()
{
   if(hasContextToResume())
   {
      manager->returnContextToPool(context);
      context = NULL;
   }
   /* No more using the object, decrement its reference */
   this->obj->Release();
}

/**************************************************************************
 *                              shouldResume                              *
 **************************************************************************/
const bool ScriptInstance::shouldResume() const
{
   return hasContextToResume() && !waitingActionEnd();
}

/**************************************************************************
 *                          waitingActionEnd                              *
 **************************************************************************/
const bool ScriptInstance::waitingActionEnd() const
{
   //TODO
   return false;
}

/**************************************************************************
 *                           hasContextToResume                           *
 **************************************************************************/
const bool ScriptInstance::hasContextToResume() const
{
   return context != NULL;
}

/**************************************************************************
 *                         setSuspendedContext                            *
 **************************************************************************/
void ScriptInstance::setSuspendedContext(asIScriptContext* context)
{
   this->context = context;
}

/**************************************************************************
 *                                  resume                                *
 **************************************************************************/
void ScriptInstance::resume()
{
   int r = manager->executeCall(context);
   if(r == asEXECUTION_SUSPENDED)
   {
      setSuspendedContext(context);
   }
   else
   {
      manager->returnContextToPool(context);
      context = NULL;
   }
}

