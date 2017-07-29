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

#ifndef _dnt_script_object_class_h
#define _dnt_script_object_class_h

#include "scriptobject.h"
#include "../../rules/ruledef.h"
#include <kobold/mutex.h>
#include <angelscript.h>

namespace DNT
{
   /*! RuleGroup reference for script usage */
   class ScriptObjectRuleGroup : public ScriptObject
   {
      public:
         /*! Constructor
          * \param groupPtr pointer to the group */
         ScriptObjectRuleGroup(RuleGroup* groupPtr);
         /*! Destructor */
         ~ScriptObjectRuleGroup();

         void setPointer(void* newPtr);
         const bool isValid();

         /*! Register its type and functions */
         static void doScriptRegister(asIScriptEngine* asEngine);

      private:
         RuleGroup* ruleGroup; /**< Referenced group */
         Kobold::Mutex mutex; /**< Mutex used */
   };
}

#endif

