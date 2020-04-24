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

#ifndef _dnt_map_editor_info_window_h
#define _dnt_map_editor_info_window_h

#include <farso/controller.h>
#include "../../rules/thing.h"

namespace DNTMapEditor
{
   /*! A window for defining specific Thing's attributes (dialog file,
    *  locked state for doors, psychoState for NPCs, etc). */
   class InfoWindow
   {
      public:
         /*! Constructor */
         InfoWindow();
         /*! Destructor */
         ~InfoWindow();

         /*! Open the window, if not already opened */
         void open();
         /*! Close the window, if opened */
         void close();

         /*! Update the window, with values from current selected */
         void update();

         /*! Check events happened on the InfoWindow */
         void checkEvents();

      private:
         Farso::TextEntry* dialogFile;  /**< Thing's dialog, if any */
         Farso::CheckBox* doorLocked;  /**< Only door: if locked or not */
         Farso::Spin* difficultValue; /**< Challenge value */
         Farso::Spin* hardnessValue; /**< Challenge value to force it up */
         Farso::ComboBox* psychoCombo; /**< If a NPC, its psycho state */

         Farso::Window* window; /**< The window itself */

         DNT::Thing* selected; /**< Thing currently displayed here */
   };
}

#endif

