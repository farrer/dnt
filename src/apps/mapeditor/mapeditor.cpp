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

#include "../../core/game.h"

#include "../../map/map.h"

#include "../../rules/thing.h"

#include "../../gui/briefing.h"

#include <goblin/camera.h>
#include <goblin/model3d.h>

#include <OGRE/Compositor/OgreCompositorManager2.h>

using namespace DNTMapEditor;

/***********************************************************************
 *                                  MapEditor                               *
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
 *                                 ~MapEditor                               *
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
            getDataPath() + "textures/mapeditor", "FileSystem", 
            "mapeditor_textures", true);
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
            "mapeditor_textures", true);

      /* Create Map Editor's GUI */
      mainGui = new MainGui();
      mainGui->hideProgressBar();
      mainGui->showTopBar();
      mainGui->setLight();

      /* define our position editor */
      positionEditor = new PositionEditor(getSceneManager());

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
void MapEditor::checkKeyboardInput()
{
   if((Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_LCTRL)) ||
      (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_RCTRL)))
   {
      if((lastKey != Kobold::KOBOLD_KEY_D) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_D)))
      {
         /* Should duplicate current selection */
         mainGui->duplicateSelection(positionEditor);
         lastKey = Kobold::KOBOLD_KEY_D;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_BACKSPACE) && 
              (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_BACKSPACE)))
      {
         /* Should unselect current selection */
         mainGui->removeSelection(positionEditor);
         lastKey = Kobold::KOBOLD_KEY_BACKSPACE;
         thingUnderCursor = NULL;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_U) && 
              (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_U)))
      {
         /* Should unselect current selection */
         mainGui->unselect(positionEditor);
         lastKey = Kobold::KOBOLD_KEY_U;
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
      }
      else if((lastKey != Kobold::KOBOLD_KEY_T) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_T)))
      {
         /* Transform window toggle */
         mainGui->toggleTransformWindow();
         lastKey = Kobold::KOBOLD_KEY_T;
      }
      else if((lastKey != Kobold::KOBOLD_KEY_M) && 
         (Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_M)))
      {
         /* Metadata window toggle */
         mainGui->toggleMetadataWindow();
         lastKey = Kobold::KOBOLD_KEY_M;
      }
   }
   
   if((lastKey != Kobold::KOBOLD_KEY_UNKNOWN) && 
      (!Kobold::Keyboard::isKeyPressed(lastKey)))
   {
      /* Last key no more pressed, must clear it */
      lastKey = Kobold::KOBOLD_KEY_UNKNOWN;
   }
}

/***********************************************************************
 *                           specialSelect                             *
 ***********************************************************************/
bool MapEditor::specialSelect(Ogre::SceneNode* sceneNode)
{
   return positionEditor->selectAxis(sceneNode);
}

/***********************************************************************
 *                          doBeforeRender                             *
 ***********************************************************************/
void MapEditor::doBeforeRender()
{
   if((DNT::Game::getCurrentMap()) && (mainGui->isLightEnabled()) &&
      (!positionEditor->hasSelection()))
   {
      /* update active light by mouse world position */
      DNT::Game::getCurrentMap()->update(floorMouse);
   }
   
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      treatedGui = true;
      shouldExit |= mainGui->checkEvents(positionEditor);
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
      mainGui->update(positionEditor);

      /* Let's check keyboard input for selections */
      checkKeyboardInput();
 
      /* Update mouse world and thing under mouse */
      updateMouseWorld(!positionEditor->isMoving());

      /* Let's update our position editor */
      positionEditor->update(leftButtonPressed, floorMouse, mouseX, mouseY);
   }

   if(thingUnderCursor)
   {
      Farso::Cursor::setTextualTip(thingUnderCursor->getName());
      if(Farso::Cursor::checkButtonRelease(0))
      {
         /* Must select it! */
         if(positionEditor->getSelectedThing() != thingUnderCursor)
         {
            positionEditor->selectThing(thingUnderCursor);
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

