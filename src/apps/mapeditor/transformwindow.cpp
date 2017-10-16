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
      window = new Farso::Window(320, 90, "Transform");
      window->setExternPointer(&window);

      /* Define each of our transforms */
      position = new Vector3TextEntry(0, 0, Vector3TextEntry::TYPE_AXIS,
            "Position", window);
      rotation = new Vector3TextEntry(0, 22, Vector3TextEntry::TYPE_AXIS,
            "Rotation", window);
      scale = new Vector3TextEntry(0, 44, Vector3TextEntry::TYPE_AXIS,
            "Scale", window);

      /* Open and set its position */
      window->open();
      window->setPosition(Farso::Controller::getWidth() - 320, 225);
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
   if((enable) && (!scale->isAvailable()))
   {
      /* Should enable to edit */
      position->enable();
      rotation->enable();
      scale->enable();
   }
   else if((!enable) && (scale->isAvailable()))
   {
      /* Should disable the edition */
      position->disable();
      rotation->disable();
      scale->disable();
   }
}

/************************************************************************
 *                             updateTexts                              *
 ************************************************************************/
void TransformWindow::updateTexts(DNT::Thing* thing)
{
   position->setValue(thing->getPosition());
   rotation->setValue(thing->getPitch(), 
                      thing->getYaw(),
                      thing->getRoll());
   scale->setValue(thing->getScale());
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
 *                             checkEvents                              *
 ************************************************************************/
bool TransformWindow::checkEvents(PositionEditor* positionEditor)
{
   if((window) && (window->isActive()) && (positionEditor->getSelectedThing()))
   {
      DNT::Thing* thing = positionEditor->getSelectedThing();
      Farso::Event event = Farso::Controller::getLastEvent();
      if(event.getType() == Farso::EVENT_USER_DEFINED)
      {
         /* Check positions */
         if(event.getWidget() == position)
         {
            thing->setPositionNow(position->getValue());
            positionEditor->updateAxisPosition();
         }
         /* Check rotations */
         else if(event.getWidget() == rotation)
         {
            const Ogre::Vector3& rot = rotation->getValue();
            thing->clearOrientation();
            thing->setOrientationNow(rot);
            positionEditor->updateAxisPosition();
         }
         /* Check scale */
         else if(event.getWidget() == scale)
         {
            const Ogre::Vector3& sc = scale->getValue();
            thing->setScaleNow(sc);
         }
      }
   }

   return false;
}


