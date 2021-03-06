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

#include "../core/inventory.h"
#include "../core/game.h"

#include "../ai/astar.h"

#include "../collision/collision.h"

#include <kobold/log.h>
#include <goblin/camera.h>

#include <assert.h>

using namespace DNT;

/***********************************************************************
 *                                Character                            *
 ***********************************************************************/
Character::Character(bool playable)
          :Thing(Thing::THING_TYPE_CHARACTER, CHARACTER_TOTAL_ANIMATIONS)
{
   /* Default: alive */
   this->dead = false;

   this->canMove = true;
   this->canAttack = true;
   this->aStar = NULL;

   /* Set ModEffects owner */
   this->effects.setOwner(this);

   /* Create its inventory */
   inventory = new Inventory();

   if(playable)
   {
      if(Game::getPcs())
      {
         Game::getPcs()->insertCharacter(this);
      }
   }
   else
   {
      if(Game::getNpcs())
      {
         Game::getNpcs()->insertCharacter(this);
      }
   }
}

/***********************************************************************
 *                               ~Character                            *
 ***********************************************************************/
Character::~Character()
{
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
 *                             addModEffect                            *
 ***********************************************************************/
void Character::addModEffect(int mod, int time, int periodicTime,
      RuleDefinition* def)
{
   /* Create ModEffect */
   ModEffect* modEffect = new ModEffect(mod, time, periodicTime, def); 

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
   setAnimatedModelAnimation(animation, loop);
}

/***********************************************************************
 *                              getAnimation                           *
 ***********************************************************************/
Character::CharacterAnimation Character::getCurrentAnimation()
{
   int curAnim = getAnimatedModelAnimation();
   if((curAnim >= 0) && (curAnim < CHARACTER_TOTAL_ANIMATIONS))
   {
      return static_cast<CharacterAnimation>(curAnim);
   }

   return CHARACTER_ANIMATION_NONE;
}

/***********************************************************************
 *                              setCanAttack                           *
 ***********************************************************************/
void Character::setCanAttack(bool value)
{
   canAttack = value;
}

/***********************************************************************
 *                               setCanMove                            *
 ***********************************************************************/
void Character::setCanMove(bool value)
{
   canMove = value;
}

#define CHARACTER_KEY_WALK_INTERVAL "walk_interval"
#define CHARACTER_KEY_BLOOD_POSITION "bloodPosition"
#define CHARACTER_KEY_BLOOD_PARTICLE "bloodParticle"

/***********************************************************************
 *                           doSpecificParse                           *
 ***********************************************************************/
bool Character::doSpecificParse(const Kobold::String& key, 
      const Kobold::String& value)
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

   return doCharacterSpecializationParse(key, value);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool Character::load(const Kobold::String& filename)
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
   return doCharacterSpecializationSave(file);
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
 *                          setInitialPosition                       *
 *********************************************************************/
void Character::setInitialPosition(Ogre::Vector3 pos)
{
   Thing::setInitialPosition(pos);
   flushAfterMove();
}

/*********************************************************************
 *                       setMoveByFoundPath                          *
 *********************************************************************/
void Character::setToMoveByFoundPath(AStar* aStar)
{
   this->aStar = aStar;
}

/*********************************************************************
 *                              update                               *
 *********************************************************************/
bool Character::update()
{
   bool res = false;

   if(aStar)
   {
      /* Must follow its path */
      Ogre::Vector3 pos = getPosition();
      float ori = getOrientation();
      if(aStar->getNewPosition(pos, ori, false, 1.0f))
      {
         /* Check if we can move to the position (some character could
          *  moved on our define path. Note that on 'pure-angle' change
          *  (where we only change the character orientation and kept
          *   the position, there's no need for a collision check, avoiding
          *   some awfull 'stuck' situations. */
         float newHeight = getPosition().y;
         if((pos == getPosition()) ||
            (Collision::canOccupy(this, pos, newHeight)))
         {
            setPosition(pos);
            setTargetOrientation(Ogre::Vector3(getPitch(), ori, getRoll()), 4);
            if(getCurrentAnimation() != CHARACTER_ANIMATION_WALK)
            {
               setAnimation(CHARACTER_ANIMATION_WALK, true);
            }
            flushAfterMove();
         }
         else
         {
            /* Couldn't move, must try to search a new path */
            if(getCurrentAnimation() == CHARACTER_ANIMATION_WALK)
            {
               setAnimation(CHARACTER_ANIMATION_IDLE, true);
            }
            float destX=0.0f, destZ=0.0f;
            aStar->getDestiny(destX, destZ);
            aStar->clear();
            aStar->findPath(this, destX, destZ, getWalkInterval(), true);
            /* Remove our reference to stop update trying to move with it */
            aStar = NULL;
         }
      }
      else
      {
         /* Done with movement, bye aStar reference */
         aStar = NULL;
         if(getCurrentAnimation() != CHARACTER_ANIMATION_IDLE)
         {
            setAnimation(CHARACTER_ANIMATION_IDLE, true);
         }
      }
   }

   res |= Thing::update();
   effects.doStep();

   return res;
}

/*********************************************************************
 *                          flushAfterMove                           *
 *********************************************************************/
void Character::flushAfterMove()
{
   Collision::removeElement(this);
   Collision::addElement(this);
   /* Move the camera, if we are the active playable character */
   if(Game::getPcs()->getActiveCharacter() == this)
   {
      Game::updateCameraPosition(this);
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
Character* CharacterList::getCharacter(const Kobold::String& filename)
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
   //FIXME: really necessary? We have a direct way to get it from map...
   Character* ch = static_cast<Character*>(getFirst());

   /* Search the list for it */
   for(int i = 0; i < getTotal(); i++)
   {
      if(ch->getSceneNode() == scNode)
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
      ch->update();
      ch = static_cast<Character*>(ch->getNext());
   }
}

