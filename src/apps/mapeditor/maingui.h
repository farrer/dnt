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

#ifndef _dnt_mapeditor_main_gui_h
#define _dnt_mapeditor_main_gui_h

#include <farso/controller.h>

namespace DNTMapEditor
{

   /*! This class implements the main menu and its uses for DNTMapEditor */
   class MainGui
   {
      public:
         /*! Constructor */
         MainGui();
         /*! Destructor */
         ~MainGui();

         /*! Check current Farso::Events for mai gui 
          * \return true if should quit the editor. */
         bool checkEvents();

         /*! \return if light is enabled or not */
         bool isLightEnabled();

      private:

         void openLoadOrSaveWindow(bool loading);
         void openNewMapWindow();
         void openMetadataWindow();
         void toggleMenuStatus();
         void setLight();

         /* File button and Menu */
         Farso::Button* fileButton;
         Farso::Menu* fileMenu;
         Farso::Menu::MenuItem* menuItemNewIndoor;
         Farso::Menu::MenuItem* menuItemNewOutdoor;
         Farso::Menu::MenuItem* menuItemLoad;
         Farso::Menu::MenuItem* menuItemSave;
         Farso::Menu::MenuItem* menuItemSaveAs;
         Farso::Menu::MenuItem* menuItemExit;

         /* Dialogs button and Menu */
         Farso::Button* dialogsButton;
         Farso::Menu* dialogsMenu;
         Farso::Menu::MenuItem* menuItemLights;
         Farso::Menu::MenuItem* menuItemSounds;
         Farso::Menu::MenuItem* menuItemTerrain;
         Farso::Menu::MenuItem* menuItemWall;
         Farso::Menu::MenuItem* menuItemTileWall;
         Farso::Menu::MenuItem* menuItemObjects;
         Farso::Menu::MenuItem* menuItemCharacters;
         Farso::Menu::MenuItem* menuItemPortal;
         Farso::Menu::MenuItem* menuItemParticles;

         /* View button and Menu */
         Farso::Button* viewButton;
         Farso::Menu* viewMenu;
         Farso::Menu::MenuItem* menuItemToggleLight;
         bool lightEnabled;
         Farso::Menu::MenuItem* menuItemShowConnections;

         /* Map button and Menu */
         Farso::Button* mapButton;
         Farso::Menu* mapMenu;
         Farso::Menu::MenuItem* menuItemMetadata;

         /* Load / Save window widgets */
         Farso::Window* loadSaveWindow;
         Farso::FileSelector* loadSaveSelector;

         /* New Map Window widgets */
         Farso::Window* newMapWindow;
         Farso::Spin* spinNewMapSizeX;
         Farso::Spin* spinNewMapSizeZ;
         Farso::Button* buttonNewMapCreate;

         /* Map Metadata widgets */
         Farso::Window* metadataWindow;
         Farso::TextEntry* mapNameTextEntry;
         Farso::TextEntry* mapMusicTextEntry;
         Farso::Button* buttonSelectMusic;
         Farso::Button* buttonApplyMetadata;

   };


}

#endif


