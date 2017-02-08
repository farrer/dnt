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
#include "metadatagui.h"
#include "nodeswindow.h"
#include "positioneditor.h"
#include "transformwindow.h"

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

         /*! Check current Farso::Events for main gui 
          * \return true if should quit the editor. */
         bool checkEvents(PositionEditor* positionEditor);

         /*! Update any needed gui with current values */
         void update(PositionEditor* positionEditor);

         /*! \return if light is enabled or not */
         bool isLightEnabled();
         
         /*! define light based on current toggle state */
         void setLight();

         /*! Show the progress bar, initially with value 0 */
         void showProgressBar();

         /*! Set progress bar value [0,100] */
         void setProgressBar(int value);

         /*! Hide the progress bar */
         void hideProgressBar();

         /*! Hide the menu bar */
         void hideTopBar();
         /*! Display the menu bar */
         void showTopBar();

         /*! Close all map relative opened windows.
          * \note usually called when a new map will be loaded / created */
         void closeMapRelatedWindows(PositionEditor* positionEditor);

         /*! \return nodes window pointer */
         NodesWindow* getNodesWindow() { return &nodesWindow; };

         /*! Duplicate current selected node, if any. */
         void duplicateSelection(PositionEditor* positionEditor);
         /*! Remove current selection, if any, from both editor and map */
         void removeSelection(PositionEditor* positionEditor);
         /*! Unselect current selection, if any. */
         void unselect(PositionEditor* positionEditor);

      private:

         void openLoadOrSaveWindow(bool loading);
         void openNewMapWindow();
         void toggleMenuStatus();

         void enableEditItems();
         void disableEditItems();

         /*! Set current camera to be within map area */
         void setCameraOnMap();

         Farso::ProgressBar* progressBar; /**< A multiple-use progress bar */

         Farso::Container* cont; /**< Top bar menu container */

         /* File button and Menu */
         Farso::Button* fileButton;
         Farso::Menu* fileMenu;
         Farso::Menu::MenuItem* menuItemNewIndoor;
         Farso::Menu::MenuItem* menuItemNewOutdoor;
         Farso::Menu::MenuItem* menuItemLoad;
         Farso::Menu::MenuItem* menuItemSave;
         Farso::Menu::MenuItem* menuItemSaveAs;
         Farso::Menu::MenuItem* menuItemExit;

         /*! Edit Menu */
         Farso::Button* editButton;
         Farso::Menu* editMenu;
         Farso::Menu::MenuItem* menuItemUnselect;
         Farso::Menu::MenuItem* menuItemDuplicate;
         Farso::Menu::MenuItem* menuItemRemove;

         /* Dialogs button and Menu */
         Farso::Button* dialogsButton;
         Farso::Menu* dialogsMenu;
         Farso::Menu::MenuItem* menuItemMetadata;
         Farso::Menu::MenuItem* menuItemNodes;
         Farso::Menu::MenuItem* menuItemTransform;

         /* View button and Menu */
         Farso::Button* viewButton;
         Farso::Menu* viewMenu;
         Farso::Menu::MenuItem* menuItemToggleLight;
         bool lightEnabled;
         Farso::Menu::MenuItem* menuItemShowConnections;

         /* Load / Save window widgets */
         Farso::Window* loadSaveWindow;
         Farso::FileSelector* loadSaveSelector;

         /* New Map Window widgets */
         Farso::Window* newMapWindow;
         Farso::Spin* spinNewMapSizeX;
         Farso::Spin* spinNewMapSizeZ;
         Farso::Button* buttonNewMapCreate;

         MetadataGui metadataGui; /**< Metadata editor gui */
         NodesWindow nodesWindow; /**< Nodes selector window */
         TransformWindow transformWindow; /**< Node transform window */
   };


}

#endif


