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

#ifndef _dnt_script_object_race_h
#define _dnt_script_object_race_h

#include "scriptobject.h"
#include "../../rules/ruledef.h"
#include <kobold/mutex.h>
#include <angelscript.h>

namespace DNT
{
   /*! RuleDefinition reference for script usage */
   class ScriptObjectRuleDefinition : public ScriptObject
   {
      public:
         /*! Constructor
          * \param defPtr pointer to the rule definition */
         ScriptObjectRuleDefinition(RuleDefinitionValue* defPtr);
         /*! Destructor */
         ~ScriptObjectRuleDefinition();

         void setPointer(void* newPtr);
         const bool isValid();

         /*! \return value */
         int getValue();

         /*! \return the name of the rule definition */
         Kobold::String getName();

         /*! \return identifier of the group */
         Kobold::String getGroupId();

         /*! \return Related RuleDefinition pointer, if any */
         ScriptObjectRuleDefinition* getRelated();

         /*! Register the related class */
         static void registerClass(asIScriptEngine* asEngine);
         /*! Register the related functions */
         static void registerFunctions(asIScriptEngine* asEngine);


      private:
         RuleDefinitionValue* ruleDef; /**< Referenced race */
         Kobold::Mutex mutex; /**< Mutex used */
   };
}

#endif

