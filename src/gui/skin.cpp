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

#include "skin.h"
#include "../script/scriptmanager.h"
#include "../core/game.h"

#include <farso/controller.h>

#include <assert.h>

using namespace DNT;

/****************************************************************************
 *                                 Constructor                              *
 ****************************************************************************/
Skin::Skin(const Kobold::String& scriptFilename)
     :Farso::Skin()
{
   script = Game::getScriptManager()->createSkinScriptInstance(scriptFilename);
   assert(script);
   load(script->getSkinFilename());
   Farso::Controller::setSkin(this);
}

/****************************************************************************
 *                                 Destructor                               *
 ****************************************************************************/
Skin::~Skin()
{
}

/****************************************************************************
 *                             getScrollTextId                              *
 ****************************************************************************/
Kobold::String Skin::getLogScrollTextId()
{
   assert(script);
   return script->getLogScrollTextId();
}

/****************************************************************************
 *                             getTotalElements                             *
 ****************************************************************************/
int Skin::getTotalElements()
{
   assert(script);
   /* The total elements are the basic ones plus the defined by the script */
   return script->getTotalElements() + 
      Farso::Skin::TOTAL_BASIC_SKIN_ELEMENT_TYPES;
}

/****************************************************************************
 *                          getExtendedElementType                          *
 ****************************************************************************/
int Skin::getExtendedElementType(Kobold::String typeName)
{
   assert(script);
   int res = script->getElementType(typeName);
   if(res != Farso::Skin::SKIN_TYPE_UNKNOWN)
   {
      /* The index should be after the last default element */
      res += Farso::Skin::TOTAL_BASIC_SKIN_ELEMENT_TYPES;
   }

   return res;
}

