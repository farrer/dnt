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

#include "modifier.h"
#include "skills.h"
#include "bonusandsaves.h"

#include "../lang/translate.h"
#include "../core/dialog.h"
#include "../core/game.h"
#include "../map/map.h"
#include "../ai/script/scriptobject.h"
#include "../ai/script/scriptmanager.h"

#include <kobold/defparser.h>
#include <kobold/log.h>
#include <kobold/userinfo.h>

#include <OGRE/OgreWireAabb.h>

using namespace DNT;

#define THING_KEY_NAME                 "name"
#define THING_KEY_DESCRIPTION          "description"
#define THING_KEY_MODEL                "model"
#define THING_KEY_ANIMATED_MODEL       "animatedModel"
#define THING_KEY_SCALE                "scale"
#define THING_KEY_STATE                "state"
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
   this->displacement =  WALK_PER_MOVE_ACTION;
   this->walkInterval = 1.2f; 
   this->turnAroundInterval = 3.5f; 
   this->model = NULL;
   this->conversation = NULL;
   this->state = 0;
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
         "Warning: This thing implementation do not support animations");
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
            model = new Goblin::Model3d(modelName, modelFileName, 
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
                    Game::getSceneManager(), getAnimationList(),
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
      }
      else if(key == THING_KEY_STATE)
      {
         sscanf(value.c_str(), "%d", &state);
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
      else
      {
         if(!doSpecificParse(key, value))
         {
            /* Got an unknow key. File definition should be fixed. */
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Warning: unknow key '%s' at thing's file '%s'",
                  key.c_str(), fileName.c_str());
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
 *                               getState                                 *
 **************************************************************************/
int Thing::getState()
{
   return state;
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
 *                             getBonusValue                              *
 **************************************************************************/
int Thing::getBonusValue(Factor& something)
{
   Skill* s = NULL;

   if(something.getType() == MOD_TYPE_ATT)
   {
      s = sk.getSkillByString(something.getId());
      if(s)
      {
         return s->getAttributeBonus();
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
             "Unknow Attribute (Skill): ", something.getId().c_str());
         return THING_UNKNOWN_VALUE;
      }
   }
   else if(something.getType() == MOD_TYPE_SKILL)
   {
      s = sk.getSkillByString(something.getId());
      if(s)
      {
         return s->getBonus(&sk);
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Unknow Skill: '%s'", something.getId().c_str());
         return THING_UNKNOWN_VALUE;
      }
   }

   return getFactorValue(something);
}

/**************************************************************************
 *                       getFactorValuePointer                            *
 **************************************************************************/
int Thing::getFactorValue(Factor& something)
{
   Skill* s = NULL;

   if( (something.getType() == MOD_TYPE_ATT) ||
       (something.getType() == MOD_TYPE_SKILL) )
   {
      s = sk.getSkillByString(something.getId());
      if(s)
      {
         return s->getPoints();
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow skill '%s'", something.getId().c_str());
      }
   }
   else if(something.getType() == MOD_TYPE_THING)
   {
      if(something.getId() == THING_ARMATURE_CLASS)
      {
         return armatureClass;
      }
      else if(something.getId() == THING_INITIATIVE_BONUS)
      {
         return initiativeBonus;
      }
      else if(something.getId() == THING_DISPLACEMENT)
      {
         return displacement;
      }
      else if(something.getId() == THING_MAX_LIFE_POINTS)
      {
         return maxLifePoints;
      }
      else if((something.getId() == DNT_BS_LEVEL) ||
              (something.getId() == DNT_BS_FORTITUDE) ||
              (something.getId() == DNT_BS_REFLEXES) ||
              (something.getId() == DNT_BS_I_AM_NOT_A_FOOL) ||
              (something.getId() == DNT_BS_WILL) )
      {
         return curBonusAndSaves.getValue(something.getId());
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow factor '%s'", something.getId().c_str());
      }
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: Unknow factor type: ", something.getType().c_str());
   }

   return THING_UNKNOWN_VALUE;
}

/**************************************************************************
 *                           incFactorValue                               *
 **************************************************************************/
void Thing::incFactorValue(Factor& something, int inc)
{
   Skill* s = NULL;

   if( (something.getType() == MOD_TYPE_ATT) ||
       (something.getType() == MOD_TYPE_SKILL) )
   {
      s = sk.getSkillByString(something.getId());
      if(s)
      {
         s->setPoints(inc + s->getPoints());
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow skill '%s'", something.getId().c_str());
      }
   }
   else if(something.getType() == MOD_TYPE_THING)
   {
      if(something.getId() == THING_ARMATURE_CLASS)
      {
         armatureClass += inc;
      }
      else if(something.getId() == THING_INITIATIVE_BONUS)
      {
         initiativeBonus += inc;
      }
      else if(something.getId() == THING_DISPLACEMENT)
      {
         displacement += inc;
      }
      else if(something.getId() == THING_MAX_LIFE_POINTS)
      {
         maxLifePoints += inc;
      }
      else if((something.getId() == DNT_BS_LEVEL) ||
              (something.getId() == DNT_BS_FORTITUDE) ||
              (something.getId() == DNT_BS_REFLEXES) ||
              (something.getId() == DNT_BS_I_AM_NOT_A_FOOL) ||
              (something.getId() == DNT_BS_WILL) )
      {
         int v = curBonusAndSaves.getValue(something.getId());

         if(something.getId() == DNT_BS_FORTITUDE)
         {
            curBonusAndSaves.setFortitude(v + inc);
         }
         else if(something.getId() == DNT_BS_REFLEXES)
         {
         }
         else if((something.getId() == DNT_BS_I_AM_NOT_A_FOOL) ||
                 (something.getId() == DNT_BS_WILL))
         {
            curBonusAndSaves.setIAmNotAFool(v + inc);
         }
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow factor '%s'", something.getId().c_str());
      }
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: Unknow factor type: ", something.getType().c_str());
   }
}

/**************************************************************************
 *                            getCurBonusAndSaves                         *
 **************************************************************************/
BonusAndSaves* Thing::getCurBonusAndSaves()
{
   return &curBonusAndSaves;
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
 *                                  getSkills                             *
 **************************************************************************/
Skills* Thing::getSkills()
{
   return &sk;
}

/**************************************************************************
 *                                   doCheck                              *
 **************************************************************************/
bool Thing::doCheck(Kobold::String stateToCheck, int difficulty)
{
   Skill* skl;
   bool couldCheck = false;
   bool res;

   /* First, let's test as skill */
   skl = sk.getSkillByString(stateToCheck);
   if(skl != NULL)
   {
      return sk.doSkillCheck(skl, difficulty);
   }

   /* Nope, so let's test as a bonus and Save */
   res = curBonusAndSaves.doCheck(stateToCheck, difficulty, &couldCheck);
   if(couldCheck)
   {
      return res;
   }

   /* Unknow! */
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Warning: thing::doCheck - Unknown state to check: '%s'",
         stateToCheck.c_str());
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
 *                             static members                             *
 **************************************************************************/
std::map<Kobold::String, int> Thing::namesMap;

