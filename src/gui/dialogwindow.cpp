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

   /* Close the window, if currently opened */
   if(isOpened())
   {
      if(DialogWindow::owner == owner)
      {
         /* Dialog is already opened for the same owner, let's keep it. */
         return;
      }
      close();
   }

   /* Define our pointers and set conversation */
   DialogWindow::owner = owner;
   owner->getConversation()->setPlayableCharacter(pc);

   /* Create the window */
   window = new Farso::Window(450, 360, owner->getName());
   window->setPosition((Farso::Controller::getWidth() / 2) - 225, 100);

   /* Barter button (only for characters) */
   if(owner->getThingType() == Thing::THING_TYPE_CHARACTER)
   {
      //TODO
   }
   /* Add portrait picture */
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_TOP_CENTERED, 
         Farso::Rect(5, 2, 320, 0), window);
   assert(owner->getPortraitFilename() != "");
   picture = new Farso::Picture(0, 0, owner->getPortraitFilename(), cont);

   /* Add the owner's scrollable text */
   cont = new Farso::Container(Farso::Container::TYPE_TOP_RIGHT, window);
   ownerText = new Farso::ScrollText(0, 0, 320, 90, cont);

   /* Add the text selector for PC options */
   cont = new Farso::Container(
         Farso::Container::TYPE_TOP_CENTERED,
         0, 100, window->getBody().getWidth() - 32, 120, window);
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
   if(!isOpened())
   {
      return false;
   }

   if(Farso::Controller::getActiveWindow() == window)
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
            return true;
         }
         //TODO: check barter button.
      }
   }
   else
   {
      keyPressed = -1;
   }

   return false;
}

/**************************************************************************
 *                                  isOpened                              *
 **************************************************************************/
bool DialogWindow::isOpened()
{
   return window != NULL;
}

/**************************************************************************
 *                                  isOpened                              *
 **************************************************************************/
bool DialogWindow::isOpened(Thing* owner)
{
   if(isOpened())
   {
      return DialogWindow::owner == owner;
   }

   return false;
}

/**************************************************************************
 *                                    close                               *
 **************************************************************************/
void DialogWindow::close()
{
   if(window)
   {
      window->close();
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

