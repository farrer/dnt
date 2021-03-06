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

#include <farso/controller.h>
#include <farso/font.h>

#include "mapeditor.h"
#include "selection.h"

#include "../../core/game.h"

#include "../../map/map.h"

#include "../../rules/thing.h"

#include "../../gui/briefing.h"

#include <goblin/camera.h>
#include <goblin/model3d.h>

#include <OGRE/Compositor/OgreCompositorManager2.h>

using namespace DNTMapEditor;

/***********************************************************************
 *                                MapEditor                            *
 ***********************************************************************/
MapEditor::MapEditor()
{
   shouldExit = false;
   mainGui = NULL;
   positionEditor = NULL;
   lastKey = Kobold::KOBOLD_KEY_UNKNOWN;
   treatedGui = false;
}

/***********************************************************************
 *                               ~MapEditor                            *
 ***********************************************************************/
MapEditor::~MapEditor()
{
   if(mainGui)
   {
      delete mainGui;
   }
   if(positionEditor)
   {
      delete positionEditor;
   }
}

/***********************************************************************
 *                             doCycleInit                             *
 ***********************************************************************/
bool MapEditor::doCycleInit(int callCounter, bool& shouldAbort)
{
   if(doCommonCycleInit(callCounter, shouldAbort))
   {  
      /* Done with common init, let's init common map things */

      /* init mapeditor own textures */
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            getDataPath() + "mapeditor/textures", "FileSystem", 
            "mapeditor_textures", true);
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
            "mapeditor_textures", true);
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            getDataPath() + "mapeditor/models", "FileSystem", 
            "mapeditor_models", true);
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
            "mapeditor_models", true);

      /* Init base module's path */
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            getDataPath() + "modules", "FileSystem",
            "mapeditor_modules", false);
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
            "mapeditor_modules", true);

      /* define our position editor */
      positionEditor = new PositionEditor(getSceneManager());

      /* Create Map Editor's GUI */
      mainGui = new MainGui(positionEditor);
      mainGui->hideProgressBar();
      mainGui->showTopBar();
      mainGui->setLight();

      return true;
   }

   return false;
}

/***********************************************************************
 *                         doLowMemoryClean                            *
 ***********************************************************************/
void MapEditor::doLowMemoryClean()
{
}

/***********************************************************************
 *                        doSendToBackground                           *
 ***********************************************************************/
void MapEditor::doSendToBackground()
{
}

/***********************************************************************
 *                        doSendToForeground                           *
 ***********************************************************************/
void MapEditor::doSendToForeground()
{
}

/***********************************************************************
 *                          checkKeyboardInput                         *
 ***********************************************************************/
bool MapEditor::checkKeyboardInput()
{
   bool res = false;

   if((Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_LCTRL)) ||
      (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_RCTRL)))
   {
      if((lastKey != Kobold::KOBOLD_KEY_D) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_D)))
      {
         /* Should duplicate current selection */
         mainGui->duplicateSelection();
         lastKey = Kobold::KOBOLD_KEY_D;
         res = true;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_BACKSPACE) && 
              (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_BACKSPACE)))
      {
         /* Should unselect current selection */
         mainGui->removeSelection();
         lastKey = Kobold::KOBOLD_KEY_BACKSPACE;
         thingUnderCursor = NULL;
         res = true;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_U) && 
              (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_U)))
      {
         /* Should unselect current selection */
         mainGui->unselect();
         lastKey = Kobold::KOBOLD_KEY_U;
         res = true;
      }
   }
   else if((Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_LALT)) ||
           (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_RALT)))
   {
      if((lastKey != Kobold::KOBOLD_KEY_N) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_N)))
      {
         /* Node window toggle */
         mainGui->toggleNodeWindow();
         lastKey = Kobold::KOBOLD_KEY_N;
         res = true;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_T) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_T)))
      {
         /* Transform window toggle */
         mainGui->toggleTransformWindow();
         lastKey = Kobold::KOBOLD_KEY_T;
         res = true;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_L) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_L)))
      {
         /* Transform window toggle */
         mainGui->toggleLightWindow();
         lastKey = Kobold::KOBOLD_KEY_L;
         res = true;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_M) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_M)))
      {
         /* Metadata window toggle */
         mainGui->toggleMetadataWindow();
         lastKey = Kobold::KOBOLD_KEY_M;
         res = true;
      }
   }
   
   if((lastKey != Kobold::KOBOLD_KEY_UNKNOWN) && 
      (!Kobold::Keyboard::isKeyPressed(lastKey)))
   {
      /* Last key no more pressed, must clear it */
      lastKey = Kobold::KOBOLD_KEY_UNKNOWN;
   }

   return res;
}

/***********************************************************************
 *                           specialSelect                             *
 ***********************************************************************/
bool MapEditor::specialSelect(Ogre::SceneNode* sceneNode)
{
   if(DNT::Game::getCurrentMap() != NULL)
   {
      return positionEditor->selectAxis(sceneNode);
   }
   return false;
}

/***********************************************************************
 *                          doBeforeRender                             *
 ***********************************************************************/
void MapEditor::doBeforeRender()
{
   if((DNT::Game::getCurrentMap()) && (mainGui->isLightEnabled()) &&
      (!Selection::hasSelection()))
   {
      /* update active light by mouse world position */
      DNT::Game::getCurrentMap()->update(floorMouse);
   }
   
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      treatedGui = true;
      shouldExit |= mainGui->checkEvents();
   }
   else
   {
      treatedGui = false;
   }
}

/***********************************************************************
 *                          doAfterRender                              *
 ***********************************************************************/
void MapEditor::doAfterRender()
{
   if(!treatedGui)
   {
      mainGui->update();

      /* Let's check keyboard input for selections */
      if(!checkKeyboardInput())
      {
         /* No valid input via keyboard happened, so no changes on world
          * after the render: we can check mouse world and do ray queries.*/

         /* Update mouse world and thing under mouse */
         updateMouseWorld(!positionEditor->isMoving());

         /* Let's update our position editor */
         positionEditor->update(leftButtonPressed, floorMouse, mouseX, mouseY);
      }
   }

   if(thingUnderCursor)
   {
      Farso::Cursor::setTextualTip(thingUnderCursor->getName());
      if(Farso::Cursor::checkButtonRelease(0))
      {
         /* Must select it! */
         if(Selection::getSelectedThing() != thingUnderCursor)
         {
            Selection::selectThing(thingUnderCursor);
            mainGui->getNodesWindow()->setSelectedNodeByData(thingUnderCursor);
         }
      }
   }
}

/***********************************************************************
 *                            shouldQuit                               *
 ***********************************************************************/
bool MapEditor::shouldQuit()
{
   return shouldExit;
}

