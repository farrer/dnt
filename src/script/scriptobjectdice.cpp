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

#include "scriptobjectdice.h"
#include <assert.h>
#include <kobold/log.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectDice::ScriptObjectDice(Dice::DiceType faces)
                 :ScriptObject(TYPE_DICE)
{
   dice = new Dice(faces);
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectDice::~ScriptObjectDice()
{
   if(dice)
   {
      delete dice;
   }
}

/**************************************************************************
 *                                 roll                                   *
 **************************************************************************/
int ScriptObjectDice::roll()
{
   return dice->roll();
}

/**************************************************************************
 *                               setPointer                               *
 **************************************************************************/
void ScriptObjectDice::setPointer(void* newPtr)
{
   /* Should not be called! */
   assert(false);
}

/**************************************************************************
 *                            factoryDice                                 *
 **************************************************************************/
ScriptObjectDice* ScriptObjectDice::factoryDice(int faces)
{
   Dice::DiceType type;
   switch(faces)
   {
      case Dice::DICE_TYPE_D2:
         type = Dice::DICE_TYPE_D2;
      break;
      case Dice::DICE_TYPE_D4:
         type = Dice::DICE_TYPE_D4;
      break;
      case Dice::DICE_TYPE_D6:
         type = Dice::DICE_TYPE_D6;
      break;
      case Dice::DICE_TYPE_D8:
         type = Dice::DICE_TYPE_D8;
      break;
      case Dice::DICE_TYPE_D10:
         type = Dice::DICE_TYPE_D10;
      break;
      case Dice::DICE_TYPE_D12:
         type = Dice::DICE_TYPE_D12;
      break;
      case Dice::DICE_TYPE_D20:
         type = Dice::DICE_TYPE_D20;
      break;
      case Dice::DICE_TYPE_D100:
         type = Dice::DICE_TYPE_D100;
      break;
      default:
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Script Warn: unknown dice type with %d faces!\nUsing 2 instead.", 
            faces);
         type = Dice::DICE_TYPE_D2;
      break;
   }

   ScriptObjectDice* obj = new ScriptObjectDice(type);
   obj->addReference();
   return obj;
}

/**************************************************************************
 *                             registerClass                              *
 **************************************************************************/
void ScriptObjectDice::registerClass(asIScriptEngine* asEngine)
{
   int r;

   r = asEngine->RegisterObjectType("Dice", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Dice", asBEHAVE_FACTORY,
         "Dice@ f(int)", asFUNCTION(ScriptObjectDice::factoryDice), 
         asCALL_CDECL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Dice", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectDice, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Dice", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectDice, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}


/**************************************************************************
 *                           registerFunctions                            *
 **************************************************************************/
void ScriptObjectDice::registerFunctions(asIScriptEngine* asEngine)
{
   int r = asEngine->RegisterObjectMethod("Dice", 
         "int roll()", asMETHOD(ScriptObjectDice, roll), asCALL_THISCALL);
   assert(r >=0 );

}

