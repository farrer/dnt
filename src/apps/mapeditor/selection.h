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

#ifndef _dnt_map_editor_selection_h
#define _dnt_map_editor_selection_h

#include <OGRE/OgreSceneManager.h>
#include <goblin/model3d.h>
#include "../../rules/thing.h"
#include "../../map/light.h"

namespace DNTMapEditor 
{

   /*! Quick reference for current selection on map editor */
   class Selection
   {
      public:
         /*! Clear any selection */
         static void clear();

         /*! Select a Thing for position edition */
         static void selectThing(DNT::Thing* thing);

         /*! \return current selected thing pointer, if any */
         static DNT::Thing* getSelectedThing() { return selectedThing; };

         /*! Select a Light for position edition
          * \note must not be a directional (ie: without position) light. */
         static void selectLight(DNT::LightInfo* light);

         /*! \return current selected light */
         static DNT::LightInfo* getSelectedLight() { return selectedLight; };

         /*! \return if a Thing, Wall or Light is selected */
         static bool hasSelection();

      private:

         static DNT::Thing* selectedThing;  /**< Thing selected */
         static DNT::LightInfo* selectedLight; /**< Light selected */
   };

}


#endif

