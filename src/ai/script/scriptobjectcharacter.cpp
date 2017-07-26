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
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectCharacter::ScriptObjectCharacter(Kobold::String filename) 
                      :ScriptObject(ScriptObject::TYPE_CHARACTER, filename) 
{
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
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectCharacter::~ScriptObjectCharacter()
{
   if(character)
   {
      character->undefineScriptObject();
   }
}

/**************************************************************************
 *                              setPointer                                *
 **************************************************************************/
void ScriptObjectCharacter::setPointer(void* newPtr)
{
   character = static_cast<Character*>(newPtr);
}

/**************************************************************************
 *                            moveToPosition                              *
 **************************************************************************/
void ScriptObjectCharacter::moveToPosition(float posX, float posZ)
{
   //TODO
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
}

/**************************************************************************
 *                              setAnimation                              *
 **************************************************************************/
void ScriptObjectCharacter::setAnimation(int index)
{
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

