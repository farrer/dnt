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

#include "../../gui/briefing.h"

#include <goblin/camera.h>

#include <OGRE/Compositor/OgreCompositorManager2.h>

using namespace DNTMapEditor;

/***********************************************************************
 *                                  MapEditor                               *
 ***********************************************************************/
MapEditor::MapEditor()
{
   shouldExit = false;
   lastMouseX = -1;
   lastMouseY = -1;
   floorMouse = Ogre::Vector3(0.0f, 0.0f, 0.0f);
   mainGui = NULL;
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

   DNT::Game::finish();

   DNT::Alignments::finish();
   DNT::SkillsDefinitions::finish();
   DNT::Races::finish();
   DNT::Classes::finish();
   DNT::FeatsList::finish();
   DNT::Briefing::finish();
   Farso::Controller::finish();
}

/***********************************************************************
 *                             doCycleInit                             *
 ***********************************************************************/
bool MapEditor::doCycleInit(int callCounter, bool& shouldAbort)
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
         //compositorManager->removeWorkspace(ogreWorkspace);
         ogreWorkspace = compositorManager->addWorkspace(ogreSceneManager,
               ogreWindow, Goblin::Camera::getOgreCamera(), 
               "DNTIndoorWorkspace", true);

         getSceneManager()->setForward3D(true, 4, 4, 4, 32, 3, 2000);

         /* Init Farso */
         Farso::Controller::init(Farso::RENDERER_TYPE_OGRE3D, 
               DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 32, "",
               getSceneManager());
         Farso::FontManager::setDefaultFont("LiberationSans-Regular.ttf");
         Farso::Controller::loadSkin("skins/clean.skin");
         Farso::Controller::setCursor("cursor/sel.png");

         /* Create Map Editor's GUI */
         mainGui = new MainGui();

         /* Define our progress bar */
         mainGui->hideTopBar();
         mainGui->showProgressBar();
         mainGui->setProgressBar(10);
         Farso::Cursor::hide();
      }
      break;
      case 1:
      {
         /* Init our widgets */
         DNT::Briefing::init();

         /* Init our rules */
         DNT::Alignments::init();
         DNT::SkillsDefinitions::init();
         DNT::FeatsList::init("feats/", "feats.ftl");
         DNT::Races::init();
         DNT::Classes::init();
         mainGui->setProgressBar(20);
      }
      break;
      case 3:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/doors", "FileSystem", 
               "doors_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "doors_textures");
         mainGui->setProgressBar(30);
      }
      break;
      case 4:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/furniture", "FileSystem", 
               "furniture_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "furniture_textures");
         mainGui->setProgressBar(40);
      }
      break;
      case 5:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/general", "FileSystem", 
               "general_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "general_textures");
         mainGui->setProgressBar(50);
      }
      break;
      case 6:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/humans", "FileSystem", 
               "humans_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "humans_textures");
         mainGui->setProgressBar(60);
      }
      break;
      case 7:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/portraits", "FileSystem", 
               "portraits_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "portraits_textures");
         mainGui->setProgressBar(70);
      }
      break;
      case 8:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/junk", "FileSystem", 
               "junk_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "junk_textures");
         mainGui->setProgressBar(80);
      }
      break;
      case 9:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/indoor", "FileSystem", 
               "indoor_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "indoor_textures");
         mainGui->setProgressBar(90);

      }
      break;
      case 10:
      {
         /* Init the Game controller */
         DNT::Game::init(ogreSceneManager);

         /* Set camera initial position */
         Goblin::Camera::set(200.0f, 30.0f, 120.0f, 0.0f, 89.0f, 410.0f);

         /* set lights */
         mainGui->setLight();
         mainGui->hideProgressBar();
         mainGui->showTopBar();
         Farso::Cursor::show();
      }
      done = true;
      break;
   }

   if(!done)
   {
      Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY);
   }
   return done;
}

/***********************************************************************
 *                        getDataDirectories                           *
 ***********************************************************************/
void MapEditor::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"gui", "compositors", "maps", "models", 
      "fonts", "rules"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 6;
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
 *                              doCycle                                *
 ***********************************************************************/
void MapEditor::doCycle()
{
   if((DNT::Game::getCurrentMap()) && (mainGui->isLightEnabled()))
   {
      DNT::Game::getCurrentMap()->update(floorMouse);
   }
   
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      shouldExit |= mainGui->checkEvents();
   }
   else
   {
      if((lastMouseX != mouseX) && (lastMouseY != mouseY))
      {
         /* The floor mouse could change with camera move too, but I believe
          * it's no problem to only update it with mouse movement after all. */
         lastMouseX = mouseX;
         lastMouseY = mouseY;
         //FIXME: must just use the last collider? Or first collider project
         // to Y=0? Anyway, the raycast to plane Y=0 isn't the best way for DNT.
         /* Calculate floor mouse coordinates */
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

