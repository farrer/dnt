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

#include "maingui.h"
#include "../../core/game.h"
using namespace DNTMapEditor;

/***********************************************************************
 *                               MainGui                               *
 ***********************************************************************/
MainGui::MainGui()
{
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_TOP_LEFT,0, 0, 1024, 23, NULL);
   cont->setFilled();
   
   /* Create File button and its menu */
   fileButton = new Farso::Button(1, 1, 80, 21, "File", cont);
   fileMenu = new Farso::Menu(100);
   fileMenu->beginCreate();
   menuItemNew = fileMenu->insertItem("New");
   fileMenu->insertSeparator();
   menuItemLoad = fileMenu->insertItem("Load");
   menuItemSave = fileMenu->insertItem("Save");
   menuItemSave->disable();
   menuItemSaveAs = fileMenu->insertItem("Save as");
   menuItemSaveAs->disable();
   fileMenu->insertSeparator();
   menuItemExit = fileMenu->insertItem("Exit");
   fileMenu->endCreate();
   fileButton->setMenu(fileMenu);

   /* Nullify Load/Save window things */
   loadSaveWindow = NULL;
   loadSaveSelector = NULL;

   /* Create view button and menu */
   viewButton = new Farso::Button(81, 1, 80, 21, "View", cont);
   viewMenu = new Farso::Menu();
   viewMenu->beginCreate();
   menuItemToggleLight = viewMenu->insertItem("Disable light");
   lightEnabled = true;
   viewMenu->insertSeparator();
   viewMenu->insertItem("Show connections");
   viewMenu->endCreate();
   viewButton->setMenu(viewMenu);

   /* Create dialogs button and menu */
   dialogsButton = new Farso::Button(161, 1, 80, 21, "Dialogs", cont);
   dialogsMenu = new Farso::Menu(120);
   dialogsMenu->beginCreate();
   dialogsMenu->insertItem("Lights");
   dialogsMenu->insertSeparator();
   dialogsMenu->insertItem("Terrain");
   dialogsMenu->insertItem("Wall");
   dialogsMenu->insertItem("Tile Wall");
   dialogsMenu->insertSeparator();
   dialogsMenu->insertItem("Objects");
   dialogsMenu->insertItem("Characters");
   dialogsMenu->insertSeparator();
   dialogsMenu->insertItem("Portal");
   dialogsMenu->insertSeparator();
   dialogsMenu->insertItem("Particles");
   dialogsMenu->endCreate();
   dialogsButton->setMenu(dialogsMenu);
}

/***********************************************************************
 *                              ~MainGui                               *
 ***********************************************************************/
MainGui::~MainGui()
{
   /* Farso::Controller::finish will delete our widgets for us. */
}

/***********************************************************************
 *                           openLoadOrSave                            *
 ***********************************************************************/
void MainGui::openLoadOrSaveWindow(bool loading)
{
   /* Remove if already created */
   if(loadSaveWindow)
   {
      loadSaveWindow->close();
   }
   /* Create the window */
   loadSaveWindow = new Farso::Window(300, 250, ((loading) ? "Load" : "Save"));
   loadSaveSelector = new Farso::FileSelector(loading, "../data/maps/", true, 
         loadSaveWindow);
   loadSaveSelector->setFilter(".map");
   loadSaveWindow->setExternPointer(&loadSaveWindow);
   loadSaveWindow->open();
   loadSaveWindow->setPosition(40, 40);
}

/***********************************************************************
 *                            checkEvents                              *
 ***********************************************************************/
bool MainGui::checkEvents()
{
   Farso::Event event = Farso::Controller::getLastEvent();
   if(event.getType() == Farso::EVENT_MENU_SELECTED)
   {
      if(event.getWidget() == fileMenu)
      {
         if(fileMenu->getCurrentItem() == menuItemExit)
         {
            /* Should quit */
            return true;
         }
         else if(fileMenu->getCurrentItem() == menuItemLoad)
         {
            openLoadOrSaveWindow(true);
         }
      }
      else if(event.getWidget() == viewMenu)
      {
         if(viewMenu->getCurrentItem() == menuItemToggleLight)
         {
            lightEnabled = !lightEnabled;
            if(lightEnabled)
            {
               /* Enable light */
               menuItemToggleLight->setCaption("Disable light");
               DNT::Game::getSceneManager()->setAmbientLight(
                     Ogre::ColourValue(0.1f, 0.1f, 0.1f));
            }
            else
            {
               /* Disable the lights */
               menuItemToggleLight->setCaption("Enable light");
               DNT::Game::getSceneManager()->setAmbientLight(
                     Ogre::ColourValue(0.8f, 0.8f, 0.8f));
            }
         }
      }
   }
   else if(event.getWidget() == loadSaveSelector)
   {
      if(event.getType() == Farso::EVENT_FILESELECTOR_ACCEPT)
      {
         if(loadSaveSelector->isLoadType())
         {
            /* Must load the map */
            if(DNT::Game::loadMap("tyrol/house1.map"))
            {
               menuItemSave->enable();
               menuItemSaveAs->enable();
            }
            else
            {
               /* TODO: Show error! */
               menuItemSave->disable();
               menuItemSaveAs->disable();
            }
         }
         else
         {
            /* Must save the map */
         }
         loadSaveWindow->close();
      }
      else if(event.getType() == Farso::EVENT_FILESELECTOR_CANCEL)
      {
         /* Close the window */
         loadSaveWindow->close();
      }
   }

   /* Should not quit. */
   return false;
}
 
/***********************************************************************
 *                           isLightEnabled                            *
 ***********************************************************************/       bool MainGui::isLightEnabled()
{
   return lightEnabled;
}

