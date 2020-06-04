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

#include "scriptcontroller.h"
#include "scriptinstance.h"
#include "scriptmanager.h"
#include <OGRE/OgreDataStream.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <kobold/log.h>
#include <assert.h>
using namespace DNT;

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
ScriptController::ScriptController(ScriptType type, ScriptManager* manager)
{ 
   this->type = type;
   this->manager = manager;
   this->asEngine = manager->getScriptEngine();
   this->module = NULL;
   this->instCount = 0;
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
ScriptController::~ScriptController()
{
   if(module)
   {
      int r = this->asEngine->DiscardModule(module->GetName());
      assert(r >= 0);
      module = NULL;
   }
   assert(instCount == 0);
}

/**************************************************************************
 *                           loadAndCompile                               *
 **************************************************************************/
bool ScriptController::loadAndCompile(Kobold::String filename)
{
   this->filename = filename;
   
   /* Open the desired file */
   Ogre::DataStreamPtr fileData;
   try
   {
      fileData = Ogre::ResourceGroupManager::getSingleton().openResource(
            filename);
   }
   catch(Ogre::FileNotFoundException&)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: Couldn't open inner file: '%s'", filename.c_str());
      return false;
   }
   
   /* Read the whole file */
   Kobold::String data = fileData->getAsString();
   fileData->close();
  
   /* Define the script */
   module = asEngine->GetModule(filename.c_str(), asGM_ALWAYS_CREATE);
   module->AddScriptSection(filename.c_str(), data.c_str());

   /* Build it */
   int r = module->Build();
   assert(r >= 0);
   if(r < 0)
   {
      /* Build error */
      return false;
   }

   /* define our main class type (which implements our needed 
    * 'entry-point' functions */
   mainType = getTypeByInterface(getMainTypeInterface());

   /* Define our 'entry' functions. */
   setFunctionPointers();

   return true;
}

/**************************************************************************
 *                          getTypeByInterface                            *
 **************************************************************************/
asITypeInfo* ScriptController::getTypeByInterface(Kobold::String interfaceName)
{
   asITypeInfo* type = NULL;

   /* Search on all types... */
   int tc = module->GetObjectTypeCount();
   for(int n=0; n < tc; n++)
   {
      type = module->GetObjectTypeByIndex(n);

      /* ... for an specific implemented interface */
      int ic = type->GetInterfaceCount();
      for(int i = 0; i < ic; i++)
      {
         if(Kobold::String(type->GetInterface(i)->GetName()) == interfaceName)
         {
            /* Found */
            return type;
         }
      }
   }

   return NULL;
}

/**************************************************************************
 *                            incInstanceCount                            *
 **************************************************************************/
void ScriptController::incInstanceCount()
{
   this->instCount++;
}

/**************************************************************************
 *                            decInstanceCount                            *
 **************************************************************************/
void ScriptController::decInstanceCount()
{
   this->instCount--;
}

