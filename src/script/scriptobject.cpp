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

#include "scriptobject.h"
#include <angelscript.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObject::ScriptObject(ScriptObjectType type, Kobold::String filename, 
      Ogre::Vector3 originalPos)
{
   this->references = 0;
   this->type = type;
   this->filename = filename;
   this->originalPos = originalPos;
   this->usePos = true;
   this->fullOnScript = false;
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObject::ScriptObject(ScriptObjectType type, Kobold::String filename)
{
   this->references = 0;
   this->type = type;
   this->filename = filename;
   this->usePos = false;
   this->fullOnScript = false;
}

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptObject::ScriptObject(ScriptObjectType type)
{
   this->references = 0;
   this->type = type;
   this->filename = "";
   this->usePos = false;

   this->fullOnScript = true;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptObject::~ScriptObject()
{
   assert(references == 0);
}

/**************************************************************************
 *                              addReference                              *
 **************************************************************************/
int ScriptObject::addReference()
{
   return asAtomicInc(references);
}

/**************************************************************************
 *                                release                                 *
 **************************************************************************/
int ScriptObject::release()
{
   asAtomicDec(references);
   if((references == 0) && (fullOnScript))
   {
      delete this;
   }

   return references;
}

/**************************************************************************
 *                            isEquivalent                                *
 **************************************************************************/
const bool ScriptObject::isEquivalent(const Kobold::String& filename)
{
   return (this->filename == filename);
}

/**************************************************************************
 *                            isEquivalent                                *
 **************************************************************************/
const bool ScriptObject::isEquivalent(const Kobold::String& filename, 
      const Ogre::Vector3& pos)
{
   return (this->filename == filename) &&
          ((!this->usePos) || (this->originalPos == pos));
}

