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

#include "../../rules/alignment.h"
#include "../../rules/feats.h"
#include "../../rules/race.h"
#include "../../rules/classes.h"
#include "../../rules/skills.h"
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
            "mapeditor_textures");

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
 *                              doCycle                                *
 ***********************************************************************/
void MapEditor::doCycle()
{
   if((DNT::Game::getCurrentMap()) && (mainGui->isLightEnabled()) &&
      (!positionEditor->hasSelection()))
   {
      /* update active light by mouse world position */
      DNT::Game::getCurrentMap()->update(floorMouse);
   }
   
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      shouldExit |= mainGui->checkEvents(positionEditor);
   }
   else
   {
      mainGui->update(positionEditor);

      /* Let's check keyboard input for selections */
      checkKeyboardInput();

      if(!Farso::Controller::wasMouseOverWidget())
      {
         if((lastMouseX != mouseX) || (lastMouseY != mouseY))
         {
            /* The floor mouse could change with camera move too, but I 
             * believe it's no problem to only update it with mouse movement
             * after all. */
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            /* Calculate floor mouse coordinates */
            //FIXME: must use the 'floor point' to later get its height.
            Ogre::Ray mouseRay;
            Goblin::Camera::getCameraToViewportRay(
                  mouseX / Ogre::Real(ogreWindow->getWidth()),
                  mouseY / Ogre::Real(ogreWindow->getHeight()), &mouseRay);

            /* with a ray cast to Y=0 plane */
            std::pair< bool, Ogre::Real > res;
            res = Ogre::Math::intersects(mouseRay, 
                  Ogre::Plane(Ogre::Vector3(0.0f, 1.0f, 0.0f), 0.0f));
            if(res.first)
            {
               floorMouse = mouseRay.getPoint(res.second);
            }

            /* Let's get if there's a 'thing' under mouse at current map */
            if((DNT::Game::getCurrentMap()) && (!positionEditor->isMoving()))
            {
               thingUnderCursor = NULL;
               ogreRaySceneQuery->setRay(mouseRay);
               ogreRaySceneQuery->setSortByDistance(true);
               Ogre::RaySceneQueryResult &result = ogreRaySceneQuery->execute();
               Ogre::RaySceneQueryResult::iterator itr;

               for( itr = result.begin( ); itr != result.end(); itr++ )
               {
                  /* Note: distance == 0 are our widgets */
                  if((itr->movable) && (itr->distance > 0))
                  {
                     Ogre::SceneNode* sceneNode = 
                        itr->movable->getParentSceneNode();

                     if(!thingUnderCursor)
                     {
                        /* Get the thing related to the sceneNode, if any */
                        thingUnderCursor = DNT::Game::getCurrentMap()->getThing(
                              sceneNode);
                     }

                     if(itr->movable->getName().substr(0,4) == "wall")
                     {
                        /* Pointing at wall. Must end. */
                        //TODO: select wall
                     }
                     else if(itr->movable->getName().substr(0,5) == "floor")
                     {
                        /* Note: the floor distance seems lesser than
                         * the real intersecton point (probably due to the 
                         * it being a great mesh) */
                     }
                     else if(positionEditor->selectAxis(sceneNode))
                     {
                        /* We are now editing a position! */
                        thingUnderCursor = NULL;
                        break;
                     }
                  }
               }
            }
         }
      }
      else
      {
         /* Mouse is over a widget, so shouldn't have any thing under it */
         thingUnderCursor = NULL;
      }

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

