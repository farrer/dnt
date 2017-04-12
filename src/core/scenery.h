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

#ifndef _dnt_scenery_h
#define _dnt_scenery_h

#include "dntconfig.h"
#include "object.h"

namespace DNT
{

/*! The Scenery is an static not living Thing. Just compose the ambience,
 * but usually couldn't do anything more to it than that. */
class Scenery : public Object
{
   public:
      /*! Contructor */
      Scenery();
      /*! Destructor */
      ~Scenery();

      /*! \return false. Scenery is static. */
      const bool canInteract() const { return false; };

      /*! Interact with Scenery: nothing to do. */
      Object::InteractResult interact(Character* actor);

   protected:

      /*! Parse scenery specific values from file */
      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);
      /*! write scenery specific values to file */
      bool doObjectSpecializationSave(std::ofstream& file);

};

}

#endif

