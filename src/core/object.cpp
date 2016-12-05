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

#include "object.h"
#include <kobold/log.h>

using namespace DNT;

#define OBJECT_KEY_IMAGE     "image"

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Object::Object()
{
   image = NULL;
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Object::~Object()
{
   if(image)
   {
      delete image;
   }
}

/**************************************************************************
 *                                getImage                                *
 **************************************************************************/
Farso::Surface* Object::getImage()
{
   return image;
}

/**************************************************************************
 *                             doSpecificParse                            *
 **************************************************************************/
bool Object::doSpecificParse(Ogre::String key, Ogre::String value)
{
   if(key == OBJECT_KEY_IMAGE)
   {
      if(!image)
      {
         image = Farso::Controller::loadImageToSurface(value);
         if(!image)
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't load object's image '%s'", value.c_str());
            return false;
         }
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                 "Warning: object's image redefinition '%s' ignored", 
                 value.c_str());
      }
      
      return true;
   }
   else
   {
      return doObjectSpecializationParse(key, value);
   }
}

/**************************************************************************
 *                             doSpecificSave                             *
 **************************************************************************/
bool Object::doSpecificSave(std::ofstream& file)
{
   if(image)
   {
      file << OBJECT_KEY_IMAGE << " = " << image->getTextureName() 
           << std::endl;
   }
   
   return doObjectSpecializationSave(file);
}

