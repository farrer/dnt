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

#include "core.h"
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
 *                                  Core                               *
 ***********************************************************************/
Core::Core()
{
   lastMouseX = -1;
   lastMouseY = -1;
   floorMouse = Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

/***********************************************************************
 *                                 ~Core                               *
 ***********************************************************************/
Core::~Core()
{
   Game::finish();

   Alignments::finish();
   SkillsDefinitions::finish();
   Races::finish();
   Classes::finish();
   FeatsList::finish();
   Briefing::finish();
   Farso::Controller::finish();
}

/***********************************************************************
 *                            doCycleInit                              *
 ***********************************************************************/
bool Core::doCycleInit(int callCounter, bool& shouldAbort)
{
   //getSceneManager()->setShadowTextureSelfShadow(true);
   
   //FIXME: for outdoor maps, must define the hemisphere colors with
   //       different values.
   getSceneManager()->setAmbientLight(
         Ogre::ColourValue(0.1f, 0.1f, 0.1f),
         Ogre::ColourValue(0.1f, 0.1f, 0.1f),
         Ogre::Vector3(0.0f, 1.0f, 0.0f));

   /* Init Farso */
   Farso::Controller::init(Farso::RENDERER_TYPE_OGRE3D, DEFAULT_WINDOW_WIDTH,
       DEFAULT_WINDOW_HEIGHT, 32, "", getSceneManager());
   Farso::FontManager::setDefaultFont("LiberationSans-Regular.ttf");
   Farso::Controller::loadSkin("skins/moderna.skin");
   Farso::Controller::setCursor("cursor/sel.png");

   /* Init our widgets */
   Briefing::init();

   /* Init our rules */
   Alignments::init();
   SkillsDefinitions::init();
   FeatsList::init("feats/", "feats.ftl");
   Races::init();
   Classes::init();

   Game::init(ogreSceneManager);
   Goblin::Camera::disableTranslations();

   /* Load a PC */
   PlayableCharacter* pc = new PlayableCharacter();
   pc->load("humans/velhaca.npc");
   pc->setAnimation(Character::CHARACTER_ANIMATION_IDLE, true);

   Game::getPcs()->insertCharacter(pc);

   /* Load a map to test. FIXME: remove from here when reimplemented our
    * initial window. */
   if(!Game::loadMap("tyrol/house1.map"))
   {
      shouldAbort = true;
      return true;
   }
   Goblin::Camera::setPosition(pc->getModel()->getPosition());

   /* FIXME: should not be here, but at map load. */
   /* Change workspace to cdefine map type */
   Ogre::CompositorManager2 *compositorManager =
      ogreRoot->getCompositorManager2();
   ogreWorkspace = compositorManager->addWorkspace(ogreSceneManager,
         ogreWindow, Goblin::Camera::getOgreCamera(),
         "DNTIndoorWorkspace", true);

   getSceneManager()->setForward3D(true, 4, 4, 4, 32, 3, 2000);

   return true;
}

/***********************************************************************
 *                        getDataDirectories                           *
 ***********************************************************************/
void Core::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"gui", "textures/doors", "textures/furniture",
      "textures/general", "textures/humans", "textures/indoor", 
      "textures/junk", "textures/portraits", "maps", "models", "fonts",
      "compositors", "rules", "skeletons"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 14;
}

/***********************************************************************
 *                         doLowMemoryClean                            *
 ***********************************************************************/
void Core::doLowMemoryClean()
{
   //TODO, when and if supporting mobile platforms.
}

/***********************************************************************
 *                        doSendToBackground                           *
 ***********************************************************************/
void Core::doSendToBackground()
{
   //TODO, when and if supporting mobile platforms.
}

/***********************************************************************
 *                        doSendToForeground                           *
 ***********************************************************************/
void Core::doSendToForeground()
{
   //TODO, when and if supporting mobile platforms.
}

/***********************************************************************
 *                              doCycle                                *
 ***********************************************************************/
void Core::doCycle()
{
   /* Define current active playable character */
   PlayableCharacter* curPc = static_cast<PlayableCharacter*>(
            Game::getPcs()->getActiveCharacter());

   /* Update current models and map */
   Game::update();
   
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      //TODO;
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


      /* Check current PC movement input */
      curPc->checkInputForMovement();
   }
}

/***********************************************************************
 *                            shouldQuit                               *
 ***********************************************************************/
bool Core::shouldQuit()
{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   return Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_ESCAPE);
#endif
   return false;
}

