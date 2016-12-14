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

   /* Windows for Map's Metadata edit */
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

      private:
         /*! Open a window to select music file */
         void openSelectMusicWindow();

         /* Map Metadata widgets */
         Farso::Window* metadataWindow;
         Farso::TextEntry* mapNameTextEntry;
         Farso::TextEntry* mapMusicTextEntry;
         Farso::Button* buttonSelectMusic;
         Farso::Button* buttonApplyMetadata;

         /* select music widgets */
         Farso::Window* selectWindow;
         Farso::FileSelector* musicSelector;


   };

}

#endif
