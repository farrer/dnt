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

#include "vector3textentry.h"
#include <OGRE/OgreStringConverter.h>
#include <assert.h>
using namespace DNTMapEditor;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
Vector3TextEntry::Vector3TextEntry(int x, int y, Vector3TextEntryType type, 
                                   Kobold::String label, Farso::Widget* parent)
                 :Widget(Widget::WIDGET_TYPE_USER_CREATED, 
                         x, y, 310, 22, parent)
{
   assert(parent != NULL);
   body.set(0, 0, parent->getBody().getWidth() - 1,
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());

   /* Define each component label */
   Kobold::String labelX = "x";
   Kobold::String labelY = "y";
   Kobold::String labelZ = "z";
   if(type == TYPE_RGB)
   {
      labelX = "r";
      labelY = "g";
      labelZ = "b";
   }

   /* Add the children */
   new Farso::Label(x, y, 60, 21, label, this);
   (new Farso::Label(x+60, y, 21, 21, labelX, this))->setFontColor(
         Farso::Color(255, 0, 0, 255));
   (new Farso::Label(x+145, y, 21, 21, labelY, this))->setFontColor(
         Farso::Color(0, 255, 0, 255));
   (new Farso::Label(x+230, y, 21, 21, labelZ, this))->setFontColor(
         Farso::Color(0, 0, 255, 255));
   textEntry[0] = new Farso::TextEntry(x+74, y, 60, 21, this);
   textEntry[1] = new Farso::TextEntry(x+159, y, 60, 21, this);
   textEntry[2] = new Farso::TextEntry(x+244, y, 60, 21, this);

   value = Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

/************************************************************************
 *                             Destructor                               *
 ************************************************************************/
Vector3TextEntry::~Vector3TextEntry()
{
}

/************************************************************************
 *                                setValue                              *
 ************************************************************************/
void Vector3TextEntry::setValue(const float x, const float y, const float z)
{
   textEntry[0]->setCaption(Ogre::StringConverter::toString(x));
   textEntry[1]->setCaption(Ogre::StringConverter::toString(y));
   textEntry[2]->setCaption(Ogre::StringConverter::toString(z));
   value = Ogre::Vector3(x, y, z);
}

/************************************************************************
 *                                setValue                              *
 ************************************************************************/
void Vector3TextEntry::setValue(const Ogre::Vector3& val)
{
   setValue(val.x, val.y, val.z);
}

/************************************************************************
 *                                setValue                              *
 ************************************************************************/
void Vector3TextEntry::setValue(const Ogre::ColourValue& val)
{
   setValue(val.r, val.g, val.b);
}

/************************************************************************
 *                               doDraw                                 *
 ************************************************************************/
void Vector3TextEntry::doDraw(const Farso::Rect& pBody)
{
   Farso::Rect parentBody = getParent()->getBody();
   if((parentBody.getWidth() != body.getWidth()) ||
      (parentBody.getHeight() != body.getHeight()))
   {
      /* Parent's body changed, must redefine our component's size */
      body.set(0, 0, parentBody.getWidth() - 1, parentBody.getHeight() - 1);
      setSize(body.getWidth(), body.getHeight());
   }
}

/************************************************************************
 *                               doTreat                                *
 ************************************************************************/
bool Vector3TextEntry::doTreat(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, int mrX, int mrY)
{
   return false;
}

/************************************************************************
 *                                parse                                 *
 ************************************************************************/
float Vector3TextEntry::parse(Farso::TextEntry* textEntry, float previousValue)
{
   return Ogre::StringConverter::parseReal(textEntry->getCaption(),
         previousValue);
}

/************************************************************************
 *                          doAfterChildTreat                           *
 ************************************************************************/
void Vector3TextEntry::doAfterChildTreat()
{
   Farso::Event event = Farso::Controller::getLastEvent();
   if(event.getType() == Farso::EVENT_TEXTENTRY_EDITION_DONE)
   {
      if(event.getWidget() == textEntry[0])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         value.x = parse(textEntry[0], value.x);
      }
      else if(event.getWidget() == textEntry[1])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         value.y = parse(textEntry[1], value.y);
      }
      else if(event.getWidget() == textEntry[2])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         value.z = parse(textEntry[2], value.z);
      }
   }
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void Vector3TextEntry::setDirty()
{
   setDirtyWithParent();
}

/***********************************************************************
 *                               enable                                *
 ***********************************************************************/
void Vector3TextEntry::enable()
{
   Farso::Widget::enable();
   textEntry[0]->enable();
   textEntry[1]->enable();
   textEntry[2]->enable();
}

/***********************************************************************
 *                              disable                                *
 ***********************************************************************/
void Vector3TextEntry::disable()
{
   Farso::Widget::disable();
   textEntry[0]->disable();
   textEntry[1]->disable();
   textEntry[2]->disable();
}

