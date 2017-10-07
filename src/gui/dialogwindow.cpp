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

#include <assert.h>

using namespace DNT;

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
   if(window != NULL)
   {
      if(DialogWindow::owner == owner)
      {
         /* Dialog is already opened for the same owner, let's keep it. */
         return;
      }
      window->close();
   }

   /* Define our pointers and set conversation */
   DialogWindow::owner = owner;
   owner->getConversation()->setPlayableCharacter(pc);

   /* Create the window */
   window = new Farso::Window(480, 360, owner->getName());

   if((lastPosX == -1) || (lastPosY == -1))
   {
      /* Use default position */
      window->setPosition(Farso::Controller::getWidth() - 480, 0);
   }
   else
   {
      /* Use last position */
      window->setPosition(lastPosX, lastPosY);
   }

   /* Barter button (only for characters) */
   if(owner->getThingType() == Thing::THING_TYPE_CHARACTER)
   {
      //TODO
   }
   /* Add portrait picture */
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_TOP_CENTERED,
         Farso::Rect(0, 0, 390, 0), window);
   assert(owner->getPortraitFilename() != "");
   picture = new Farso::Picture(0, 0, owner->getPortraitFilename(), cont);

   /* Add the owner's scrollable text */
   cont = new Farso::Container(Farso::Container::TYPE_TOP_RIGHT, window);
   ownerText = new Farso::ScrollText(0, 0, 390, 108, cont);

   /* Add the text selector for PC options */
   cont = new Farso::Container(
         Farso::Container::TYPE_TOP_LEFT,
         0, 110, window->getBody().getWidth() - 32, 10, window);
   pcOptions = new Farso::TextSelector(cont);

   window->setExternPointer(&window);
   window->open();

   /* Set the initial dialog. */
   owner->getConversation()->changeDialog();
}

/**************************************************************************
 *                                checkEvents                             *
 **************************************************************************/
bool DialogWindow::checkEvents()
{

   mutex.lock();

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

   if(window != NULL)
   {
      if(Farso::Controller::getActiveWindow() == window)
      {
         /* Update our last coordinates, to keep them on next open */
         lastPosX = window->getWidgetRenderer()->getPositionX();
         lastPosY = window->getWidgetRenderer()->getPositionY();

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
            /* Selected an option by keyabord. */
            if(pcOptions->haveOption(keyPressed - 1))
            {
               pcOptions->forceSelection(keyPressed - 1);
               /* TODO: Play gui sound */
               /* Proccess the action */
               owner->getConversation()->proccessAction(
                     pcOptions->getSelectedOption());
            }
            keyPressed = -1;

            mutex.unlock();
            return true;
         }

         if(keyPressed != -1)
         {
            /* Done the current selection by keyboard */
            pcOptions->forceSelection(keyPressed - 1);
         }
         else
         {
            /* Check selection by Farso event */
            Farso::Event event = Farso::Controller::getLastEvent();
            if((event.getType() == Farso::EVENT_TEXTSELECTOR_OPTION_SELECTED) &&
                  (event.getWidget() == pcOptions))
            {
               owner->getConversation()->proccessAction(
                     pcOptions->getSelectedOption());
               mutex.unlock();
               return true;
            }
            //TODO: check barter button.
         }
      }
      else
      {
         keyPressed = -1;
      }

      /* Check window close (needed, to keep our pointer access atomic). */
      Farso::Event event = Farso::Controller::getLastEvent();
      if((event.getType() == Farso::EVENT_WINDOW_WILL_CLOSE) &&
            (event.getWidget() == window))
      {
         /* Unset the pointer */
         window->clearExternPointer();
         window = NULL;
         mutex.unlock();
         return true;
      }
   }

   mutex.unlock();
   return false;
}

/**************************************************************************
 *                                  isOpened                              *
 **************************************************************************/
bool DialogWindow::isOpened()
{
   mutex.lock();
   bool res = (window != NULL);
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
   if(window)
   {
      res = (DialogWindow::owner == owner);
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
   if(window)
   {
      window->close();
   }
}

/**************************************************************************
 *                                   clear                                *
 **************************************************************************/
void DialogWindow::clear()
{
   if(window)
   {
      ownerText->clear();
      pcOptions->clearOptions();
   }
}

/**************************************************************************
 *                               setOwnerText                             *
 **************************************************************************/
void DialogWindow::setOwnerText(const Kobold::String& text)
{
   if(window)
   {
      ownerText->setText(text);
   }
}

/**************************************************************************
 *                                 addOption                              *
 **************************************************************************/
void DialogWindow::addOption(const Kobold::String& text, int index)
{
   if(window)
   {
      pcOptions->addOption(text, index);
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
