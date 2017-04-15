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
#include "object.h"

#include "../map/map.h"

#include "../rules/alignment.h"
#include "../rules/feats.h"
#include "../rules/race.h"
#include "../rules/classes.h"
#include "../rules/skills.h"

#include "../gui/briefing.h"
#include "../gui/dialogwindow.h"

#include "../core/playablecharacter.h"
#include "../collision/collision.h"

#include <goblin/camera.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>

using namespace DNT;

/***********************************************************************
 *                                  Core                               *
 ***********************************************************************/
Core::Core()
{
   testedThingUnderCursor = NULL;
   testedAtSightResult = false;
}

/***********************************************************************
 *                                 ~Core                               *
 ***********************************************************************/
Core::~Core()
{
}

/***********************************************************************
 *                            doCycleInit                              *
 ***********************************************************************/
bool Core::doCycleInit(int callCounter, bool& shouldAbort)
{
   if(doCommonCycleInit(callCounter, shouldAbort))
   {
      //FIXME: for outdoor maps, must define the hemisphere colors with
      //       different values.
      getSceneManager()->setAmbientLight(
            Ogre::ColourValue(0.1f, 0.1f, 0.1f),
            Ogre::ColourValue(0.1f, 0.1f, 0.1f),
            Ogre::Vector3(0.0f, 1.0f, 0.0f));
      Goblin::Camera::disableTranslations();

      /* Load a PC */
      PlayableCharacter* pc = new PlayableCharacter();
      pc->load("humans/padre.npc");
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

      return true;
   }

   return false;
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
 *                           specialSelect                             *
 ***********************************************************************/
bool Core::specialSelect(Ogre::SceneNode* sceneNode)
{
   return false;
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

   bool treatedGui = Farso::Controller::verifyEvents(leftButtonPressed, false, 
         mouseX, mouseY);
   treatedGui |= DialogWindow::checkEvents();

   if(!treatedGui)
   {
      /* Update mouse world and thing under mouse */
      updateMouseWorld(true);

      /* Check current PC movement input */
      if(curPc->checkInputForMovement(floorMouse))
      {
         /* Must disable thngUnderCursor if trying to move */
         thingUnderCursor = NULL;
         testedThingUnderCursor = NULL;
      }
   }
 
   /* Check actions with things under mouse */
   if(thingUnderCursor)
   {
      /* Verify if we need to check atSight again or use the cached result */
      if(thingUnderCursor != testedThingUnderCursor)
      {
         /* Recheck */
         testedAtSightResult = Collision::isAtSight(curPc, thingUnderCursor);
         testedThingUnderCursor = thingUnderCursor;
      }
      
      /* Only set textual tip (and act on the thingUnderCursor) if at sight */
      if(testedAtSightResult)
      {
         Farso::Cursor::setTextualTip(thingUnderCursor->getName());
         if(Farso::Cursor::checkButtonRelease(0))
         {
            // TODO: Check action range; 
            // TODO: Check if have actions when on battle mode.
            if(thingUnderCursor->canInteract())
            {
               Object::InteractResult res = thingUnderCursor->interact(curPc);
               if(res == Object::INTERACTION_OPEN_CONVERSATION)
               {
                  DialogWindow::open(thingUnderCursor, curPc);
               }
            }
         }
      }
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

