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

#include "dialogwindow.h"
#include "../rules/thing.h"
#include "../core/playablecharacter.h"
#include "../core/dialog.h"
#include "../script/scriptmanager.h"
#include "../core/game.h"

#include <assert.h>

using namespace DNT;

/**************************************************************************
 *                                    init                                *
 **************************************************************************/
void DialogWindow::init(const Kobold::String& widgetFilename)
{
   mutex.lock();

   script = Game::getScriptManager()->createDialogWidgetScriptInstance(
         widgetFilename);
   rootId = script->getRootWidgetId();

   owner = NULL;
   window = NULL;
   picture = NULL;
   ownerText = NULL;
   pcOptions = NULL;
   keyPressed = -1;
   lastPosX = -1;
   lastPosY = -1;
   toOpenOwner = NULL; 
   toOpenPC = NULL;
   shouldClose = false;

   mutex.unlock();
}

/**************************************************************************
 *                                  finish                                *
 **************************************************************************/
void DialogWindow::finish()
{
   mutex.lock();
   closeNow();
   rootId = "";
   if(script)
   {
      Game::getScriptManager()->removeInstance(script);
      script = NULL;
   }
   mutex.unlock();
}

/**************************************************************************
 *                                    open                                *
 **************************************************************************/
void DialogWindow::open(Thing* owner, PlayableCharacter* pc)
{
   assert(owner != NULL);
   assert(owner->getConversation() != NULL);
   assert(pc != NULL);

   /* Note: as we create a GL texture for the window, we must
    * only open the window on the main thread. Thus, here we just 
    * mark to open the dialog window latter, on checkEvents call
    * by the main thread. */

   mutex.lock();
   toOpenOwner = owner;
   toOpenPC = pc;
   mutex.unlock();
}

/**************************************************************************
 *                                 openNow                                *
 **************************************************************************/
void DialogWindow::openNow(Thing* owner, PlayableCharacter* pc)
{
   assert(owner != NULL);
   assert(owner->getConversation() != NULL);
   assert(pc != NULL);
   
   /* Close the window, if currently opened */
   if((!rootId.empty()) && (Farso::Controller::getWidgetById(rootId) == NULL))
   {
      /* Open the widget */
      DialogWidgetScript* dialogScript = static_cast<DialogWidgetScript*>(
            script->getScript());

      /* Note: as we shouldn't map the Ogre::Renderable buffer more than once
       * per frame, we should not set the window position if we plan to 
       * change it latter. */
      bool changePos = (lastPosX != -1) && (lastPosY != -1);
      Farso::Controller::insertFromJson(dialogScript->getJson(), script,
            !changePos);
      
      /* Reset to our last position, if any (to let the widget open
       * on the area the user liked it to be). */
      if(changePos)
      {
         Farso::Widget* widget = Farso::Controller::getWidgetById(rootId);
         if((widget->getX() != lastPosX) || (widget->getY() != lastPosY))
         {
            widget->setPosition(lastPosX, lastPosY);
         }
         if(widget->getType() == Farso::Widget::WIDGET_TYPE_WINDOW)
         {
            Farso::Window* window = static_cast<Farso::Window*>(widget);
            window->open();
         }
      }
   }

   /* Define our pointers and set conversation */
   DialogWindow::owner = owner;
   owner->getConversation()->setPlayableCharacter(pc);
   script->setOwner(owner);

   /* Set the initial dialog. */
   owner->getConversation()->changeDialog();
}

/**************************************************************************
 *                                checkEvents                             *
 **************************************************************************/
bool DialogWindow::checkEvents()
{
   mutex.lock();

   if(rootId.empty())
   {
      /* Dialog undefined, nothing to do here. */
      mutex.unlock();
      return false;
   }

   /* Check if we should close the current window */
   if(shouldClose)
   {
      closeNow();
      shouldClose = false;
      mutex.unlock();
      return true;
   }

   /*! Check if we should open a new dialog */
   if(toOpenOwner != NULL)
   {
      openNow(toOpenOwner, toOpenPC);
      toOpenOwner = NULL;
      toOpenPC = NULL;
      mutex.unlock();
      return true;
   }

   bool checkKeyboard = true;
   Farso::Widget* widget = Farso::Controller::getWidgetById(rootId);
   if(widget)
   {
      if(widget->getParent() == NULL)
      {
         /* Keep last widget position, to open at the same next time */
         lastPosX = widget->getWidgetRenderer()->getPositionX();
         lastPosY = widget->getWidgetRenderer()->getPositionY();
      }
      if(widget->getType() == Farso::Widget::WIDGET_TYPE_WINDOW)
      {
         /* Should only check keyboard if the window is the active one. */
         Farso::Window* window = static_cast<Farso::Window*>(widget);
         checkKeyboard = Farso::Controller::getActiveWindow() == window;
      }
   }
   else
   {
      /* Dialog not opened, nothing to check here. */
      mutex.unlock();
      return false;
   }

   if(checkKeyboard)
   {
      /* Check option selection by key */
      if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_1))
      {
         keyPressed = 1;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_2))
      {
         keyPressed = 2;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_3))
      {
         keyPressed = 3;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_4))
      {
         keyPressed = 4;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_5))
      {
         keyPressed = 5;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_6))
      {
         keyPressed = 6;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_7))
      {
         keyPressed = 7;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_8))
      {
         keyPressed = 8;
      }
      else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_9))
      {
         keyPressed = 9;
      }
      else if(keyPressed != -1)
      {
         /* Selected an option by keyboard. */
         script->selectOption(keyPressed - 1);

         mutex.unlock();
         return true;
      }
   }
   else
   {
      keyPressed = -1;
   }

   int curSelected = script->getSelectedOption();
   if(curSelected >= 0)
   {
      owner->getConversation()->proccessAction(curSelected);
   }


   mutex.unlock();
   return curSelected > 0;
}

/**************************************************************************
 *                                  isOpened                              *
 **************************************************************************/
bool DialogWindow::isOpened()
{
   mutex.lock();

   bool res = false;
   if(!rootId.empty())
   {
      res = Farso::Controller::getWidgetById(rootId) != NULL;
   } 
   
   mutex.unlock();
   
   return res;
}

/**************************************************************************
 *                                  isOpened                              *
 **************************************************************************/
bool DialogWindow::isOpened(Thing* owner)
{
   bool res = false;

   mutex.lock();
   if(!rootId.empty())
   {
      res = (Farso::Controller::getWidgetById(rootId) != NULL) && 
         (DialogWindow::owner == owner);
   }
   mutex.unlock();

   return res;
}

/**************************************************************************
 *                                    close                               *
 **************************************************************************/
void DialogWindow::close()
{
   mutex.lock();
   shouldClose = true;
   mutex.unlock();
}

/**************************************************************************
 *                                  closeNow                              *
 **************************************************************************/
void DialogWindow::closeNow()
{
   if(!rootId.empty())
   {
      Farso::Widget* widget = Farso::Controller::getWidgetById(rootId);
      if(widget)
      {
         Farso::Controller::markToRemoveWidget(widget);
      }
   }
}

/**************************************************************************
 *                                   clear                                *
 **************************************************************************/
void DialogWindow::clear()
{
   if(script)
   {
      script->clear();
   }
}

/**************************************************************************
 *                               setOwnerText                             *
 **************************************************************************/
void DialogWindow::setOwnerText(const Kobold::String& text)
{
   if(script)
   {
      script->setOwnerText(text);
   }
}

/**************************************************************************
 *                                 addOption                              *
 **************************************************************************/
void DialogWindow::addOption(const Kobold::String& text, int index)
{
   if(script)
   {
      script->addOption(text, index);
   }
}

/**************************************************************************
 *                               Static Members                           *
 **************************************************************************/
Thing* DialogWindow::owner = NULL;
Farso::Window* DialogWindow::window = NULL;
Farso::Picture* DialogWindow::picture = NULL;
Farso::ScrollText* DialogWindow::ownerText = NULL;
Farso::TextSelector* DialogWindow::pcOptions = NULL;
int DialogWindow::keyPressed = -1;
int DialogWindow::lastPosX = -1;
int DialogWindow::lastPosY = -1;
Kobold::Mutex DialogWindow::mutex;
Thing* DialogWindow::toOpenOwner = NULL; 
PlayableCharacter* DialogWindow::toOpenPC = NULL;
bool DialogWindow::shouldClose = false;
Kobold::String DialogWindow::rootId= "";
DialogWidgetScriptInstance* DialogWindow::script = NULL;

