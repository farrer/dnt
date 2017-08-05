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

#ifndef _dnt_map_editor_metadata_gui_h
#define _dnt_map_editor_metadata_gui_h

#include <farso/controller.h>

namespace DNTMapEditor
{

   /*! Window for Map's Metadata edit */
   class MetadataGui
   {
      public:
         /*! Constructor */
         MetadataGui();
         /*! Destructor */
         ~MetadataGui();

         /*! Open metadata window (closing it if already opened before 
          *  opening again). */
         void open();

         /*! Close the all metadata windows if some are opened */
         void close();

         /*! Check current Farso::Events for mai gui 
          * \return true if should quit the editor. */
         bool checkEvents();

         /*! \return if metadata window is opened */
         const bool isOpened() const { return metadataWindow != NULL; };

      private:
         /*! Open a window to select a file
          * \param music true to selec a music (.ogg) file. 
          *              false for script scelection (*.as). */
         void openSelectWindow(bool music);

         /* Map Metadata widgets */
         Farso::Window* metadataWindow; /**< Its window */
         Farso::TextEntry* mapNameTextEntry; /**< Text for map name */
         Farso::TextEntry* mapMusicTextEntry; /**< Text for map music */
         Farso::Button* buttonSelectMusic; /**< Button to select music */
         Farso::TextEntry* mapScriptTextEntry; /**< Text for map script */
         Farso::Button* buttonSelectScript; /**< Button to select script */
         Farso::Button* buttonApplyMetadata; /**< Button to apply */

         /* select music/script widgets */
         Farso::Window* selectWindow; /**< The select window */
         Farso::FileSelector* fileSelector; /**< Its selector */
         bool selectMusic; /**< True is selecting music, false script */


   };

}

#endif

