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

#include "commonapp.h"
#include "game.h"

#include "../map/map.h"

#include "../rules/ruledef.h"

#include "../gui/briefing.h"

#include "../core/playablecharacter.h"

#include <goblin/camera.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>

using namespace DNT;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
CommonApp::CommonApp()
          :BaseApp(Goblin::CameraConfig())
{
   thingUnderCursor = NULL;
   lastMouseX = -1;
   lastMouseY = -1;
   floorMouse = Ogre::Vector3(0.0f, 0.0f, 0.0f);
   ogreRaySceneQuery = NULL;
   progressBar = NULL;
   renderer = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
CommonApp::~CommonApp()
{
   DNT::Game::finish();

   DNT::Briefing::finish();
   Farso::Controller::finish();

   if(ogreRaySceneQuery != NULL)
   {
      ogreSceneManager->destroyQuery(ogreRaySceneQuery);
   }
   if(renderer != NULL)
   {
      delete renderer;
   }
}

/***********************************************************************
 *                             doCycleInit                             *
 ***********************************************************************/
bool CommonApp::doCommonCycleInit(int callCounter, bool& shouldAbort)
{
   bool done = false;
   switch(callCounter)
   {
      case 0:
      {
         /* FIXME: should not be here, but at map load. */
         /* Change workspace to cdefine map type */
         Ogre::CompositorManager2 *compositorManager =
            ogreRoot->getCompositorManager2();
         if(ogreWorkspace)
         {
            compositorManager->removeWorkspace(ogreWorkspace);
         }
         ogreWorkspace = compositorManager->addWorkspace(ogreSceneManager,
               ogreWindow->getTexture(), Goblin::Camera::getOgreCamera(), 
               "DNTIndoorWorkspace", true);

         getSceneManager()->setForward3D(true, 4, 4, 4, 32, 3, 2000);

         /* Init Farso */
         renderer = new Farso::OgreRenderer(getSceneManager(), 
               ogreRoot->getRenderSystem());
         Farso::Controller::init(&loader, renderer, 
               DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 32, "");
         Farso::FontManager::setDefaultFont("LiberationSans-Regular.ttf");
         Farso::Controller::loadSkin(getSkin());
         Farso::Controller::setCursor("cursor/sel.png");

         /* Create the progress bar */
         progressBar = new Farso::ProgressBar(412, 374, 200, 20, NULL);

         /* Define our progress bar */
         progressBar->show();
         progressBar->setValue(10);
         Farso::Cursor::hide();
      }
      break;
      case 1:
      {
         /* Init our widgets */
         DNT::Briefing::init();
         progressBar->setValue(20);
      }
      break;
      case 2:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "modules", "FileSystem", 
               "dnt_modules", false);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "dnt_modules", false);
      }
      case 3:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "common/textures/other", "FileSystem", 
               "dnt_other_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "dnt_other_textures", true);
         progressBar->setValue(50);
      }
      break;
      case 4:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "common/textures/general", "FileSystem", 
               "dnt_general_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "dnt_general_textures", true);
         progressBar->setValue(80);
      }
      break;
      case 5:
      {
         /* Init the Game controller */
         DNT::Game::init(ogreSceneManager);

         /* Set camera initial position */
         Goblin::Camera::set(0.0f, 2.0f, 0.0f, 0.0f, 70.0f, 22.30f);
         Farso::Cursor::show();
         
         /* Create a SceneQuery */
         ogreRaySceneQuery = ogreSceneManager->createRayQuery(Ogre::Ray());

         done = true;
      }
      break;
   }

   if(!done)
   {
      Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY);
   }
   else
   {
      progressBar->hide();
   }
   return done;
}

/***********************************************************************
 *                        getDataDirectories                           *
 ***********************************************************************/
void CommonApp::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"common/gui", "common/compositors", 
      "common/fonts", "common/models", "common/sndfx",
      "common/dialogs"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 6;
}

/***********************************************************************
 *                       shouldDoCameraInput                           *
 ***********************************************************************/
const bool CommonApp::shouldDoCameraInput()
{
   return (Farso::Controller::getLastEvent().getType() == Farso::EVENT_NONE) &&
          Goblin::BaseApp::shouldDoCameraInput();
}

/***********************************************************************
 *                        updateMouseWorld                             *
 ***********************************************************************/
void CommonApp::updateMouseWorld(bool updateThingUnderMouse)
{
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
         if((DNT::Game::getCurrentMap()) && (updateThingUnderMouse))
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
                     if(thingUnderCursor == NULL)
                     {
                        thingUnderCursor = DNT::Game::getPcs()->getCharacter(
                              sceneNode);
                     }
                  }

                  if(specialSelect(sceneNode))
                  {
                     /* We are now editing a position! */
                     thingUnderCursor = NULL;
                     break;
                  }
                  else if(itr->movable->getName().substr(0,4) == "wall")
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
               }
            }
         }
      }
   }
   else
   {
      /* Mouse is over a widget, so shouldn't have any thing under it */
      thingUnderCursor = NULL;
      specialSelect(NULL);
   }
}

