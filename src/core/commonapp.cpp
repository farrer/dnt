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

#include "../rules/alignment.h"
#include "../rules/feats.h"
#include "../rules/race.h"
#include "../rules/classes.h"
#include "../rules/skills.h"

#include "../gui/briefing.h"

#include "../core/playablecharacter.h"

#include <goblin/camera.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>


using namespace DNT;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
CommonApp::CommonApp()
{
   thingUnderCursor = NULL;
   lastMouseX = -1;
   lastMouseY = -1;
   floorMouse = Ogre::Vector3(0.0f, 0.0f, 0.0f);
   ogreRaySceneQuery = NULL;
   progressBar = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
CommonApp::~CommonApp()
{
   DNT::Game::finish();

   DNT::Alignments::finish();
   DNT::SkillsDefinitions::finish();
   DNT::Races::finish();
   DNT::Classes::finish();
   DNT::FeatsList::finish();
   DNT::Briefing::finish();
   Farso::Controller::finish();

   if(ogreRaySceneQuery)
   {
      ogreSceneManager->destroyQuery(ogreRaySceneQuery);
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

         /* Init our rules */
         DNT::Alignments::init();
         DNT::SkillsDefinitions::init();
         DNT::FeatsList::init("feats/", "feats.ftl");
         DNT::Races::init();
         DNT::Classes::init();
         progressBar->setValue(20);
      }
      break;
      case 3:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/doors", "FileSystem", 
               "doors_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "doors_textures");
         progressBar->setValue(30);
      }
      break;
      case 4:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/furniture", "FileSystem", 
               "furniture_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "furniture_textures");
         progressBar->setValue(40);
      }
      break;
      case 5:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/general", "FileSystem", 
               "general_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "general_textures");
         progressBar->setValue(50);
      }
      break;
      case 6:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/humans", "FileSystem", 
               "humans_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "humans_textures");
         progressBar->setValue(60);
      }
      break;
      case 7:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/portraits", "FileSystem", 
               "portraits_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "portraits_textures");
         progressBar->setValue(70);
      }
      break;
      case 8:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/junk", "FileSystem", 
               "junk_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "junk_textures");
         progressBar->setValue(80);
      }
      break;
      case 9:
      {
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
               getDataPath() + "textures/indoor", "FileSystem", 
               "indoor_textures", true);
         Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
               "indoor_textures");
         progressBar->setValue(90);
      }
      break;
      case 10:
      {
         /* Init the Game controller */
         DNT::Game::init(ogreSceneManager);

         /* Set camera initial position */
         Goblin::Camera::set(200.0f, 30.0f, 120.0f, 0.0f, 89.0f, 410.0f);
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
   static Ogre::String dirs[] = {"gui", "compositors", "maps", "models", 
      "fonts", "rules", "skeletons"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 7;
}

