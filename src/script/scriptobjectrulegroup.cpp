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

#include "scriptobjectrulegroup.h"
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectRuleGroup::ScriptObjectRuleGroup(RuleGroupAvailableInfo* groupPtr)
                  :ScriptObject(TYPE_RULE_GROUP, groupPtr->getId())
{
   ruleGroup = groupPtr;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectRuleGroup::~ScriptObjectRuleGroup()
{
}

/**************************************************************************
 *                               setPointer                               *
 **************************************************************************/
void ScriptObjectRuleGroup::setPointer(void* newPtr)
{
   mutex.lock();
   ruleGroup = static_cast<RuleGroupAvailableInfo*>(newPtr);
   mutex.unlock();
}

/**************************************************************************
 *                                 isValid                                *
 **************************************************************************/
const bool ScriptObjectRuleGroup::isValid()
{
   mutex.lock();
   bool res = ruleGroup != NULL;
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                                 addTotal                               *
 **************************************************************************/
void ScriptObjectRuleGroup::addTotal(int val)
{
   mutex.lock();
   if(ruleGroup)
   {
      ruleGroup->add(val);
   }
   mutex.unlock();
}

/**************************************************************************
 *                                 setTotal                               *
 **************************************************************************/
void ScriptObjectRuleGroup::setTotal(int val)
{
   mutex.lock();
   if(ruleGroup)
   {
      ruleGroup->setTotal(val);
   }
   mutex.unlock();
}

/**************************************************************************
 *                                 getTotal                               *
 **************************************************************************/
int ScriptObjectRuleGroup::getTotal()
{
   int res = 0;

   mutex.lock();
   if(ruleGroup)
   {
      res = ruleGroup->getTotal();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                             registerClass                              *
 **************************************************************************/
void ScriptObjectRuleGroup::registerClass(asIScriptEngine* asEngine)
{
   int r = asEngine->RegisterObjectType("RuleGroup", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("RuleGroup", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectRuleGroup, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("RuleGroup", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectRuleGroup, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}

/**************************************************************************
 *                            registerFunctions                           *
 **************************************************************************/
void ScriptObjectRuleGroup::registerFunctions(asIScriptEngine* asEngine)
{
   int r;
   
   r = asEngine->RegisterObjectMethod("RuleGroup", 
         "void addTotal(int val)", 
         asMETHOD(ScriptObjectRuleGroup, addTotal), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("RuleGroup", 
         "void setTotal(int val)", 
         asMETHOD(ScriptObjectRuleGroup, setTotal), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("RuleGroup", 
         "int getTotal()", 
         asMETHOD(ScriptObjectRuleGroup, getTotal), asCALL_THISCALL);
   assert(r >=0 );
}

