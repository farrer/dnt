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

#ifndef _dnt_ruledef_h
#define _dnt_ruledef_h

#include "dntconfig.h"

#include <kobold/kstring.h>
#include <farso/controller.h>

namespace DNT
{

/*! Abstract class to each rule definition (RuleDefinition, Skill, etc). */
class RuleDefinition
{
   public:
      /*! Constructor */
      RuleDefinition(int id, Kobold::String strId);
      RuleDefinition();
      /*! Destructor */
      ~RuleDefinition();

      /*! Define identifier. 
       * \param id new integer identifier 
       * \param strId new string identifier */
      void setId(int id, Kobold::String strId);
      /*! \return RuleDefinition's integer identifier */
      int getId();
      /*! \return RuleDefinition's String identifier */
      Kobold::String getStringId();

      /*! Load RuleDefinition's image.
       * \param imageFilename filename of the image to load 
       * \note should be only called once. */
      void loadImage(Kobold::String imageFilename);
      /*! \return RuleDefinition's image */
      Farso::Surface* getImage();

      /*! Set RuleDefinition's name.
       * \param name name to set */
      void setName(Kobold::String name);
      /*! \return RuleDefinition's name */
      Kobold::String getName();

      /*! Set RuleDefinition's Description.
       * \param desc desctiption to set */
      void setDescription(Kobold::String desc);
      /*! \return RuleDefinition's description */
      Kobold::String getDescription();

   private:

      int intID;              /**< Integer Identifier of the definition */
      Kobold::String strID;   /**< String Identifier of the definition */
      Farso::Surface* image;  /**< Image representing the definition */

      Kobold::String name;        /**< Name of the definition */
      Kobold::String description; /**< Description of the definition */
};

}

#endif

