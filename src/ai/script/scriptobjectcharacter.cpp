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
#include "scriptmanager.h"
#include "pendingaction.h"
#include "../../rules/character.h"
#include "../../core/game.h"
#include <kobold/log.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectCharacter::ScriptObjectCharacter(Kobold::String filename, 
      Ogre::Vector3 originalPos)
                      :ScriptObject(ScriptObject::TYPE_CHARACTER, filename, 
                            originalPos)
{
   mutex.lock();
   character = Game::getCharacter(filename, originalPos);

   if(character)
   {
      character->defineScriptObject(this);
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
            "Warning: script: Couldn't find character '%s' (%.3f %.3f %.3f)\n",
            filename.c_str(), originalPos.x, originalPos.y, originalPos.z);
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
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
         "Warning: script: Couldn't find character '%s'\n", filename.c_str());
   }
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
      pos = character->getModel()->getPosition();
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
      character->getModel()->setPosition(pos);
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
      character->getModel()->setTargetOrientation(0.0f, ori, 0.0f, 4);
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
      res = character->getModel()->getOrientation();
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
void ScriptObjectCharacter::moveToCharacter(ScriptObjectCharacter* character)
{
   //TODO
}

/**************************************************************************
 *                            moveFromCharacter                           *
 **************************************************************************/
void ScriptObjectCharacter::moveFromCharacter(ScriptObjectCharacter* character)
{
   //TODO
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
}

/**************************************************************************
 *                            doScriptRegister                            *
 **************************************************************************/
void ScriptObjectCharacter::doScriptRegister(asIScriptEngine* asEngine)
{
   int r;

   /* Character object and its functions */
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
   r = asEngine->RegisterObjectMethod("Character", 
         "void moveToPosition(float posX, float posZ)", 
         asMETHOD(ScriptObjectCharacter, moveToPosition), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Character", 
         "void moveToCharacter(Character@+ character)",
         asMETHOD(ScriptObjectCharacter, moveToCharacter), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Character", 
         "void moveFromCharacter(Character@+ character)",
         asMETHOD(ScriptObjectCharacter, moveFromCharacter), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Character", 
         "void callAnimation(int index)",
         asMETHOD(ScriptObjectCharacter, callAnimation), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Character", 
         "void setAnimation(int index)",
         asMETHOD(ScriptObjectCharacter, setAnimation), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Character", "void openDialog()",
         asMETHOD(ScriptObjectCharacter, openDialog), asCALL_THISCALL);
   assert(r >=0 );
}

