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


#include "thing.h"

#include "../lang/translate.h"
#include "../core/dialog.h"
#include "../core/game.h"
#include "../map/map.h"
#include "../script/scriptobject.h"
#include "../script/scriptmanager.h"

#include <kobold/defparser.h>
#include <kobold/log.h>
#include <kobold/userinfo.h>

#include <OGRE/OgreWireAabb.h>
#include <assert.h>

using namespace DNT;

#define THING_KEY_NAME                 "name"
#define THING_KEY_DESCRIPTION          "description"
#define THING_KEY_MODEL                "model"
#define THING_KEY_ANIMATED_MODEL       "animatedModel"
#define THING_KEY_SCALE                "scale"
#define THING_KEY_STATE                "state"
#define THING_KEY_DIFFICULTY           "difficulty"
#define THING_KEY_HARDNESS             "hardness"
#define THING_KEY_WALKABLE             "walkable"
#define THING_KEY_WALK_INTERVAL        "walkInterval"
#define THING_KEY_TURN_AROUND_INTERVAL "turnAroundInterval"
#define THING_KEY_PORTRAIT             "portrait"
#define THING_KEY_CONVERSATION         "conversation"
#define THING_KEY_REDEFINE_BOUNDS      "redefineBounds"

#define THING_VALUE_TRUE               "true"
#define THING_VALUE_FALSE              "false"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Thing::Thing(ThingType type, int totalAnimations)
{
   this->maxLifePoints = 0;
   this->lifePoints = 0;
   this->armatureClass = 0;
   this->initiativeBonus = 0;
   this->displacement = 3.0f;
   this->walkInterval = 0.06f; 
   this->turnAroundInterval = 3.5f; 
   this->model = NULL;
   this->conversation = NULL;
   this->state = 0;
   this->difficulty = 0;
   this->hardness = 0;
   this->walkable = false;
   this->currentEnemy = NULL;
   this->psychoState = PSYCHO_NEUTRAL;
   this->portraitFile = "";
   this->filename = "";
   this->thingType = type;
   this->totalAnimations = totalAnimations;
   this->colElements = new Kobold::List();
   this->scriptObject = NULL;
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Thing::~Thing()
{
   if(scriptObject)
   {
      /* Tell the script object that its pointer is no longer valid */
      scriptObject->setPointer(NULL);
   }

   /* Clean conversation, if loaded */
   if(conversation)
   {
      delete conversation;
   }
   /* Delete model, if defined */
   if(model)
   {
      delete model;
   }
   if(colElements)
   {
      delete colElements;
   }
}

/**************************************************************************
 *                             getThingType                               *
 **************************************************************************/
Thing::ThingType Thing::getThingType()
{
   return thingType;
}

/**************************************************************************
 *                           getArmatureClass                             *
 **************************************************************************/
int Thing::getArmatureClass()
{
   return armatureClass;
}

/**************************************************************************
 *                           setArmatureClass                             *
 **************************************************************************/
void Thing::setArmatureClass(int value)
{
   armatureClass = value;
}

/**************************************************************************
 *                           getWalkInterval                              *
 **************************************************************************/
float Thing::getWalkInterval()
{
   return walkInterval;
}

/**************************************************************************
 *                        getTurnAroundInterval                           *
 **************************************************************************/
float Thing::getTurnAroundInterval()
{
   return turnAroundInterval;
}

/**************************************************************************
 *                          getAnimationList                              *
 **************************************************************************/
Kobold::String* Thing::getAnimationList()
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
         "Warning: This thing implementation does not support animations");
   assert(false);
   return NULL;
}

/**************************************************************************
 *                                  load                                  *
 **************************************************************************/
bool Thing::load(Kobold::String fileName,
      Goblin::Model3d::Model3dType modelType, bool fullPath)
{
   Kobold::DefParser defParser;
   Kobold::String key, value, modelName;
   float scale = 1.0f;

   if(!defParser.load(fileName, fullPath))
   {
      return false;
   }
   filename = fileName;
   
   populateWithRules();

   while(defParser.getNextTuple(key, value))
   {
      if(key == THING_KEY_NAME)
      {
         untranslatedName = value;
         name = translateDataString(value);
         
         /* Define unique name */
         std::map<Kobold::String,int>::iterator it = namesMap.find(value);
         int counter = 0; 
         if(it != namesMap.end())
         {
            counter = it->second;
         }
         if(counter > 0)
         {
            Ogre::StringStream ss;
            ss << value;
            ss << counter;
            modelName = ss.str();
         }
         else
         {
            modelName = value;
         }
         /* Define new counter for value */
         namesMap[value] = counter + 1;
      }
      else if(key == THING_KEY_DESCRIPTION)
      {
         description = translateDataString(value);
      }
      else if(key == THING_KEY_MODEL)
      {
         assert(model == NULL);
         assert(totalAnimations == 0);
         if(model == NULL)
         {
            modelFileName = value;
            model = new Goblin::Model3d(modelName, modelFileName, "models", 
                  Game::getSceneManager(), modelType);
         }
      }
      else if(key == THING_KEY_ANIMATED_MODEL)
      {
         assert(model == NULL);
         assert(totalAnimations > 0);
         if(model == NULL)
         {
            modelFileName = value;
            model = new Goblin::AnimatedModel3d(modelName, modelFileName, 
                  "models", Game::getSceneManager(), getAnimationList(),
                  totalAnimations);
         }
      }
      else if(key == THING_KEY_REDEFINE_BOUNDS)
      {
         /* Redefine the bounding, to be squared. This is needed for humanoid
          * models, where the bounding box of its 'pose' is a lot larger than
          * the one for walking or idle (due to arms opened on pose, and down
          * or idle and walk). */
         assert(model != NULL);
         float factor=0.0f;
         sscanf(value.c_str(), "%f", &factor);
         Ogre::Aabb aabb = model->getItem()->getLocalAabb();
         if(aabb.mHalfSize.x < aabb.mHalfSize.z)
         {
            aabb.mHalfSize.z = aabb.mHalfSize.x * factor;
         }
         else
         {
            aabb.mHalfSize.x = aabb.mHalfSize.z * factor;
         }
         model->getItem()->setLocalAabb(aabb);
      }
      else if(key == THING_KEY_SCALE)
      {
         /* Let's load its scale (to apply later) */
         sscanf(value.c_str(), "%f", &scale);
         /* Must redefine its bounds applying the scale factor */
         Ogre::Aabb aabb = model->getItem()->getLocalAabb();
         aabb.mHalfSize *= scale;
         model->getItem()->setLocalAabb(aabb);
      }
      else if(key == THING_KEY_STATE)
      {
         sscanf(value.c_str(), "%d", &state);
      }
      else if(key == THING_KEY_HARDNESS)
      {
         sscanf(value.c_str(), "%d", &hardness);
      }
      else if(key == THING_KEY_DIFFICULTY)
      {
         sscanf(value.c_str(), "%d", &difficulty);
      }
      else if(key == THING_KEY_WALKABLE)
      {
         walkable = (value == THING_VALUE_TRUE);
      }
      else if(key == THING_KEY_WALK_INTERVAL)
      {
         sscanf(value.c_str(), "%f", &walkInterval);
      }
      else if(key == THING_KEY_TURN_AROUND_INTERVAL)
      {
         sscanf(value.c_str(), "%f", &turnAroundInterval);
      }
      else if(key == THING_KEY_PORTRAIT)
      {
         portraitFile = value;
      }
      else if(key == THING_KEY_CONVERSATION)
      {
         setConversationFile(value);
      }
      else if(!doSpecificParse(key, value))
      {
         /* Check key as a rule definition */
         RuleDefinition* ruleDef = Rules::getDefinition(key);
         if(ruleDef)
         {
            RuleDefinitionValue* val = NULL;
            if((ruleDef->getGroup()->getType() == RuleGroup::TYPE_VALUE) || 
               (ruleDef->getGroup()->getType() == RuleGroup::TYPE_CALCULATED))
            {
               /* Already have the definition, just set its value */
               val = getRuleDefinition(ruleDef); 
            }
            else
            {
               /* Do not have it, must add and set its value */
               RuleGroupAvailableInfo* group = getRuleGroup(
                     ruleDef->getGroup()->getId());
               val = group->insert(ruleDef, &ruleGroups);

            }
            assert(val != NULL);
            int iValue = 0;
            sscanf(value.c_str(), "%d", &iValue);
            val->setValue(iValue);
         }
         else
         {
            /* Check key as RuleGroupId */
            RuleGroupAvailableInfo* group = getRuleGroup(key);
            if(group)
            {
               /* Set its group total */
               int iValue = 0;
               sscanf(value.c_str(), "%d", &iValue);
               group->setTotal(iValue);
            }
            else
            {
               /* Got an unknow key. File definition should be fixed. */
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                     "Warning: unknow key '%s' at thing's file '%s'",
                     key.c_str(), fileName.c_str());
            }
         }
      }
   }


   if(model != NULL)
   {
      /* Apply our scale */
      model->setScale(scale, scale, scale);
      /* Define our 'playable character walking bounding box' */
      walkableBounds = model->getItem()->getLocalAabb();
      if(walkableBounds.mHalfSize.x > walkableBounds.mHalfSize.z)
      {
         walkableBounds.mHalfSize.z = walkableBounds.mHalfSize.x;
      }
      else
      {
         walkableBounds.mHalfSize.x = walkableBounds.mHalfSize.z;
      }
      /* And cache our vertices for depth collision check. */
      model->updateCachedMeshInformation();
      /* Uncomment for bounding box debug. XXX: Note that will leak (lazy!). */
      //Ogre::WireAabb* wire = Game::getSceneManager()->createWireAabb();
      //wire->track(model->getItem());
   }

   return true;
}

/**************************************************************************
 *                           populateWithRules                            *
 **************************************************************************/
void Thing::populateWithRules()
{
   Rules::populate(&ruleGroups);
}

/**************************************************************************
 *                           setInitialPosition                           *
 **************************************************************************/
void Thing::setInitialPosition(Ogre::Vector3 pos)
{
   initialPos = pos;
   model->setPositionNow(pos);
   scriptObject = Game::getScriptManager()->getAndDefinePointer(filename, 
      pos, this);
}

/**************************************************************************
 *                                   save                                 *
 **************************************************************************/
bool Thing::save(Kobold::String filename, bool fullPath)
{
   bool success = true;
   Kobold::String filePath;
   std::ofstream file;

   /* Define file with its path */
   if(fullPath)
   {
      filePath = filename;
   }
   else
   {
      filePath = Kobold::UserInfo::getUserHome() + filename;
   }

   /* Let's try to open it */
   file.open(filePath.c_str(), std::ios::out | std::ios::binary);
   if(!file)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Couldn't open thing file '%s' for saving it.", filePath.c_str());
      return false;
   }

   /* write thing's values */
   file << THING_KEY_NAME << " = gettext(\"" << name << "\")" << std::endl;
   file << THING_KEY_DESCRIPTION << " = gettext(\"" 
        << description << "\")" << std::endl;
   file << THING_KEY_MODEL << " = " << modelFileName << std::endl;
   file << THING_KEY_STATE << " = " << state << std::endl;
   file << THING_KEY_WALKABLE << " = " 
        << (walkable ? THING_VALUE_TRUE : THING_VALUE_FALSE) << std::endl;
   file << THING_KEY_CONVERSATION << " = " << conversationFile << std::endl;

   /* Write the RuleDefinitions values */
   RuleGroupAvailableInfo* group = static_cast<RuleGroupAvailableInfo*>(
         ruleGroups.getFirst());
   for(int g = 0; g < ruleGroups.getTotal(); g++)
   {
      success |= group->save(file);   
      group = static_cast<RuleGroupAvailableInfo*>(group->getNext());
   }

   /* Save specific implementation values */
   success |= doSpecificSave(file);

   /* Close the file and done */
   file.close();

   return success;
}

/**************************************************************************
 *                               update                                   *
 **************************************************************************/
void Thing::update()
{
   if(model)
   {
      model->update();
   }
}

/**************************************************************************
 *                             getModel3d                                 *
 **************************************************************************/
Goblin::Model3d* Thing::getModel()
{
   return model;
}

/**************************************************************************
 *                             getFileName                                *
 **************************************************************************/
Kobold::String Thing::getFilename()
{
   return filename;
}

/**************************************************************************
 *                               getName                                  *
 **************************************************************************/
Kobold::String Thing::getName()
{
   return name;
}

/**************************************************************************
 *                               getDescription                           *
 **************************************************************************/
Kobold::String Thing::getDescription()
{
   return description;
}

/**************************************************************************
 *                            setDifficulty                               *
 **************************************************************************/
void Thing::setDifficulty(int value)
{
   difficulty = value;
}

/**************************************************************************
 *                              setHardness                               *
 **************************************************************************/
void Thing::setHardness(int value)
{
   hardness = value;
}

/**************************************************************************
 *                               setState                                 *
 **************************************************************************/
void Thing::setState(int state)
{
   this->state = state;
}

/**************************************************************************
 *                                isWalkable                              *
 **************************************************************************/
bool Thing::isWalkable()
{
   return walkable;
}

/**************************************************************************
 *                               setWalkable                              *
 **************************************************************************/
void Thing::setWalkable(bool canWalkThrough)
{
   walkable = canWalkThrough;
}

/**************************************************************************
 *                             getCurrentEnemy                            *
 **************************************************************************/
Thing* Thing::getCurrentEnemy()
{
   return currentEnemy;
}

/**************************************************************************
 *                             setCurrentEnemy                            *
 **************************************************************************/
void Thing::setCurrentEnemy(Thing* target)
{
   this->currentEnemy = target;
}

/**************************************************************************
 *                             getPsychoState                             *
 **************************************************************************/
Thing::PsychoState Thing::getPsychoState()
{
   return psychoState;
}

/**************************************************************************
 *                             setPsychoState                             *
 **************************************************************************/
void Thing::setPsychoState(PsychoState state)
{
   psychoState = state;
}

/**************************************************************************
 *                            getConversation                             *
 **************************************************************************/
Conversation* Thing::getConversation()
{
   if((conversation == NULL) && (hasConversationFile()))
   {
      /* Create and load our conversation to use */
      conversation = new Conversation();
      conversation->load(conversationFile);
      conversation->setOwner(this, Game::getCurrentMap()->getFilename());
   }
   return conversation;
}

/**************************************************************************
 *                            hasConversationFile                         *
 **************************************************************************/
bool Thing::hasConversationFile()
{
   return !conversationFile.empty();
}

/**************************************************************************
 *                            setConversationFile                         *
 **************************************************************************/
void Thing::setConversationFile(Kobold::String fileName)
{
   conversationFile = fileName;
}

/**************************************************************************
 *                                   doCheck                              *
 **************************************************************************/
bool Thing::doCheck(RuleDefinition* ruleDef, int difficulty)
{
   RuleDefinitionValue* defVal = getRuleDefinition(ruleDef);
   if(defVal)
   {
      return Rules::getScriptInstance()->callRollValue(defVal, difficulty);
   }
   return false;
}

/**************************************************************************
 *                                   doCheck                              *
 **************************************************************************/
bool Thing::doCheck(RuleDefinition* ruleDef, RuleDefinitionValue* against)
{
   RuleDefinitionValue* defVal = getRuleDefinition(ruleDef);
   if(defVal)
   {
      return Rules::getScriptInstance()->callRoll(defVal, against);
   }

   return false;
}

/******************************************************
 *                    getLifePoints                   *
 ******************************************************/
int Thing::getLifePoints()
{
   return lifePoints;
}

/******************************************************
 *                     addLifePoints                  *
 ******************************************************/
void Thing::addLifePoints(int points)
{
   lifePoints += points;
   if(lifePoints > maxLifePoints)
   {
      lifePoints = maxLifePoints;
   }
}

/******************************************************
 *                   setLifePoints                    *
 ******************************************************/
void Thing::setLifePoints(int points)
{
   lifePoints = points;
}

/******************************************************
 *                  getMaxLifePoints                  *
 ******************************************************/
int Thing::getMaxLifePoints()
{
   return maxLifePoints;
}

/******************************************************
 *                  setMaxLifePoints                  *
 ******************************************************/
void Thing::setMaxLifePoints(int points)
{
   maxLifePoints = points;
   lifePoints = maxLifePoints;
}

/**************************************************************************
 *                          getRuleDefinition                             *
 **************************************************************************/
RuleDefinitionValue* Thing::getRuleDefinition(const Kobold::String id)
{
   RuleGroupAvailableInfo* cur = static_cast<RuleGroupAvailableInfo*>(
         ruleGroups.getFirst());
   for(int i = 0; i < ruleGroups.getTotal(); i++)
   {
      RuleDefinitionValue* val = cur->getDefinitionValue(id);
      if(val)
      {
         return val;
      }
      cur = static_cast<RuleGroupAvailableInfo*>(cur->getNext());
   }
   return NULL;
}

/**************************************************************************
 *                          getRuleDefinition                             *
 **************************************************************************/
RuleDefinitionValue* Thing::getRuleDefinition(const Kobold::String groupId, 
      const Kobold::String id)
{
   RuleGroupAvailableInfo* group = getRuleGroup(groupId);
   if(group)
   {
      return group->getDefinitionValue(id);
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: no RuleGroup with identifier '%s'", groupId.c_str());
   }

   return NULL;
}

/**************************************************************************
 *                          getRuleDefinition                             *
 **************************************************************************/
RuleDefinitionValue* Thing::getRuleDefinition(RuleDefinition* def)
{
   if(def)
   {
      return getRuleDefinition(def->getGroup()->getId(), def->getId());
   }
   return NULL;
}

/**************************************************************************
 *                            getRuleGroup                                *
 **************************************************************************/
RuleGroupAvailableInfo* Thing::getRuleGroup(const Kobold::String id)
{
   RuleGroupAvailableInfo* cur = static_cast<RuleGroupAvailableInfo*>(
         ruleGroups.getFirst());
   for(int i = 0; i < ruleGroups.getTotal(); i++)
   {
      if(cur->getGroup()->getId() == id)
      {
         return cur;
      }
      cur = static_cast<RuleGroupAvailableInfo*>(cur->getNext());
   }
   return NULL;
}

/**************************************************************************
 *                             static members                             *
 **************************************************************************/
std::map<Kobold::String, int> Thing::namesMap;

