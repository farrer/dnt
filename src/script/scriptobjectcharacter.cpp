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

#include "scriptobjectcharacter.h"
#include "scriptobjectobject.h"
#include "scriptmanager.h"
#include "pendingaction.h"
#include "../rules/character.h"
#include "../core/game.h"
#include "../core/util.h"
#include "../core/playablecharacter.h"
#include "../gui/dialogwidget.h"
#include <kobold/log.h>
#include <stdlib.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectCharacter::ScriptObjectCharacter(Kobold::String filename, 
      Ogre::Vector3 originalPos, Character* c)
                      :ScriptObject(ScriptObject::TYPE_CHARACTER, filename, 
                            originalPos)
{
   mutex.lock();

   if(c)
   {
      character = c;
   }
   else
   {
      character = Game::getCharacter(filename, originalPos);

      if(character)
      {
         character->defineScriptObject(this);
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
            "Warning: script: Couldn't find character '%s' (%.3f %.3f %.3f)\n",
            filename.c_str(), originalPos.x, originalPos.y, originalPos.z);
      }
   }
   mutex.unlock();
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectCharacter::ScriptObjectCharacter(Kobold::String filename) 
                      :ScriptObject(ScriptObject::TYPE_CHARACTER, filename) 
{
   mutex.lock();
   character = Game::getCharacter(filename);

   if(character)
   {
      character->defineScriptObject(this);
   }
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
         "Warning: script: Couldn't find character '%s'\n", filename.c_str());
   }
   mutex.unlock();
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectCharacter::ScriptObjectCharacter(Character* c) 
                      :ScriptObject(ScriptObject::TYPE_CHARACTER, 
                                    c->getFilename(),
                                    c->getInitialPosition()) 
{
   mutex.lock();
   character = c;
   character->defineScriptObject(this);
   mutex.unlock();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectCharacter::~ScriptObjectCharacter()
{
   mutex.lock();
   if(character)
   {
      character->undefineScriptObject();
   }
   mutex.unlock();
}

/**************************************************************************
 *                              setPointer                                *
 **************************************************************************/
void ScriptObjectCharacter::setPointer(void* newPtr)
{
   mutex.lock();
   character = static_cast<Character*>(newPtr);
   mutex.unlock();
}
 
/**************************************************************************
 *                               isValid                                  *
 **************************************************************************/ 
const bool ScriptObjectCharacter::isValid()
{
   bool res = false;
   
   mutex.lock();
   res = character != NULL;
   mutex.unlock();
   
   return res;
}

/**************************************************************************
 *                              getPosition                               *
 **************************************************************************/
const Ogre::Vector3 ScriptObjectCharacter::getPosition()
{
   Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
   mutex.lock();
   if(character)
   {
      pos = character->getPosition();
   }
   mutex.unlock();

   return pos;
}

/**************************************************************************
 *                              setPosition                               *
 **************************************************************************/
void ScriptObjectCharacter::setPosition(const Ogre::Vector3& pos)
{
   mutex.lock();
   if(character)
   {
      character->setPosition(pos);
   }
   mutex.unlock();
}

/**************************************************************************
 *                            setOrientation                              *
 **************************************************************************/
void ScriptObjectCharacter::setOrientation(const float ori)
{
   mutex.lock();
   if(character)
   {
      character->setTargetOrientation(ori, 4);
   }
   mutex.unlock();
}
 
/**************************************************************************
 *                            getOrientation                              *
 **************************************************************************/
const float ScriptObjectCharacter::getOrientation()
{
   float res = 0.0f;
   mutex.lock();
   if(character)
   {
      res = character->getOrientation();
   }
   mutex.unlock();
   return res;
}

/**************************************************************************
 *                           getDisplacement                              *
 **************************************************************************/
const float ScriptObjectCharacter::getDisplacement()
{
   float res = 0;
   mutex.lock();
   if(character)
   {
      res = character->getDisplacement();
   }
   mutex.unlock();
   return res;
}

/**************************************************************************
 *                              getDistance                               *
 **************************************************************************/
const float ScriptObjectCharacter::distanceToCharacter(
      ScriptObjectCharacter& target)
{
   Ogre::Vector3 myPos = getPosition();
   Ogre::Vector3 tgtPos = target.getPosition();
   return myPos.distance(tgtPos);
}

/**************************************************************************
 *                              getDistance                               *
 **************************************************************************/
const float ScriptObjectCharacter::distanceToObject(ScriptObjectObject& target)
{
   Ogre::Vector3 myPos = getPosition();
   Ogre::Vector3 tgtPos = target.getPosition();
   return myPos.distance(tgtPos);
}

/**************************************************************************
 *                           getWalkableBounds                            *
 **************************************************************************/
Ogre::Aabb ScriptObjectCharacter::getWalkableBounds()
{
   Ogre::Aabb res;
   mutex.lock();
   if(character)
   {
      res = character->getWalkableBounds();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                           setMoveByFoundPath                           *
 **************************************************************************/
void ScriptObjectCharacter::setToMoveByFoundPath(AStar* aStar)
{
   mutex.lock();
   if(character)
   {
      character->setToMoveByFoundPath(aStar);
   }
   mutex.unlock();
}

/**************************************************************************
 *                            isMovingbyPath                              *
 **************************************************************************/
const bool ScriptObjectCharacter::isMovingByPath()
{
   bool res = false;
   mutex.lock();
   if(character)
   {
      res = character->isMovingByPath();
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                            moveToPosition                              *
 **************************************************************************/
void ScriptObjectCharacter::moveToPosition(float posX, float posZ)
{
   mutex.lock();
   if(character != NULL)
   {
      PendingActionMove* action = new PendingActionMove(this, character, 
            posX, posZ);
      Game::getScriptManager()->suspendByPendingAction(action);
   }
   mutex.unlock();
}

/**************************************************************************
 *                            moveToCharacter                             *
 **************************************************************************/
void ScriptObjectCharacter::moveToCharacter(ScriptObjectCharacter* target)
{
   Ogre::Aabb aabb = target->getWalkableBounds();
   float sizeX = aabb.mHalfSize.x * 2.0f;
   float sizeZ = aabb.mHalfSize.z * 2.0f;

   /* Select a random position near the target */
   bool signalX = ((rand()/(RAND_MAX+1.0)) > 0.5);
   float varX = (int)((10 * (rand()/(RAND_MAX+1.0)))+sizeX);

   bool signalZ = ((rand()/(RAND_MAX+1.0)) > 0.5);
   float varZ = (int)((10 * (rand()/(RAND_MAX+1.0)))+sizeZ);

   Ogre::Vector3 tPos = target->getPosition();
   float tgtX = tPos.x + ((signalX) ? 1 : -1) * varX;
   float tgtZ = tPos.z + ((signalZ) ? 1 : -1) * varZ;

   /* Try to move there */
   moveToPosition(tgtX, tgtZ);
}

/**************************************************************************
 *                            moveFromCharacter                           *
 **************************************************************************/
void ScriptObjectCharacter::moveFromCharacter(ScriptObjectCharacter* target)
{
   Ogre::Vector3 myPos = getPosition();
   Ogre::Vector3 tgtPos = target->getPosition();

   /* Define a position away from the Character */
   float angle = Util::getAngle(tgtPos.x, tgtPos.z, myPos.x, myPos.z);
   const Ogre::Radian angleRad = Ogre::Radian(Ogre::Degree(angle));
   float doubleDisp = 2.0f * getDisplacement();

   float posX = myPos.x - (Ogre::Math::Cos(angleRad) * doubleDisp);
   float posZ = myPos.z - (Ogre::Math::Sin(angleRad) * doubleDisp);

   /* Try to move there */
   moveToPosition(posX, posZ);
}

/**************************************************************************
 *                            callAnimation                               *
 **************************************************************************/
void ScriptObjectCharacter::callAnimation(int index)
{
   if((index >= Character::CHARACTER_ANIMATION_ATTACK) &&
      (index < Character::CHARACTER_TOTAL_ANIMATIONS))
   {
      mutex.lock();
      if(character)
      {
         character->setAnimation(
               static_cast<Character::CharacterAnimation>(index), false);
      }
      mutex.unlock();
   }
}

/**************************************************************************
 *                              setAnimation                              *
 **************************************************************************/
void ScriptObjectCharacter::setAnimation(int index)
{
   if((index >= Character::CHARACTER_ANIMATION_ATTACK) &&
      (index < Character::CHARACTER_TOTAL_ANIMATIONS))
   {
      mutex.lock();
      if(character)
      {
         character->setAnimation(
               static_cast<Character::CharacterAnimation>(index));
      }
      mutex.unlock();
   }
}

/**************************************************************************
 *                               openDialog                               *
 **************************************************************************/
void ScriptObjectCharacter::openDialog()
{
   mutex.lock();
   if(character)
   {
      DialogWidget::open(character, 
            static_cast<PlayableCharacter*>(
               Game::getPcs()->getActiveCharacter()));
   }
   mutex.unlock();
}

/**************************************************************************
 *                           getRuleDefinition                            *
 **************************************************************************/
ScriptObjectRuleDefinition* ScriptObjectCharacter::getRuleDefinition(
      Kobold::String groupId, Kobold::String ruleDefId)
{
   ScriptObjectRuleDefinition* res = NULL;
   mutex.lock();
   if(character)
   {
      RuleDefinitionValue* r = character->getRuleDefinition(groupId, ruleDefId);
      if(r)
      {
         res = r->getScriptObject();
      }
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                           registerFunctions                            *
 **************************************************************************/
void ScriptObjectCharacter::registerFunctions(asIScriptEngine* asEngine)
{
   int r = asEngine->RegisterObjectMethod("Character", 
         "void moveTo(float posX, float posZ)", 
         asMETHOD(ScriptObjectCharacter, moveToPosition), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "void moveTo(Character@+ character)",
         asMETHOD(ScriptObjectCharacter, moveToCharacter), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "void moveFrom(Character@+ character)",
         asMETHOD(ScriptObjectCharacter, moveFromCharacter), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "void callAnimation(int index)",
         asMETHOD(ScriptObjectCharacter, callAnimation), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "void setAnimation(int index)",
         asMETHOD(ScriptObjectCharacter, setAnimation), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", "void openDialog()",
         asMETHOD(ScriptObjectCharacter, openDialog), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "RuleDefinition@+ getRuleDefinition(string group, string id)",
         asMETHOD(ScriptObjectCharacter, getRuleDefinition), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", "float getDisplacement()",
         asMETHOD(ScriptObjectCharacter, getDisplacement), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "float distance(Character@+ target)",
         asMETHOD(ScriptObjectCharacter, distanceToCharacter), asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectMethod("Character", 
         "float distance(Object@+ target)",
         asMETHOD(ScriptObjectCharacter, distanceToObject), asCALL_THISCALL);
   assert(r >= 0);
}

/**************************************************************************
 *                             registerClass                              *
 **************************************************************************/
void ScriptObjectCharacter::registerClass(asIScriptEngine* asEngine)
{
   int r;

   r = asEngine->RegisterObjectType("Character", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Character", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectCharacter, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Character", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectCharacter, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}

