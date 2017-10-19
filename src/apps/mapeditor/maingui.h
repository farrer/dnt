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
#include "lightwindow.h"
#include "../../core/module.h"

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

         /*! Open/Close the Node Window */
         void toggleNodeWindow();
         /*! Open/Close the metadata window */
         void toggleMetadataWindow();
         /*! Open/Close the transform window */
         void toggleTransformWindow();
         /*! Open/Close the light window */
         void toggleLightWindow();

      private:

         /*! Open the dialog window to load or save.
          * \param loading true will open a load dialog, false a save one 
          * \param map true if loading/saving a map, false if a module. */
         void openLoadOrSaveWindow(bool loading, bool map);
         /*! Open the New Map window */
         void openNewMapWindow();
         /*! Toggle menu availability status */
         void toggleMenuStatus();

         /*! Enable edit menu items */
         void enableEditItems();
         /*! Disable edit menu items */
         void disableEditItems();

         /*! Set current camera to be within map area */
         void setCameraOnMap();

         /*! Populate with all datablocks, for selection */
         void populateWithDatablocks();

         Farso::ProgressBar* progressBar; /**< A multiple-use progress bar */

         Farso::Container* cont; /**< Top bar menu container */

         /* File button and Menu */
         Farso::Button* fileButton; /**< The file button */
         Farso::Menu* fileMenu;     /**< The file menu */
         Farso::Menu::MenuItem* menuItemOpenModule; /**< Open Module */
         Farso::Menu::MenuItem* menuItemNewIndoor; /**< New Infoor map */
         Farso::Menu::MenuItem* menuItemNewOutdoor; /**< New outdoor map */
         Farso::Menu::MenuItem* menuItemLoad; /**< Load map */
         Farso::Menu::MenuItem* menuItemSave; /**< Save map */
         Farso::Menu::MenuItem* menuItemSaveAs; /**< Save map as */
         Farso::Menu::MenuItem* menuItemExit; /**< Exit editor */

         /*! Edit Menu */
         Farso::Button* editButton; /**< Edit button */
         Farso::Menu* editMenu;     /**< Edit menu */
         Farso::Menu::MenuItem* menuItemUnselect; /**< Unselect item */
         Farso::Menu::MenuItem* menuItemDuplicate; /**< Duplicate item */
         Farso::Menu::MenuItem* menuItemRemove; /**< Remove item */

         /* Dialogs button and Menu */
         Farso::Button* dialogsButton; /**< Dialogs button */
         Farso::Menu* dialogsMenu; /**< Dialogs menu */
         Farso::Menu::MenuItem* menuItemMetadata; /**< Metadata item */
         Farso::Menu::MenuItem* menuItemNodes; /**< Nodes item */
         Farso::Menu::MenuItem* menuItemTransform; /**< Transform item */
         Farso::Menu::MenuItem* menuItemLight; /**< Light item */

         /* View button and Menu */
         Farso::Button* viewButton; /**< View button */
         Farso::Menu* viewMenu; /**< View menu */
         Farso::Menu::MenuItem* menuItemToggleLight; /**< Toggle light */
         bool lightEnabled; /**< If light is enabled or not */
         Farso::Menu::MenuItem* menuItemShowConnections; /**< Show connects */

         /* Load / Save window widgets */
         Farso::Window* loadSaveWindow; /**< Window to load or save */
         Farso::FileSelector* loadSaveSelector; /**< The selector */

         /* New Map Window widgets */
         Farso::Window* newMapWindow; /**< New Map window */
         Farso::Spin* spinNewMapSizeX; /**< Map size X */
         Farso::Spin* spinNewMapSizeZ; /**< Map size Z */
         Farso::Button* buttonNewMapCreate; /**< Button create window */

         MetadataGui metadataGui; /**< Metadata editor gui */
         NodesWindow nodesWindow; /**< Nodes selector window */
         TransformWindow transformWindow; /**< Node transform window */
         LightWindow lightWindow; /**< Light edit window */
         DNT::Module* module; /**< Module to use */
         bool loadingModule; /**< If loading module */
   };


}

#endif


