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

#include "scriptobjectruledef.h"
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectRuleDefinition::ScriptObjectRuleDefinition(RuleDefinition* defPtr)
                  :ScriptObject(TYPE_RULE_DEFINITION, defPtr->getId())
{
   ruleDef = defPtr;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectRuleDefinition::~ScriptObjectRuleDefinition()
{
}

/**************************************************************************
 *                               setPointer                               *
 **************************************************************************/
void ScriptObjectRuleDefinition::setPointer(void* newPtr)
{
   mutex.lock();
   ruleDef = static_cast<RuleDefinition*>(newPtr);
   mutex.unlock();
}

/**************************************************************************
 *                                 isValid                                *
 **************************************************************************/
const bool ScriptObjectRuleDefinition::isValid()
{
   mutex.lock();
   bool res = ruleDef != NULL;
   mutex.unlock();

   return res;
}


/**************************************************************************
 *                            doScriptRegister                            *
 **************************************************************************/
void ScriptObjectRuleDefinition::doScriptRegister(asIScriptEngine* asEngine)
{
   int r;

   r = asEngine->RegisterObjectType("RuleDefinition", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("RuleDefinition", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectRuleDefinition, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("RuleDefinition", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectRuleDefinition, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}


