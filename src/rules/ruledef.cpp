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

#include "ruledef.h"
#include "character.h"
#include "../lang/translate.h"
#include "../core/game.h"
#include "../script/scriptobjectruledef.h"
#include "../script/scriptobjectrulegroup.h"
#include "../script/scriptobjectcharacter.h"
#include "../script/scriptmanager.h"
#include "../script/rulescript.h"
#include "../script/ruledefscript.h"
#include <kobold/defparser.h>
#include <kobold/log.h>
#include <assert.h>
using namespace DNT;

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               RuleGroup                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
RuleGroup::RuleGroup(Kobold::String id)
{
   this->id = id;
   this->type = TYPE_SELECTABLE;
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
RuleGroup::~RuleGroup()
{
}

/******************************************************************
 *                      insertRuleDefinition                      *
 ******************************************************************/
void RuleGroup::insert(RuleDefinition* ruleDef)
{
   definitions.insert(ruleDef);
}

/******************************************************************
 *                         getDefinition                          *
 ******************************************************************/
RuleDefinition* RuleGroup::getDefinition(const Kobold::String id)
{
   RuleDefinition* cur = static_cast<RuleDefinition*>(definitions.getFirst());
   for(int i=0; i < definitions.getTotal(); i++)
   {
      if(cur->getId() == id)
      {
         return cur;
      }
      cur = static_cast<RuleDefinition*>(cur->getNext());
   }

   return NULL;
}

/******************************************************************
 *                              setType                           *
 ******************************************************************/
void RuleGroup::setType(const RuleGroupType type)
{
   this->type = type;
}
 
/******************************************************************
 *                            setName                             *
 ******************************************************************/
void RuleGroup::setName(Kobold::String name)
{
   this->name = name;
}

/******************************************************************
 *                         setDescription                         *
 ******************************************************************/
void RuleGroup::setDescription(Kobold::String desc)
{
   this->description = desc;
}

/******************************************************************
 *                            populate                            *
 ******************************************************************/
void RuleGroup::populate(RuleGroupAvailableInfo* groupInfo, 
      Kobold::List* groupsInfo)
{
   RuleDefinition* def = static_cast<RuleDefinition*>(definitions.getFirst());
   for(int i=0; i < definitions.getTotal(); i++)
   {
      groupInfo->insert(def, groupsInfo);
      def = static_cast<RuleDefinition*>(def->getNext());
   }
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                           RulePreRequisite                             //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
RulePreRequisite::RulePreRequisite(RuleDefinition* requisite, int minValue)
{
   this->required = requisite;
   this->minValue = minValue;
}

/******************************************************************
 *                             Destructor                         *
 ******************************************************************/
RulePreRequisite::~RulePreRequisite()
{
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                            RuleDefinition                              //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
RuleDefinition::RuleDefinition(Kobold::String id)
{
   this->group = NULL;
   this->related = NULL;
   this->id = id;
   this->image = NULL;
   this->script = NULL;
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
RuleDefinition::~RuleDefinition()
{
   if(script)
   {
      Game::getScriptManager()->removeInstance(script);
   }
   if(image != NULL)
   {
      delete image;
   }
}

/******************************************************************
 *                            setGroup                            *
 ******************************************************************/
void RuleDefinition::setGroup(const RuleGroup* ruleGroup)
{
   this->group = ruleGroup;
}

/******************************************************************
 *                            setRelated                          *
 ******************************************************************/
void RuleDefinition::setRelated(const RuleDefinition* ruleDef)
{
   this->related = ruleDef;
}

/******************************************************************
 *                            loadImage                           *
 ******************************************************************/
void RuleDefinition::loadImage(Kobold::String imageFilename)
{
   assert(image == NULL);
   image = Farso::Controller::loadImageToSurface(imageFilename);
}

/******************************************************************
 *                              setName                           *
 ******************************************************************/
void RuleDefinition::setName(Kobold::String name)
{
   this->name = name;
}

/******************************************************************
 *                          setDescription                        *
 ******************************************************************/
void RuleDefinition::setDescription(Kobold::String desc)
{
   this->description = desc;
}

/******************************************************************
 *                             isUsable                           *
 ******************************************************************/
const bool RuleDefinition::isUsable() const
{
   if(script != NULL)
   {
      RuleDefinitionScript* ctrl = static_cast<RuleDefinitionScript*>(
            script->getScript());
      return ctrl->getUseFunction() != NULL;
   }

   return false;
}

/******************************************************************
 *                           onInsert                             *
 ******************************************************************/
void RuleDefinition::onInsert(Character* owner)
{
   if(script)
   {
      script->callOnInsert(static_cast<ScriptObjectCharacter*>(
               owner->getScriptObject()));
   }
}

/******************************************************************
 *                            setScript                           *
 ******************************************************************/
void RuleDefinition::setScript(const Kobold::String& scriptFile)
{
   assert(script == NULL);
   Game::getScriptManager()->createRuleDefinitionScriptInstance(scriptFile);
}

/******************************************************************
 *                              use                               *
 ******************************************************************/
void RuleDefinition::use(Character* owner, Character* target)
{
   if(script)
   {
      script->callUse(
            static_cast<ScriptObjectCharacter*>(owner->getScriptObject()),
            static_cast<ScriptObjectCharacter*>(target->getScriptObject()));
   }
}
 
/******************************************************************
 *                        addPreRequisite                         *
 ******************************************************************/
void RuleDefinition::addPreRequisite(RulePreRequisite* preRequisite)
{
   this->requisites.insert(preRequisite);
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                         RuleDefinitionValue                            //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
RuleDefinitionValue::RuleDefinitionValue(RuleDefinition* def, 
      RuleDefinitionValue* related)
{
   char buf[256];
   RuleDefinitionValue::count++;
   sprintf(buf, "%s_%d", def->getId().c_str(), RuleDefinitionValue::count);
   this->id = buf;
   this->ruleDef = def;
   assert((related == NULL && def->getRelated() == NULL) ||
          (related != NULL && related->getDefinition() == def->getRelated()));
   this->related = related;
   this->value = 0;
   this->scriptObject = new ScriptObjectRuleDefinition(this);
   this->scriptObject->addReference();
   Game::getScriptManager()->insertScriptObject(this->scriptObject);
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
RuleDefinitionValue::~RuleDefinitionValue()
{
   scriptObject->release();
}

/******************************************************************
 *                               add                              *
 ******************************************************************/
void RuleDefinitionValue::add(int v)
{
   this->value += v;
}

/******************************************************************
 *                           setValue                             *
 ******************************************************************/
void RuleDefinitionValue::setValue(int v)
{
   this->value = v;
}
int RuleDefinitionValue::count = 0;

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       RuleGroupAvailableInfo                           //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
RuleGroupAvailableInfo::RuleGroupAvailableInfo(RuleGroup* group)
{
   char buf[256];
   RuleGroupAvailableInfo::count++;
   sprintf(buf, "%s_%d", group->getId().c_str(), 
         RuleGroupAvailableInfo::count);
   this->id = buf;
   this->group = group;
   this->total = 0;
   this->scriptObject = new ScriptObjectRuleGroup(this);
   this->scriptObject->addReference();
   Game::getScriptManager()->insertScriptObject(this->scriptObject);
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
RuleGroupAvailableInfo::~RuleGroupAvailableInfo()
{
   this->scriptObject->release();
}

/******************************************************************
 *                               add                              *
 ******************************************************************/
void RuleGroupAvailableInfo::add(int v)
{
   this->total += v;
}

/******************************************************************
 *                           setTotal                             *
 ******************************************************************/
void RuleGroupAvailableInfo::setTotal(int v)
{
   this->total = v;
}

/******************************************************************
 *                      getDefinitionValue                        *
 ******************************************************************/
RuleDefinitionValue* RuleGroupAvailableInfo::getDefinitionValue(
      const Kobold::String id)
{
   RuleDefinitionValue* cur = static_cast<RuleDefinitionValue*>(
         defValues.getFirst());
   for(int i=0; i < defValues.getTotal(); i++)
   {
      if(cur->getDefinition()->getId() == id)
      {
         return cur;
      }
      cur = static_cast<RuleDefinitionValue*>(cur->getNext());
   }

   return NULL;
}

/******************************************************************
 *                            insert                              *
 ******************************************************************/
RuleDefinitionValue* RuleGroupAvailableInfo::insert(RuleDefinition* ruleDef, 
      Kobold::List* groupsInfo)
{
   assert(ruleDef != NULL);
   assert(getDefinitionValue(ruleDef->getId()) == NULL);

   RuleDefinitionValue* related = NULL;
   if(ruleDef->getRelated() != NULL)
   {
      /* must search for the related value */
      const RuleGroup* relGroup = ruleDef->getRelated()->getGroup();
      RuleGroupAvailableInfo* curGroup = static_cast<
         RuleGroupAvailableInfo*>(groupsInfo->getFirst());
      for(int i=0; i < groupsInfo->getTotal(); i++)
      {
         if(curGroup->getGroup() == relGroup)
         {
            related = curGroup->getDefinitionValue(
                  ruleDef->getRelated()->getId());
            if(!related)
            {
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                     "Error: Couldn't found related RuleDefinitionValue '%s'",
                     ruleDef->getRelated()->getId().c_str());
            }
            break;
         }
         curGroup = static_cast<RuleGroupAvailableInfo*>(curGroup->getNext());
      }
   }
   
   RuleDefinitionValue* res = new RuleDefinitionValue(ruleDef, related);
   defValues.insert(res);

   return res;
}

/******************************************************************
 *                             save                               *
 ******************************************************************/
bool RuleGroupAvailableInfo::save(std::ofstream& file)
{
   /* Write the group total values */
   file << group->getId() << " = " << total << std::endl;

   /* Write each definition value */
   RuleDefinitionValue* cur = static_cast<RuleDefinitionValue*>(
         defValues.getFirst());
   for(int i=0; i < defValues.getTotal(); i++)
   {
      file << cur->getDefinition()->getId() << " = " 
           << cur->getValue() << std::endl;
      cur = static_cast<RuleDefinitionValue*>(cur->getNext());
   }

   return true;
}

/******************************************************************
 *                        static members                          *
 ******************************************************************/
int RuleGroupAvailableInfo::count = 0;


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                Rules                                   //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
void Rules::init(const Kobold::String filename)
{
   Kobold::Log::add("Initing Rules...");
   assert(groups == NULL);
   groups = new Kobold::List();
   if(!load(filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Couldn't load rules file: '%s'", filename.c_str());
      assert(false);
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
void Rules::finish()
{
   Kobold::Log::add("Finishing Rules...");
   if(groups != NULL)
   {
      delete groups;
      groups = NULL;
   }
   if(scriptInstance)
   {
      Game::getScriptManager()->removeInstance(scriptInstance);
      scriptInstance = NULL;
   }
}

/******************************************************************
 *                            getGroup                            *
 ******************************************************************/
RuleGroup* Rules::getGroup(const Kobold::String id)
{
   RuleGroup* cur = static_cast<RuleGroup*>(groups->getFirst());
   for(int i=0; i < groups->getTotal(); i++)
   {
      if(cur->getId() == id)
      {
         return cur;
      }
      cur = static_cast<RuleGroup*>(cur->getNext());
   }

   return NULL;
}

/******************************************************************
 *                        getDefinition                           *
 ******************************************************************/
RuleDefinition* Rules::getDefinition(const Kobold::String groupId,
      const Kobold::String id)
{
   RuleGroup* group = getGroup(groupId);
   if(group)
   {
      return group->getDefinition(id);
   }

   return NULL;
}

/******************************************************************
 *                        getDefinition                           *
 ******************************************************************/
RuleDefinition* Rules::getDefinition(Kobold::String id)
{
   RuleGroup* cur = static_cast<RuleGroup*>(groups->getFirst());
   for(int i=0; i < groups->getTotal(); i++)
   {
      RuleDefinition* def = cur->getDefinition(id);
      if(def)
      {
         return def;
      }
      cur = static_cast<RuleGroup*>(cur->getNext());
   }

   return NULL;
}

/******************************************************************
 *                        checkNotNull                            *
 ******************************************************************/
void Rules::checkNotNull(void* info, const Kobold::String& filename, 
      const Kobold::String key, const Kobold::String infoDecl)
{
   if(info == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: '%s': %s without %s!", 
            filename.c_str(), key.c_str(), infoDecl.c_str());
   }
   assert(info != NULL);
}

/******************************************************************
 *                  checkIsIsNullOrHaveGroup                      *
 ******************************************************************/
void Rules::checkIsNullOrHaveGroup(RuleDefinition* curDef, 
      const Kobold::String& filename)
{
   if((curDef != NULL) && (curDef->getGroup()  == NULL))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: '%s': RuleDefinition '%s' is without a defined RuleGroup!", 
            filename.c_str(), curDef->getName().c_str());
   }
   assert((curDef == NULL) || (curDef->getGroup() != NULL));
}

/******************************************************************
 *                              load                              *
 ******************************************************************/
bool Rules::load(const Kobold::String filename)
{
   Kobold::DefParser defParser;

   if(!defParser.load(filename))
   {
      return false;
   }

   Kobold::String key, value;
   RuleGroup* curGroup = NULL;
   RuleGroup* foundGroup = NULL;
   RuleDefinition* curDef = NULL;
   while(defParser.getNextTuple(key, value))
   {
      /* Rule Group */
      if(key == "ruleScript")
      {
         assert(scriptInstance == NULL);
         /* Create the instance of the RuleController script */
         scriptInstance = 
            Game::getScriptManager()->createRuleScriptInstance(value);
      }
      else if(key == "ruleGroup")
      {
         curDef = NULL;
         curGroup = new RuleGroup(value);
         groups->insertAtEnd(curGroup);
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
               "\tAdded group '%s'", curGroup->getId().c_str());
      }
      else if(key == "ruleGroupName")
      {
         checkNotNull(curGroup, filename, key, "ruleGroup");
         curGroup->setName(translateDataString(value));
      }
      else if(key == "ruleGroupDesc")
      {
         checkNotNull(curGroup, filename, key, "ruleGroup");
         curGroup->setDescription(translateDataString(value));
      }
      else if(key == "ruleGroupType")
      {
         checkNotNull(curGroup, filename, key, "ruleGroup");
         if(value == "value")
         {
            curGroup->setType(RuleGroup::TYPE_VALUE);
         }
         else if(value == "selectable")
         {
            curGroup->setType(RuleGroup::TYPE_SELECTABLE);
         }
         else if(value == "level_selectable")
         {
            curGroup->setType(RuleGroup::TYPE_LEVEL_SELECTABLE);
         }
         else if(value == "calculated")
         {
            curGroup->setType(RuleGroup::TYPE_CALCULATED);
         }
         else
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: '%s': unexpected RuleGroup type '%s'!",
               filename.c_str(), value.c_str());
            assert(false);
         }
      }
      /* RuleDefinition */
      else if(key == "ruleDef" || key == "ruleDefinition")
      {
         curGroup = NULL;
         checkIsNullOrHaveGroup(curDef, filename);
         curDef = new RuleDefinition(value);
      }
      else if(key == "ruleDefName" || key == "ruleDefinitionName")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         curDef->setName(translateDataString(value));
      }
      else if(key == "ruleDefDesc" || key == "ruleDefinitionDesc")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         curDef->setDescription(translateDataString(value));
      }
      else if(key == "ruleDefImage" || key == "ruleDefinitionImage")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         curDef->loadImage(value);
      }
      else if(key == "ruleDefScript" || key == "ruleDefinitionScript")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         curDef->setScript(value);
      }
      else if(key == "ruleDefRelated")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         RuleDefinition* related = getDefinition(value);
         if(!related)
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: '%s' couldn't find or not yet declared related '%s'",
                  filename.c_str(), value.c_str());
         }
         assert(related != NULL);
         curDef->setRelated(related);
      }
      else if(key == "ruleDefGroup" || key == "ruleDefinitionGroup")
      {
         checkNotNull(curDef, filename, key, "ruleDef");
         if((foundGroup == NULL) || (foundGroup->getId() != value))
         {
            foundGroup = getGroup(value);
            if(foundGroup == NULL)
            {
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: '%s': Unknown RuleGroup '%s' used at a RuleDef!",
                  filename.c_str(), value.c_str());
            }
         }
         assert(foundGroup != NULL);
         curDef->setGroup(foundGroup);
         foundGroup->insert(curDef);
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
               "\tAdded '%s' to group '%s'", curDef->getId().c_str(),
               foundGroup->getId().c_str());
      }
   }
   
   checkIsNullOrHaveGroup(curDef, filename);
   if(!scriptInstance)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Rules must have a defined script!");
   }
   assert(scriptInstance);

   return true;
}

/******************************************************************
 *                            populate                            *
 ******************************************************************/
void Rules::populate(Kobold::List* groupInfoList)
{
   assert(groupInfoList->getTotal() == 0);
   RuleGroup* group = static_cast<RuleGroup*>(groups->getFirst());
   for(int i = 0; i < groups->getTotal(); i++)
   {
      RuleGroupAvailableInfo* groupInfo = new RuleGroupAvailableInfo(group);
      /* Check if need to add all definitions (!selectable) */
      if((group->getType() == RuleGroup::TYPE_VALUE) || 
         (group->getType() == RuleGroup::TYPE_CALCULATED))
      {
         /* Insert all definitions */
         group->populate(groupInfo, groupInfoList);
      }
      /* insert it */
      groupInfoList->insertAtEnd(groupInfo);
      group = static_cast<RuleGroup*>(group->getNext());
   }
}

/******************************************************************
 *                        getScriptInstance                       *
 ******************************************************************/
RuleScriptInstance* Rules::getScriptInstance()
{
   return scriptInstance;
}

/******************************************************************
 *                            members                             *
 ******************************************************************/
Kobold::List* Rules::groups = NULL;
RuleScriptInstance* Rules::scriptInstance = NULL;


