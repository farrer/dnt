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

#ifndef _dnt_script_instance_h
#define _dnt_script_instance_h

#include <angelscript.h>
#include <OGRE/OgreString.h>
#include <kobold/list.h>
#include <kobold/mutex.h>
#include "dntconfig.h"

namespace DNT
{

/*! Information about a suspended execution on a script instance. An instance
 * could be suspended for two reasons: if it takes too much time or if it 
 * called a pending action function. As each instance could be called by 
 * different entry functions (ie: step, onClick, onLoad, etc.), it can be
 * suspended more than one time at a cycle, making this class necessary. */
class SuspendedInfo : public Kobold::ListElement
{
   public:
      SuspendedInfo(asIScriptContext* context, PendingAction* action, 
            ScriptManager* manager);
      ~SuspendedInfo();

      asIScriptContext* getContext() { return context; };
      PendingAction* getPendingAction() { return action; };

      /*! Delete the action (usually when it's done), to allow the
       * resume of the suspended context on next cycle */
      void deleteAction();

      /*! \return if should resume an instance running that was suspended.
       * \note Equals to hasContextToResume() && !waitingActionEnd() */
      const bool shouldResume() const;
      /*! \return if is waiting for a pending action to end before 
       *          should be resumed */
      const bool waitingActionEnd() const;
      /*! \return if have a suspended context to resume latter */
      const bool hasContextToResume() const;

   private:
      asIScriptContext* context;
      PendingAction* action;
      ScriptManager* manager;
};

/*! DNT implements each of its scripts as an AngelScript class. Thus,
 * each one should have an instance, which information is kept by this class */
class ScriptInstance : public Kobold::ListElement
{
   public:
      ScriptInstance(asIScriptObject* obj, ScriptController* script,
            ScriptManager* manager); 
      ~ScriptInstance();

      asIScriptObject* getObject() { return obj; };
      ScriptController* getScript() { return script; };

      /*! Do a step update on the script instance */
      void step();

      /*! Add a suspended context to the script */
      void addSuspendedContext(asIScriptContext* ctx, PendingAction* act);

   protected:

      /*! Resume a suspended context from this instance
       * \param info suspended information to be resumed.
       * \return AngelScript return integer. */
      int resume(SuspendedInfo* info);

      asIScriptObject* obj; /**< AngelScript object as instance of script */
      ScriptController* script; /**< The script the object is an instance of */
      ScriptManager* manager; /**< Script manager used */

      Kobold::List suspended; /**< List of suspended contexts */
      Kobold::Mutex mutex;

};

}

#endif
