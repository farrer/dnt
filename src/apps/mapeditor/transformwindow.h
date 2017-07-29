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

#ifndef _dnt_map_editor_transform_window_h
#define _dnt_map_editor_transform_window_h

#include <farso/controller.h>
#include <map>
#include "positioneditor.h"

namespace DNTMapEditor
{
   /*! A window to direct define node's transform variables (ie: position,
    * rotation and scale). */
   class TransformWindow
   {
      public:
         /*! Constructor */
         TransformWindow();
         /*! Destructor */
         ~TransformWindow();

         /*! Open window (closing it if already opened before 
          *  opening again). */
         void open();

         /*! Close the window, if opened */
         void close();

         /*! \return if transform window is opened */
         const bool isOpened() const { return window != NULL; };

         /*! Check current Farso::Events for the window 
          * \return true if treated some event. */
         bool checkEvents(PositionEditor* positionEditor);

         /*! Update window with the values from current selected */
         void update(PositionEditor* positionEditor);

      private:
         /*! Set enable status.
          * \param enable if enabled or not */
         void setEnabled(bool enable);

         /*! Update current transform texts with ones defined by the thing */
         void updateTexts(DNT::Thing* thing);

         /*! Parse value from text entry. on error, use previous value */
         float parse(Farso::TextEntry* textEntry, float previousValue);

         Farso::Window* window; /**< The window */
         Farso::TextEntry* position[3]; /**< Each position to edit */
         Farso::TextEntry* rotation[3]; /**< Each rotation to edit */
         Farso::TextEntry* scale[3]; /**< Each scale to edit */
   };

}

#endif

