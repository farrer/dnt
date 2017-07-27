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
SuspendedInfo::SuspendedInfo(asIScriptContext* context, PendingAction* action,
      ScriptManager* manager)
{
   this->context = context;
   this->action = action;
   this->manager = manager;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
SuspendedInfo::~SuspendedInfo()
{
   if(hasContextToResume())
   {
      manager->returnContextToPool(context);
      context = NULL;
   }
   if(action != NULL)
   {
      delete action;
      action = NULL;
   }
}

/**************************************************************************
 *                              deleteAction                              *
 **************************************************************************/
void SuspendedInfo::deleteAction()
{
   assert(action != NULL);
   delete action;
   action = NULL;
}

/**************************************************************************
 *                            clearContextInfo                            *
 **************************************************************************/
void SuspendedInfo::clearContextInfo()
{
   context = NULL;
}

/**************************************************************************
 *                              shouldResume                              *
 **************************************************************************/
const bool SuspendedInfo::shouldResume() const
{
   return hasContextToResume() && !waitingActionEnd();
}

/**************************************************************************
 *                          waitingActionEnd                              *
 **************************************************************************/
const bool SuspendedInfo::waitingActionEnd() const
{
   return action != NULL;
}

/**************************************************************************
 *                           hasContextToResume                           *
 **************************************************************************/
const bool SuspendedInfo::hasContextToResume() const
{
   return context != NULL;
}

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
   suspended.clear();
   /* No more using the object, decrement its reference */
   this->obj->Release();
}

/**************************************************************************
 *                                  resume                                *
 **************************************************************************/
int ScriptInstance::resume(asIScriptContext* ctx)
{
   return manager->executeCall(ctx, this);
}

/**************************************************************************
 *                           addSuspendedContext                          *
 **************************************************************************/
void ScriptInstance::addSuspendedContext(asIScriptContext* ctx, 
      PendingAction* act)
{
   mutex.lock();

   /* Only add if context is different from last added one (to avoid double
    * references to the same suspended context) */
   SuspendedInfo* last = static_cast<SuspendedInfo*>(suspended.getLast());
   if((last == NULL) || (last->getContext() != ctx))
   {
      suspended.insert(new SuspendedInfo(ctx, act, manager));
   }

   mutex.unlock();
}

/**************************************************************************
 *                                   step                                 *
 **************************************************************************/
void ScriptInstance::step()
{
   /* Note: only do the step if not already running any other
    * function on the script. If are, use its time to resume it. */
   if(suspended.getTotal() == 0)
   {
      /* No suspended contexts: call the normal step function */
      if(script->getStepFunction())
      {
         manager->callFunction(this, script->getStepFunction());
      }
   }
   else
   {
      bool removed;
      mutex.lock();
      /* Let's check all suspended contexts */
      SuspendedInfo* info = static_cast<SuspendedInfo*>(suspended.getFirst());
      int total = suspended.getTotal();
      mutex.unlock();

      for(int i=0; i < total; i++)
      {
         removed = false;
         if(info->shouldResume())
         {
            /* Remove the current suspended info (and set next) */
            SuspendedInfo* tmp = info;
            asIScriptContext* ctx = tmp->getContext();
            tmp->clearContextInfo();
            mutex.lock();
            info = static_cast<SuspendedInfo*>(info->getNext());
            suspended.remove(tmp);
            removed = true;
            mutex.unlock();
            
            /* Resume the suspended script */
            if(resume(ctx) != asEXECUTION_SUSPENDED)
            {
               /* Execution finished. We must return our context to the pool */
               manager->returnContextToPool(ctx);
            }
         }
         else if(info->waitingActionEnd())
         {
            /* Waiting for action end, update it. */
            if(info->getPendingAction()->update())
            {
               /* Action done, must remove it and resume script on next cycle */
               info->deleteAction();
            }
         }
         
         if(!removed)
         {
            mutex.lock();
            info = static_cast<SuspendedInfo*>(info->getNext());
            mutex.unlock();
         }
      }

   }
}

