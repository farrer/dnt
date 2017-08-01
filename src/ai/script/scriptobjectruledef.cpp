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
ScriptObjectRuleDefinition::ScriptObjectRuleDefinition(
      RuleDefinitionValue* defPtr)
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
   ruleDef = static_cast<RuleDefinitionValue*>(newPtr);
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
 *                                getValue                                *
 **************************************************************************/
int ScriptObjectRuleDefinition::getValue()
{
   int res = 0;
   mutex.lock();
   if(ruleDef)
   {
      res = ruleDef->getValue();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                                getName                                 *
 **************************************************************************/
Kobold::String ScriptObjectRuleDefinition::getName()
{
   Kobold::String res;
   mutex.lock();
   if(ruleDef)
   {
      res = ruleDef->getDefinition()->getName();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                               getGroupId                               *
 **************************************************************************/
Kobold::String ScriptObjectRuleDefinition::getGroupId()
{
   Kobold::String res;
   mutex.lock();
   if(ruleDef)
   {
      res = ruleDef->getDefinition()->getGroup()->getId();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                               getRelated                               *
 **************************************************************************/
ScriptObjectRuleDefinition* ScriptObjectRuleDefinition::getRelated()
{
   ScriptObjectRuleDefinition* res = NULL;
   mutex.lock();
   if((ruleDef) && (ruleDef->getRelated()))
   {
      res = ruleDef->getRelated()->getScriptObject();
   }
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

   r = asEngine->RegisterObjectMethod("RuleDefinition",
         "int getValue()", asMETHOD(ScriptObjectRuleDefinition, getValue), 
         asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("RuleDefinition",
         "string getName()", 
         asMETHOD(ScriptObjectRuleDefinition, getName), 
         asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("RuleDefinition",
         "string getGroupId()", 
         asMETHOD(ScriptObjectRuleDefinition, getGroupId), 
         asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("RuleDefinition",
         "RuleDefinition @+ getRelated()", 
         asMETHOD(ScriptObjectRuleDefinition, getRelated), 
         asCALL_THISCALL);
   assert(r >=0 );

}


