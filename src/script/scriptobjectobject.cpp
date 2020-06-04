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

#include "scriptobjectobject.h"
#include "scriptmanager.h"
#include "pendingaction.h"
#include "../core/object.h"
#include "../core/game.h"
#include "../map/map.h"
#include <kobold/log.h>
#include <stdlib.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectObject::ScriptObjectObject(Kobold::String filename, 
      Ogre::Vector3 originalPos, Object* obj)
                      :ScriptObject(ScriptObject::TYPE_OBJECT, filename, 
                            originalPos)
{
   mutex.lock();

   if(obj)
   {
      object = obj;
   }
   else
   {
      assert(Game::getCurrentMap());
      object = Game::getCurrentMap()->getObject(filename, originalPos);

      if(object)
      {
         object->defineScriptObject(this);
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
               "Warning: script: Couldn't find object '%s' (%.3f %.3f %.3f)\n",
               filename.c_str(), originalPos.x, originalPos.y, originalPos.z);
      }
   }
   mutex.unlock();
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObjectObject::ScriptObjectObject(Kobold::String filename) 
                      :ScriptObject(ScriptObject::TYPE_OBJECT, filename) 
{
   mutex.lock();
   assert(Game::getCurrentMap());
   object = Game::getCurrentMap()->getObject(filename);

   if(object)
   {
      object->defineScriptObject(this);
   }
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL, 
         "Warning: script: Couldn't find object '%s'\n", filename.c_str());
   }
   mutex.unlock();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObjectObject::~ScriptObjectObject()
{
   mutex.lock();
   if(object)
   {
      object->undefineScriptObject();
   }
   mutex.unlock();
}

/**************************************************************************
 *                              setPointer                                *
 **************************************************************************/
void ScriptObjectObject::setPointer(void* newPtr)
{
   mutex.lock();
   object = static_cast<Object*>(newPtr);
   mutex.unlock();
}
 
/**************************************************************************
 *                               isValid                                  *
 **************************************************************************/ 
const bool ScriptObjectObject::isValid()
{
   bool res = false;
   
   mutex.lock();
   res = object != NULL;
   mutex.unlock();
   
   return res;
}

/**************************************************************************
 *                              getPosition                               *
 **************************************************************************/
const Ogre::Vector3 ScriptObjectObject::getPosition()
{
   Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
   mutex.lock();
   if(object)
   {
      pos = object->getPosition();
   }
   mutex.unlock();

   return pos;
}


/**************************************************************************
 *                              setPosition                               *
 **************************************************************************/
void ScriptObjectObject::setPosition(float x, float y, float z)
{
   mutex.lock();
   if(object)
   {
      object->setPosition(Ogre::Vector3(x, y, z));
   }
   mutex.unlock();
}

/**************************************************************************
 *                            setOrientation                              *
 **************************************************************************/
void ScriptObjectObject::setOrientation(float ori)
{
   mutex.lock();
   if(object)
   {
      object->setOrientation(ori);
   }
   mutex.unlock();
}
 
/**************************************************************************
 *                           registerFunctions                            *
 **************************************************************************/
void ScriptObjectObject::registerFunctions(asIScriptEngine* asEngine)
{
   int r = asEngine->RegisterObjectMethod("Object", 
         "void setPosition(float x, float y, float z)", 
         asMETHOD(ScriptObjectObject, setPosition), asCALL_THISCALL);
   assert(r >=0 );
   r = asEngine->RegisterObjectMethod("Object", 
         "void setOrientation(float ori)",
         asMETHOD(ScriptObjectObject, setOrientation), asCALL_THISCALL);
   assert(r >=0 );
}

/**************************************************************************
 *                             registerClass                              *
 **************************************************************************/
void ScriptObjectObject::registerClass(asIScriptEngine* asEngine)
{
   int r;

   r = asEngine->RegisterObjectType("Object", 0, asOBJ_REF);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Object", asBEHAVE_ADDREF, 
         "void f()", asMETHOD(ScriptObjectObject, addReference), 
         asCALL_THISCALL);
   assert(r >= 0);
   r = asEngine->RegisterObjectBehaviour("Object", asBEHAVE_RELEASE, 
         "void f()", asMETHOD(ScriptObjectObject, release), 
         asCALL_THISCALL);
   assert(r >= 0);
}

