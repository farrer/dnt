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

#include "ruledef.h"
using namespace DNT;

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
RuleDefinition::RuleDefinition(int id, Kobold::String strId)
{
   this->image = NULL;
   setId(id, strId);
}
RuleDefinition::RuleDefinition()
{
   this->image = NULL;
   setId(0, "");
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
RuleDefinition::~RuleDefinition()
{
   if(image != NULL)
   {
      delete image;
   }
}

/******************************************************************
 *                               setId                            *
 ******************************************************************/
void RuleDefinition::setId(int id, Kobold::String strId)
{
   this->intID = id;
   this->strID = strId;
}

/******************************************************************
 *                               getId                            *
 ******************************************************************/
int RuleDefinition::getId()
{
   return intID;
}

/******************************************************************
 *                            getStringId                         *
 ******************************************************************/
Kobold::String RuleDefinition::getStringId()
{
   return strID;
}

/******************************************************************
 *                            loadImage                           *
 ******************************************************************/
void RuleDefinition::loadImage(Kobold::String imageFilename)
{
   assert(image == NULL);
   image = Farso::Controller::loadImageToSurface(imageFilename);
}

/******************************************************************
 *                             getImage                           *
 ******************************************************************/
Farso::Surface* RuleDefinition::getImage()
{
   return image;
}

/******************************************************************
 *                              setName                           *
 ******************************************************************/
void RuleDefinition::setName(Kobold::String name)
{
   this->name = name;
}

/******************************************************************
 *                              getName                           *
 ******************************************************************/
Kobold::String RuleDefinition::getName()
{
   return name;
}

/******************************************************************
 *                          setDescription                        *
 ******************************************************************/
void RuleDefinition::setDescription(Kobold::String desc)
{
   this->description = desc;
}

/******************************************************************
 *                          getDescription                        *
 ******************************************************************/
Kobold::String RuleDefinition::getDescription()
{
   return description;
}



