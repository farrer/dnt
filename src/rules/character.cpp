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

#include "character.h"

#include "alignment.h"
#include "classes.h"
#include "race.h"
#include "feats.h"
#include "modeffect.h"

#include "../core/inventory.h"

#include <kobold/log.h>

#include <assert.h>

using namespace DNT;

/***********************************************************************
 *                                Character                            *
 ***********************************************************************/
Character::Character()
          :Thing(Thing::THING_TYPE_CHARACTER, CHARACTER_TOTAL_ANIMATIONS)
{
   /* Default: alive */
   this->dead = false;

   /* Set ModEffects owner */
   this->effects.setOwner(this);

   /* Nullify things */
   this->curAlign = NULL;
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      this->classes[i] = NULL;
      this->classLevel[i] = 0;
   }
   this->race = NULL;

   /* Create Feats list and define needed Feats */
   feats = new Feats();
   insertDefaultNeededFeats();

   /* Define default bare hands damage dice */
   bareHandsDice.setBaseDice(Dice(Dice::DICE_TYPE_D2));
   bareHandsDice.setInitialLevel(1);

   /* Create its inventory */
   inventory = new Inventory();
}

/***********************************************************************
 *                               ~Character                            *
 ***********************************************************************/
Character::~Character()
{
   if(feats)
   {
      delete feats;
   }
   if(inventory)
   {
      delete inventory;
   }
}

/***********************************************************************
 *                             animationList                           *
 ***********************************************************************/
Kobold::String Character::animationList[CHARACTER_TOTAL_ANIMATIONS] =
{
   "attack", "dead", "die", "idle", "run", "walk"
};

/***********************************************************************
 *                            getAnimationList                         *
 ***********************************************************************/
Kobold::String* Character::getAnimationList()
{
   return animationList;   
}

/***********************************************************************
 *                        insertDefaultNeededFeats                     *
 ***********************************************************************/
void Character::insertDefaultNeededFeats()
{
   feats->insert(FeatsList::getFeatByNumber(FEAT_WEAPON_ATTACK));
}

/***********************************************************************
 *                             addModEffect                            *
 ***********************************************************************/
void Character::addModEffect(int mod, int time, int periodicTime,
      Kobold::String factorId, Kobold::String factorType)
{
   /* Create ModEffect */
   ModEffect* modEffect = new ModEffect(mod, time, periodicTime, 
         factorId, factorType);

   /* Add and apply it */
   effects.insert(modEffect, true);
}

/***********************************************************************
 *                           removeAllModEffect                        *
 ***********************************************************************/
void Character::removeAllModEffects()
{
   effects.clear();
}

/***********************************************************************
 *                              getEffects                             *
 ***********************************************************************/
ModEffectList* Character::getEffects()
{
   return &effects;
}

/***********************************************************************
 *                             updateEffects                           *
 ***********************************************************************/
void Character::updateEffects()
{
   effects.doStep();
}

/***********************************************************************
 *                               isAlignOf                             *
 ***********************************************************************/
bool Character::isAlignOf(Kobold::String al)
{
   if(curAlign)
   {
      return curAlign->getStringId().find(al) != Kobold::String::npos;
   }

   return false;
}

/***********************************************************************
 *                           getEmptyClassIndex                        *
 ***********************************************************************/
int Character::getEmptyClassIndex()
{
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] == NULL)
      {
         return i;
      }
   }
   return -1;
}

/***********************************************************************
 *                              getInventory                           *
 ***********************************************************************/
Inventory* Character::getInventory()
{
   return inventory;
}

/***********************************************************************
 *                              setAnimation                           *
 ***********************************************************************/
void Character::setAnimation(CharacterAnimation animation, bool loop)
{
   Goblin::AnimatedModel3d* model = static_cast<Goblin::AnimatedModel3d*>(
         getModel());
   model->setBaseAnimation(animation, loop, true);
}

/***********************************************************************
 *                              getAnimation                           *
 ***********************************************************************/
Character::CharacterAnimation Character::getCurrentAnimation()
{
   Goblin::AnimatedModel3d* model = static_cast<Goblin::AnimatedModel3d*>(
         getModel());

   int curAnim = model->getCurrentAnimation();
   if((curAnim >= 0) && (curAnim < CHARACTER_TOTAL_ANIMATIONS))
   {
      return static_cast<CharacterAnimation>(curAnim);
   }

   return CHARACTER_ANIMATION_NONE;
}

/***********************************************************************
 *                           getInitialPosition                        *
 ***********************************************************************/
Ogre::Vector3 Character::getInitialPosition()
{
   return initialPos;
}

/***********************************************************************
 *                           setInitialPosition                        *
 ***********************************************************************/
void Character::setInitialPosition(Ogre::Vector3 pos)
{
   initialPos = pos;
}

#define CHARACTER_KEY_WALK_INTERVAL "walk_interval"
#define CHARACTER_KEY_BLOOD_POSITION "bloodPosition"
#define CHARACTER_KEY_BLOOD_PARTICLE "bloodParticle"
#define CHARACTER_KEY_RACE "race"
#define CHARACTER_KEY_CLASS "class"
#define CHARACTER_KEY_ALIGNMENT "align"

/***********************************************************************
 *                           doSpecificParse                           *
 ***********************************************************************/
bool Character::doSpecificParse(Kobold::String key, Kobold::String value)
{
   /* Walk Interval */
   if(key == CHARACTER_KEY_WALK_INTERVAL)
   {
      //TODO
      return true;
   }
   /* Blood position */
   else if(key == CHARACTER_KEY_BLOOD_POSITION)
   {
      //TODO: particles
      return true;
   }
   /* Blood particle */
   else if(key == CHARACTER_KEY_BLOOD_PARTICLE)
   {
      //TODO: particles
      return true;
   }
   /* Race */
   else if(key == CHARACTER_KEY_RACE)
   {
      race = Races::getRaceByString(value);
      if(race == NULL)
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: Unknown race '%s' for character!", value.c_str());
      }
      return true;
   }
   /* Alignment */
   else if(key == CHARACTER_KEY_ALIGNMENT)
   {
      curAlign = Alignments::getAlignmentByString(value);
      if(curAlign == NULL)
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: Unknown alignment '%s' for character!", value.c_str());
      }
      return true;
   }
   /* Class */
   else if(key == CHARACTER_KEY_CLASS)
   {
      int cindex = getEmptyClassIndex();
      if(cindex != -1)
      {
         /* Parse class name and level */
         char buf[128];
         int lvl = 0;
         sscanf(value.c_str(), "%s %d", &buf[0], &lvl);
         /* Set it */
         classes[cindex] = Classes::getClassByString(buf);
         classLevel[cindex] = lvl;
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: class overflow for character with value '%s'",
               value.c_str());
      }
   }

   return doCharacterSpecializationParse(key, value);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool Character::load(Kobold::String filename)
{
   if(Thing::load(filename, Goblin::Model3d::MODEL_DYNAMIC))
   {
      doAfterLoad();
      return true;
   }

   return false;
}

/***********************************************************************
 *                            doAfterLoad                              *
 ***********************************************************************/
void Character::doAfterLoad()
{
   /* Define the AC if not yet defined at the file */
   if(getArmatureClass() == 0)
   {
      /* Define AC TODO others values to sum here*/ 
      setArmatureClass(10 + getSkills()->getSkillByString(
               "DEXTERITY")->getAttributeBonus());
   }

   /* Apply Saves and Bonus, if not yet defined (some npcs without classes
    * defines its bonus on .npc file) */
   if(!getCurBonusAndSaves()->isDefined())
   {
      applyBonusAndSaves();
   }

   /* Apply cost to skills */
   applySkillCosts();
   
   /* Set as idle by default */
   setAnimation(CHARACTER_ANIMATION_IDLE, true);
}

/***********************************************************************
 *                           doSpecifcSave                             *
 ***********************************************************************/
bool Character::doSpecificSave(std::ofstream& file)
{
   //TODO: file << CHARACTER_KEY_PORTRAIT << " = " << std::endl;
   //TODO: file << CHARACTER_KEY_WALK_INTERVAL << " = " << std::endl;
   //TODO: file << CHARACTER_KEY_BLOOD_POSITION << " = " << std::endl;
   //TODO: file << CHARACTER_KEY_BLOOD_PARTICLE << " = " << std::endl;
   file << CHARACTER_KEY_RACE << " = " << race->getStringId() << std::endl;
   file << CHARACTER_KEY_ALIGNMENT << " = " << curAlign->getStringId() 
        << std::endl;
   /* Class */
   for(int i = 0; 
       ((i < CHARACTER_MAX_DISTINCT_CLASSES) || (classes[i] == NULL)); i++)
   {
      file << CHARACTER_KEY_CLASS << " = " << classes[i]->getStringId() 
           << " " << classLevel[i] << std::endl;
   }
   return doCharacterSpecializationSave(file);
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel()
{
   int totalLevels = 0;
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         totalLevels += classLevel[i];
      }
   }
   return totalLevels;
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel(Class* cl)
{
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] == cl)
      {
         return classLevel[i];
      }
   }
   return 0;
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int Character::getLevel(Kobold::String classId)
{
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if((classes[i] != NULL) && 
         (classes[i]->getStringId() == classId))
      {
         return classLevel[i];
      }
   }
   return 0;
}

/*********************************************************************
 *                              instantKill                          *
 *********************************************************************/
void Character::instantKill()
{
   dead = true;
   //TODO: skeleton state: set as dead!
   //scNode->getModel()->setState(STATE_DEAD);
}

/*********************************************************************
 *                           applySkillCosts                         *
 *********************************************************************/
void Character::applySkillCosts()
{
   Skills* skills = getSkills();

   /* Clear current costs */
   skills->clearCosts();

   /* Apply Race Costs */
   if(race) 
   {
      race->applySkillCosts(skills);
   }

   //FIXME Classes Costs will be only per actual class?
   //      In the way bellow is always for all classes.
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         classes[i]->applySkillCosts(skills);
      }
   }
}

/*********************************************************************
 *                         applyBonusAndSaves                        *
 *********************************************************************/
void Character::applyBonusAndSaves()
{
   BonusAndSaves* bas = getCurBonusAndSaves();

   /* Clear the current */
   bas->clear();

   /* Add from all classes */
   for(int i = 0; i < CHARACTER_MAX_DISTINCT_CLASSES; i++)
   {
      if(classes[i] != NULL)
      {
         (*bas) = (*bas) + classes[i]->getBonusAndSaves(classLevel[i] - 1);
      }
   }
}

/***************************************************************************
 ***************************************************************************
 *                            CharacterList                                *
 ***************************************************************************
 ***************************************************************************/


/*********************************************************************
 *                         listConstructor                           *
 *********************************************************************/
CharacterList::CharacterList()
{
   curTreat = NULL;
}

/*********************************************************************
 *                          listDestructor                           *
 *********************************************************************/
CharacterList::~CharacterList()
{
   clear();
}

/*********************************************************************
 *                           insertCharacter                         *
 *********************************************************************/
void CharacterList::insertCharacter(Character* dude)
{
   /* Now insert it on the list */
   insert(dude);
   
   /* And set it as the active Character */
   activeCharacter = dude;
} 

/*********************************************************************
 *                          removeCharacter                          *
 *********************************************************************/
void CharacterList::removeCharacter(Character* dude)
{
   /* Empty potential references to it */
   if(dude == curTreat)
   {
      curTreat = NULL;
   }
   if(dude == activeCharacter)
   {
      activeCharacter = NULL;
   }

   /* Remove the Character from the list */
   remove(dude);
}

/*********************************************************************
 *                            getEnemyCharacter                      *
 *********************************************************************/
Character* CharacterList::getNextEnemyCharacter(Character* last)
{
   //TODO: must see use to reimplement this. seems strange.
#if 0
   while(last != first)
   {
      if(last->getPsychoState() == PSYCHO_HOSTILE)
      {
         return last;
      }
      last = static_cast<Character*>(last->getNext());
   }
#endif
   return NULL;
}

/*********************************************************************
 *                           getActiveCharacter                      *
 *********************************************************************/
Character* CharacterList::getActiveCharacter()
{
   return activeCharacter;
}

/*********************************************************************
 *                              getCharacter                         *
 *********************************************************************/
Character* CharacterList::getCharacter(Kobold::String filename)
{
   Character* ch = static_cast<Character*>(getFirst());

   /* Search the list for it */
   for(int i = 0; i < getTotal(); i++)
   {
      if(ch->getFilename() == filename)
      {
         return ch;
      }
      ch = static_cast<Character*>(ch->getNext());
   }

   return NULL;
}

/*********************************************************************
 *                              getCharacter                         *
 *********************************************************************/
Character* CharacterList::getCharacter(Ogre::SceneNode* scNode)
{
   Character* ch = static_cast<Character*>(getFirst());

   /* Search the list for it */
   for(int i = 0; i < getTotal(); i++)
   {
      if((ch->getModel()) && (ch->getModel()->ownSceneNode(scNode)))
      {
         return ch;
      }
      ch = static_cast<Character*>(ch->getNext());
   }

   return NULL;
}

/*********************************************************************
 *                         getNextSameCharacter                      *
 *********************************************************************/
Character* CharacterList::getNextSameCharacter(Character* ch)
{
   Character* cur = static_cast<Character*>(ch->getNext());

   /* Search the list for it */
   while( (cur != ch) && (cur != static_cast<Character*>(getFirst())) )
   {
      if(cur->getFilename() == ch->getFilename())
      {
         return cur;
      }
      cur = static_cast<Character*>(cur->getNext());
   }

   return NULL;
}

/*********************************************************************
 *                            isCharacterIn                          *
 *********************************************************************/
bool CharacterList::isCharacterIn(Character* ch)
{
   Character* c = static_cast<Character*>(getFirst());
   int i;

   /* Search the list for it */
   for(i = 0; i < getTotal(); i++)
   {
      if(c == ch)
      {
         return true;
      }
      c = static_cast<Character*>(c->getNext());
   }

   return false;
}

/*********************************************************************
 *                           setActiveCharacter                      *
 *********************************************************************/
void CharacterList::setActiveCharacter(Character* dude)
{
   activeCharacter = dude;
}

/*********************************************************************
 *                                  update                           *
 *********************************************************************/
void CharacterList::update()
{
   int i;
   Character* ch = static_cast<Character*>(getFirst());
   for(i=0; i < getTotal(); i++)
   {
      ch->updateEffects();
      ch = static_cast<Character*>(ch->getNext());
   }
}

/*********************************************************************
 *                           treatGeneralScripts                     *
 *********************************************************************/
void CharacterList::treatGeneralScripts()
{
//TODO: scripts.
#if 0
   int i;
   iaScript* script;
   DialogWindow dlg;
   Character* ch;

   for(i=0; i < CHARACTER_TREAT_SCRIPTS; i++)
   {
      if(total <= 0)
      {
         /* No Characters -> Nothing to treat */
         return;
      }
      
      /* Set, if needed the current Character to treat */
      if(!curTreat)
      {
         curTreat = static_cast<Character*>(first);
      }
            
      /* Get and treat script */
      script = static_cast<Script*> curTreat->getGeneralScript();
      if( (script) && (curTreat->isAlive()))
      {
         /* Only treat a Character script, if no dialog with it */
         // TODO barter window too!
         ch = script->getCharacterOwner();
         if( (ch == NULL) || 
             (!dlg.isOpened((Conversation*)ch->getConversation())))
         {
            script->defineMap(actualMap, NPCs);
            script->run(MAX_SCRIPT_LINES);
         }
      }

      /* Treat killed scripts (defined when Character is killed, 
       * and removed from list when ends) */
      script = curTreat->killedScript;
      if((script) && (!curTreat->isAlive()))
      {
         script->defineMap(actualMap, NPCs);
         script->run(MAX_SCRIPT_LINES);
         if(script->finished())
         {
            /* Done with killed script, must clear it! */
            delete(curTreat->killedScript);
            curTreat->killedScript = NULL;
         }
      }

      /* forward on the list */
      curTreat = static_cast<Character*>(curTreat->getNext());
   }
#endif
}


