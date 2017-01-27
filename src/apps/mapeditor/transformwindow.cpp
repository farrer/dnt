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

#include "transformwindow.h"
using namespace DNTMapEditor;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
TransformWindow::TransformWindow()
{
   window = NULL;
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
TransformWindow::~TransformWindow()
{
}

/************************************************************************
 *                                 open                                 *
 ************************************************************************/
void TransformWindow::open()
{
   if(!window)
   {
      window = new Farso::Window(320, 122, "Transform");
      window->setExternPointer(&window);

      /* Define each of our transforms */
      new Farso::Label(0, 0, 54, 21, "Position", window);
      new Farso::Label(0, 22, 54, 21, "Rotation", window);
      new Farso::Label(0, 44, 54, 21, "Scale", window);
      for(int i = 0; i < 3; i++)
      {
         (new Farso::Label(60, i * 22, 21, 21, "x:", window))->setFontColor(
            Farso::Color(255, 0, 0, 255));
         (new Farso::Label(145, i * 22, 21, 21, "y:", window))->setFontColor(
            Farso::Color(0, 255, 0, 255));
         (new Farso::Label(230, i * 22, 21, 21, "z:", window))->setFontColor(
            Farso::Color(0, 0, 255, 255));

         position[i] = new Farso::TextEntry(77 + (i * 85), 0, 60, 21, window);
         rotation[i] = new Farso::TextEntry(77 + (i * 85), 22, 60, 21, window);
         scale[i] = new Farso::TextEntry(77 + (i * 85), 44, 60, 21, window);
      }

      /* Open and set its position */
      window->open();
      window->setPosition(Farso::Controller::getWidth() - 320, 227);
   }
}

/************************************************************************
 *                                close                                 *
 ************************************************************************/
void TransformWindow::close()
{
   if(window)
   {
      window->close();
   }
}

/************************************************************************
 *                             setEnabled                               *
 ************************************************************************/
void TransformWindow::setEnabled(bool enable)
{
   if((enable) && (!scale[0]->isAvailable()))
   {
      /* Should enable to edit */
      for(int i = 0; i < 3; i++)
      {
         position[i]->enable();
         rotation[i]->enable();
         scale[i]->enable();
      }
   }
   else if((!enable) && (scale[0]->isAvailable()))
   {
      /* Should disable the edition */
      for(int i = 0; i < 3; i++)
      {
         position[i]->disable();
         position[i]->setCaption("");
         rotation[i]->disable();
         rotation[i]->setCaption("");
         scale[i]->disable();
         scale[i]->setCaption("");
      }
   }
}

/************************************************************************
 *                             updateTexts                              *
 ************************************************************************/
void TransformWindow::updateTexts(DNT::Thing* thing)
{
   for(int i = 0; i < 3; i++)
   {
      position[i]->setCaption(Ogre::StringConverter::toString(
               thing->getModel()->getPosition()[i]));
      scale[i]->setCaption(Ogre::StringConverter::toString(
               thing->getModel()->getScale()[i]));
   }
   rotation[0]->setCaption(Ogre::StringConverter::toString(
               thing->getModel()->getPitch()));
   rotation[1]->setCaption(Ogre::StringConverter::toString(
               thing->getModel()->getYaw()));
   rotation[2]->setCaption(Ogre::StringConverter::toString(
               thing->getModel()->getRoll()));
}

/************************************************************************
 *                               update                                 *
 ************************************************************************/
void TransformWindow::update(PositionEditor* positionEditor)
{
   if(window)
   {
      /* Check if enabled or disabled */
      setEnabled(positionEditor->getSelectedThing() != NULL);
      if(positionEditor->getSelectedThing())
      {
         /* Let's update current transforms */
         updateTexts(positionEditor->getSelectedThing());
      }
   }
}

/************************************************************************
 *                                parse                                 *
 ************************************************************************/
float TransformWindow::parse(Farso::TextEntry* textEntry, float previousValue)
{
   return Ogre::StringConverter::parseReal(textEntry->getCaption(), 
         previousValue);
}

/************************************************************************
 *                             checkEvents                              *
 ************************************************************************/
bool TransformWindow::checkEvents(PositionEditor* positionEditor)
{
   if((window) && (window->isActive()) && (positionEditor->getSelectedThing()))
   {
      DNT::Thing* thing = positionEditor->getSelectedThing();
      Farso::Event event = Farso::Controller::getLastEvent();
      if(event.getType() == Farso::EVENT_TEXTENTRY_EDITION_DONE)
      {
         /* Check positions */
         if(event.getWidget() == position[0])
         {
            thing->getModel()->setPosition(
                  parse(position[0], thing->getModel()->getPosition().x),
                  thing->getModel()->getPosition().y,
                  thing->getModel()->getPosition().z);
            positionEditor->updateAxisPosition();
         }
         else if(event.getWidget() == position[1])
         {
            thing->getModel()->setPosition(
                  thing->getModel()->getPosition().x,
                  parse(position[1], thing->getModel()->getPosition().y),
                  thing->getModel()->getPosition().z);
            positionEditor->updateAxisPosition();
         }
         else if(event.getWidget() == position[2])
         {
            thing->getModel()->setPosition(
                  thing->getModel()->getPosition().x,
                  thing->getModel()->getPosition().y,
                  parse(position[2], thing->getModel()->getPosition().z));
            positionEditor->updateAxisPosition();
         }
         /* Check rotations */
         else if(event.getWidget() == rotation[0])
         {
            thing->getModel()->setOrientation(
                  parse(rotation[0], thing->getModel()->getPitch()),
                  thing->getModel()->getYaw(),
                  thing->getModel()->getRoll());
            positionEditor->updateAxisPosition();
         }
         else if(event.getWidget() == rotation[1])
         {
            thing->getModel()->setOrientation(
                  thing->getModel()->getPitch(),
                  parse(rotation[1], thing->getModel()->getYaw()),
                  thing->getModel()->getRoll());
         }
         else if(event.getWidget() == rotation[2])
         {
            thing->getModel()->setOrientation(
                  thing->getModel()->getPitch(),
                  thing->getModel()->getYaw(),
                  parse(rotation[2], thing->getModel()->getRoll()));
         }
         /* Check scale */
         else if(event.getWidget() == scale[0])
         {
            thing->getModel()->setScale(
                  parse(scale[0], thing->getModel()->getScale().x),
                  thing->getModel()->getScale().y,
                  thing->getModel()->getScale().z);
         }
         else if(event.getWidget() == scale[1])
         {
            thing->getModel()->setScale(
                  thing->getModel()->getScale().x,
                  parse(scale[1], thing->getModel()->getScale().y),
                  thing->getModel()->getScale().z);
         }
         else if(event.getWidget() == scale[2])
         {
            thing->getModel()->setScale(
                  thing->getModel()->getScale().x,
                  thing->getModel()->getScale().y,
                  parse(scale[2], thing->getModel()->getScale().z));
         }
      }
   }

   return false;
}


