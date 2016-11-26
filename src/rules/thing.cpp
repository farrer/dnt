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
#include <kobold/defparser.h>
#include <kobold/log.h>

using namespace DNT;

#define THING_KEY_NAME                "name"
#define THING_KEY_DESCRIPTION         "description"
#define THING_KEY_MODEL               "model"
#define THING_KEY_STATE               "state"
#define THING_KEY_WALKABLE            "walkable"
#define THING_KEY_CONVERSATION        "conversation"

#define THING_VALUE_TRUE              "true"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Thing::Thing()
{
   maxLifePoints = 0;
   lifePoints = 0;
   armatureClass = 0;
   initiativeBonus = 0;
   displacement =  WALK_PER_MOVE_ACTION;;
   model3d = NULL;
   state = 0;
   walkable = false;
   currentEnemy = NULL;
   psychoState = PSYCHO_NEUTRAL;
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Thing::~Thing()
{
   /* Delete model, if defined */
   if(model3d)
   {
      delete model3d;
   }

   /* Decrement 'avoid-names-clash-map' counter */
   std::map<Ogre::String,int>::iterator it = namesMap.find(untranslatedName);
   if(it != namesMap.end())
   {
      namesMap[untranslatedName] = it->second - 1;
   }
   //TODO: else show error!
}

/**************************************************************************
 *                                  load                                  *
 **************************************************************************/
bool Thing::load(Ogre::SceneManager* sceneManager, Ogre::String fileName, 
      bool fullPath)
{
   Kobold::DefParser defParser;
   Ogre::String key, value, modelName;

   if(!defParser.load(fileName, fullPath))
   {
      return false;
   }

   while(defParser.getNextTuple(key, value))
   {
      if(key == THING_KEY_NAME)
      {
         untranslatedName = value;
         name = translateDataString(value);
         
         /* Define unique name */
         std::map<Ogre::String,int>::iterator it = namesMap.find(value);
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
         description = value;
      }
      else if(key == THING_KEY_MODEL)
      {
         /* Load model. Just once, avoiding overload and memmory leaks on
          * wrong defined files. */
         if(model3d == NULL)
         {
            modelFileName = value;
            model3d = new Goblin::Model3d(modelName, modelFileName, 
                  sceneManager);
         }
      }
      else if(key == THING_KEY_STATE)
      {
         sscanf(value.c_str(), "%d", &state);
      }
      else if(key == THING_KEY_WALKABLE)
      {
         walkable = (value == THING_VALUE_TRUE);
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

   return true;
}

/**************************************************************************
 *                             getModel3d                                 *
 **************************************************************************/
Goblin::Model3d* Thing::getModel()
{
   return model3d;
}

/**************************************************************************
 *                               getName                                  *
 **************************************************************************/
Ogre::String Thing::getName()
{
   return name;
}

/**************************************************************************
 *                               getDescription                           *
 **************************************************************************/
Ogre::String Thing::getDescription()
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
 *                               getLifePoints                            *
 **************************************************************************/
int Thing::getLifePoints()
{
   return lifePoints;
}

/**************************************************************************
 *                               addLifePoints                            *
 **************************************************************************/
void Thing::addLifePoints(int value)
{
   lifePoints += value;
   if(lifePoints > maxLifePoints)
   {
      lifePoints = maxLifePoints;
   }
   //TODO: updateHealthBar!:w
   //
}

/**************************************************************************
 *                             getMaxLifePoints                           *
 **************************************************************************/
int Thing::getMaxLifePoints()
{
   return maxLifePoints;
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
         return -1;
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
         return -1;
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

   return -1;
}

/**************************************************************************
 *                            getCurBonusAndSaves                         *
 **************************************************************************/
BonusAndSaves* Thing::getCurBonusAndSaves()
{
   return &curBonusAndSaves;
}

/**************************************************************************
 *                            getConversationFile                         *
 **************************************************************************/
Ogre::String Thing::getConversationFile()
{
   return conversationFile;
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
void Thing::setConversationFile(Ogre::String fileName)
{
   conversationFile = fileName;
}

/**************************************************************************
 *                             static members                             *
 **************************************************************************/
std::map<Ogre::String, int> Thing::namesMap;

