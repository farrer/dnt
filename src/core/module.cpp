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

#include "module.h"
#include "game.h"
#include "../script/scriptmanager.h"
#include "../rules/ruledef.h"
#include "../gui/skin.h"
#include <farso/controller.h>
#include <kobold/log.h>
using namespace DNT;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Module::Module(const Kobold::String& filename)
{
   Kobold::Log::add("Initing Module...");

   Kobold::String basename;
   Ogre::StringUtil::splitFilename(filename, basename, path);

   //TODO: Add support for modules outside 'modules' directory!.
   /* Remove '../datamodules/' if defined */
   Kobold::String file = Ogre::StringUtil::replaceAll(filename, 
         "../data/modules/", "");

   /* Load our module script */
   script = Game::getScriptManager()->createModuleScriptInstance(file);

   /* Parse our directories list */
   dirs = Ogre::StringUtil::split(script->getBaseFolders(), "\t\n;");
   curLoad = 0;
   totalInitCycles = dirs.size() + 1;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Module::~Module()
{
   Kobold::Log::add("Finishing Module...");
   if(script)
   {
      Game::getScriptManager()->removeInstance(script);
   }
   if(curLoad > 0)
   {
      DNT::Rules::finish();
   }
}

/***********************************************************************
 *                              doCycleInit                            *
 ***********************************************************************/
bool Module::doCycleInit(bool changeSkin, const Kobold::String& dataPath,
               Farso::ProgressBar* progressBar)
{
   if(curLoad < totalInitCycles - 1)
   {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            dataPath + dirs[curLoad], "FileSystem", dirs[curLoad], true);
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
            dirs[curLoad], true);
      progressBar->setValue(((float)curLoad / totalInitCycles) * 100);
   }
   else
   {
      if(changeSkin)
      {
         /* Initialize our skin definition
          * Note: the skin dealloc will be made by Farso.*/
         new Skin(script->getSkinFilename());
      }

      /* Load our rules */
      DNT::Rules::init(script->getRulesFilename());

      return true;
   }

   curLoad++;
   return false;
}

/***********************************************************************
 *                            loadInitialMap                           *
 ***********************************************************************/
bool Module::loadInitialMap()
{
   if(Game::loadMap(script->getInitialMap()))
   {
      script->callOnInitGame();
      return true;
   }

   return false;
}

