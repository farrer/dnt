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
#include "scriptcontroller.h"
#include "pendingaction.h"
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
   this->suspendedContext = NULL;
   this->pendingAction = NULL;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptInstance::~ScriptInstance()
{
   if(hasContextToResume())
   {
      manager->returnContextToPool(suspendedContext);
      suspendedContext = NULL;
   }
   if(pendingAction != NULL)
   {
      delete pendingAction;
      pendingAction = NULL;
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
   return pendingAction != NULL;
}

/**************************************************************************
 *                           hasContextToResume                           *
 **************************************************************************/
const bool ScriptInstance::hasContextToResume() const
{
   return suspendedContext != NULL;
}

/**************************************************************************
 *                         setSuspendedContext                            *
 **************************************************************************/
void ScriptInstance::setSuspendedContext(asIScriptContext* context)
{
   this->suspendedContext = context;
}

/**************************************************************************
 *                                  resume                                *
 **************************************************************************/
void ScriptInstance::resume()
{
   int r = manager->executeCall(suspendedContext, this);
   if(r == asEXECUTION_SUSPENDED)
   {
      setSuspendedContext(suspendedContext);
   }
   else
   {
      manager->returnContextToPool(suspendedContext);
      suspendedContext = NULL;
   }
}

/**************************************************************************
 *                            setPendingAction                            *
 **************************************************************************/
void ScriptInstance::setPendingAction(PendingAction* action)
{
   this->pendingAction = action;
}

