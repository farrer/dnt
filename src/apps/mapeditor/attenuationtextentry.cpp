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

#include "attenuationtextentry.h"
#include <OGRE/OgreStringConverter.h>
#include <assert.h>
using namespace DNTMapEditor;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
AttenuationTextEntry::AttenuationTextEntry(int x, int y, Farso::Widget* parent)
                 :Widget(Widget::WIDGET_TYPE_USER_CREATED, 
                         x, y, 310, 44, parent)
{
   assert(parent != NULL);
   body.set(0, 0, parent->getBody().getWidth() - 1,
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());

   /* Add the labels */
   new Farso::Label(x, y+11, 75, 21, "Attenuation", this);
   new Farso::Label(x+70, y, 60, 21, "Range", this);
   new Farso::Label(x+190, y, 60, 21, "Linear", this);
   new Farso::Label(x+70, y+22, 60, 21, "Constant", this);
   new Farso::Label(x+190, y+22, 60, 21, "Quadric", this);

   /* Add the text entries */
   textEntry[0] = new Farso::TextEntry(x+129, y, 60, 21, this);
   textEntry[1] = new Farso::TextEntry(x+244, y, 60, 21, this);
   textEntry[2] = new Farso::TextEntry(x+129, y+22, 60, 21, this);
   textEntry[3] = new Farso::TextEntry(x+244, y+22, 60, 21, this);

   this->range = 0.0f;
   this->constant = 0.0f;
   this->linear = 0.0f;
   this->quadric = 0.0f;
}

/************************************************************************
 *                             Destructor                               *
 ************************************************************************/
AttenuationTextEntry::~AttenuationTextEntry()
{
}

/************************************************************************
 *                                setValue                              *
 ************************************************************************/
void AttenuationTextEntry::setValue(const Ogre::Real range, 
      const Ogre::Real constant, const Ogre::Real linear, 
      const Ogre::Real quadric)

{
   textEntry[0]->setCaption(Ogre::StringConverter::toString(range));
   textEntry[1]->setCaption(Ogre::StringConverter::toString(constant));
   textEntry[2]->setCaption(Ogre::StringConverter::toString(linear));
   textEntry[3]->setCaption(Ogre::StringConverter::toString(quadric));
   this->range = range;
   this->constant = constant;
   this->linear = linear;
   this->quadric = quadric;
}

/************************************************************************
 *                               doDraw                                 *
 ************************************************************************/
void AttenuationTextEntry::doDraw(Farso::Rect pBody)
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
bool AttenuationTextEntry::doTreat(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, int mrX, int mrY)
{
   return false;
}

/************************************************************************
 *                                parse                                 *
 ************************************************************************/
float AttenuationTextEntry::parse(Farso::TextEntry* textEntry, float previousValue)
{
   return Ogre::StringConverter::parseReal(textEntry->getCaption(),
         previousValue);
}

/************************************************************************
 *                          doAfterChildTreat                           *
 ************************************************************************/
void AttenuationTextEntry::doAfterChildTreat()
{
   Farso::Event event = Farso::Controller::getLastEvent();
   if(event.getType() == Farso::EVENT_TEXTENTRY_EDITION_DONE)
   {
      if(event.getWidget() == textEntry[0])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         range = parse(textEntry[0], range);
      }
      else if(event.getWidget() == textEntry[1])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         constant = parse(textEntry[1], constant);
      }
      else if(event.getWidget() == textEntry[2])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         linear = parse(textEntry[2], linear);
      }
      else if(event.getWidget() == textEntry[3])
      {
         Farso::Controller::setEvent(this, Farso::EVENT_USER_DEFINED);
         quadric = parse(textEntry[3], quadric);
      }
   }
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void AttenuationTextEntry::setDirty()
{
   setDirtyWithParent();
}

/***********************************************************************
 *                               enable                                *
 ***********************************************************************/
void AttenuationTextEntry::enable()
{
   Farso::Widget::enable();
   textEntry[0]->enable();
   textEntry[1]->enable();
   textEntry[2]->enable();
   textEntry[3]->enable();
}

/***********************************************************************
 *                              disable                                *
 ***********************************************************************/
void AttenuationTextEntry::disable()
{
   Farso::Widget::disable();
   textEntry[0]->disable();
   textEntry[1]->disable();
   textEntry[2]->disable();
   textEntry[3]->disable();
}

