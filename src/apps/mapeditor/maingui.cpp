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
#include "../../map/map.h"

#include <goblin/camera.h>

using namespace DNTMapEditor;

/***********************************************************************
 *                               MainGui                               *
 ***********************************************************************/
MainGui::MainGui()
{
   /* Create the progress bar */
   progressBar = new Farso::ProgressBar(412, 374, 200, 20, NULL);

   cont = new Farso::Container(Farso::Container::TYPE_TOP_LEFT, 
         0, 0, 1024, 23, NULL);
   cont->setFilled();
   
   /* Create File button and its menu */
   fileButton = new Farso::Button(1, 1, 80, 21, "File", cont);
   fileMenu = new Farso::Menu(100);
   fileMenu->beginCreate();
   menuItemNewIndoor = fileMenu->insertItem("New Indoor Map...");
   menuItemNewOutdoor = fileMenu->insertItem("New Outdoor Map...");
   fileMenu->insertSeparator();
   menuItemLoad = fileMenu->insertItem("Load...");
   menuItemSave = fileMenu->insertItem("Save");
   menuItemSaveAs = fileMenu->insertItem("Save as...");
   fileMenu->insertSeparator();
   menuItemExit = fileMenu->insertItem("Exit");
   fileMenu->endCreate();
   fileButton->setMenu(fileMenu);

   /* Nullify Load/Save window things */
   loadSaveWindow = NULL;
   loadSaveSelector = NULL;
   newMapWindow = NULL;
   spinNewMapSizeX = NULL;
   spinNewMapSizeZ = NULL;
   buttonNewMapCreate = NULL;
   
   /* Create view button and menu */
   viewButton = new Farso::Button(81, 1, 80, 21, "View", cont);
   viewMenu = new Farso::Menu();
   viewMenu->beginCreate();
   menuItemToggleLight = viewMenu->insertItem("Disable light");
   lightEnabled = true;
   viewMenu->insertSeparator();
   menuItemShowConnections = viewMenu->insertItem("Show connections");
   viewMenu->endCreate();
   viewButton->setMenu(viewMenu);

   /* Create dialogs button and menu */
   dialogsButton = new Farso::Button(161, 1, 80, 21, "Dialogs", cont);
   dialogsMenu = new Farso::Menu(120);
   dialogsMenu->beginCreate();
   menuItemLights = dialogsMenu->insertItem("Lights");
   menuItemSounds = dialogsMenu->insertItem("Sounds");
   dialogsMenu->insertSeparator();
   menuItemTerrain = dialogsMenu->insertItem("Terrain");
   menuItemWall = dialogsMenu->insertItem("Wall");
   menuItemTileWall = dialogsMenu->insertItem("Tile Wall");
   dialogsMenu->insertSeparator();
   menuItemObjects = dialogsMenu->insertItem("Objects");
   menuItemCharacters = dialogsMenu->insertItem("Characters");
   dialogsMenu->insertSeparator();
   menuItemPortal = dialogsMenu->insertItem("Portal");
   dialogsMenu->insertSeparator();
   menuItemParticles = dialogsMenu->insertItem("Particles");
   dialogsMenu->endCreate();
   dialogsButton->setMenu(dialogsMenu);

   /* Create map button and menu */
   mapButton = new Farso::Button(241, 1, 80, 21, "Map", cont);
   mapMenu = new Farso::Menu(100);
   mapMenu->beginCreate();
   menuItemMetadata = mapMenu->insertItem("Metadata");
   menuItemNodes = mapMenu->insertItem("Nodes");
   mapMenu->endCreate();
   mapButton->setMenu(mapMenu);

   toggleMenuStatus();
}

/***********************************************************************
 *                              ~MainGui                               *
 ***********************************************************************/
MainGui::~MainGui()
{
   /* Farso::Controller::finish will delete our widgets for us. */
}

/***********************************************************************
 *                            hideTopBar                               *
 ***********************************************************************/
void MainGui::hideTopBar()
{
   cont->hide();
}

/***********************************************************************
 *                            showTopBar                               *
 ***********************************************************************/
void MainGui::showTopBar()
{
   cont->show();
}

/***********************************************************************
 *                          showProgressBar                            *
 ***********************************************************************/
void MainGui::showProgressBar()
{
   progressBar->show();
}

/***********************************************************************
 *                           setProgressBar                            *
 ***********************************************************************/
void MainGui::setProgressBar(int value)
{
   progressBar->setValue(value);
}

/***********************************************************************
 *                          hideProgressBar                            *
 ***********************************************************************/
void MainGui::hideProgressBar()
{
   progressBar->hide();
}


/***********************************************************************
 *                          toggleMenuStatus                           *
 ***********************************************************************/
void MainGui::toggleMenuStatus()
{
   menuItemNewIndoor->enable();
   menuItemNewOutdoor->enable();
   menuItemLoad->enable();
   menuItemExit->enable();

   if(DNT::Game::getCurrentMap())
   {
      /* Direct-save only enabled when with filename defined. */
      if(DNT::Game::getCurrentMap()->getFilename().empty())
      {
         menuItemSave->disable();
      }
      else
      {
         menuItemSave->enable();
      }
      menuItemSaveAs->enable();
      menuItemLights->enable();
      menuItemSounds->enable();
      menuItemTerrain->enable();
      menuItemWall->enable();
      menuItemTileWall->enable();
      menuItemObjects->enable();
      menuItemCharacters->enable();
      menuItemPortal->enable();
      menuItemParticles->enable();

      menuItemToggleLight->enable();
      menuItemShowConnections->enable();

      menuItemMetadata->enable();
      menuItemNodes->enable();
   }
   else
   {
      menuItemSave->disable();
      menuItemSaveAs->disable();
      menuItemLights->disable();
      menuItemSounds->disable();
      menuItemTerrain->disable();
      menuItemWall->disable();
      menuItemTileWall->disable();
      menuItemObjects->disable();
      menuItemCharacters->disable();
      menuItemPortal->disable();
      menuItemParticles->disable();
      menuItemToggleLight->disable();
      menuItemShowConnections->disable();
      menuItemMetadata->disable();
      menuItemNodes->disable();
   }
}

/***********************************************************************
 *                               setLight                              *
 ***********************************************************************/
void MainGui::setLight()
{
   //FIXME: for outdoor maps, must define the hemisphere colors with
   //       different values.
   if(lightEnabled)
   {
      /* Enable light */
      menuItemToggleLight->setCaption("Disable light");
      DNT::Game::getSceneManager()->setAmbientLight(
            Ogre::ColourValue(0.05f, 0.05f, 0.05f),
            Ogre::ColourValue(0.05f, 0.05f, 0.05f),
            Ogre::Vector3(0.0f, -1.0f, 0.0f));
/*
      Ogre::Light *light = DNT::Game::getSceneManager()->createLight();
      Ogre::SceneNode *lightNode = DNT::Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
      lightNode->attachObject( light );
      light->setPowerScale( 1.0f );
      light->setType( Ogre::Light::LT_DIRECTIONAL );
      light->setDirection( Ogre::Vector3( -1, -1, -1 ).normalisedCopy() ); */
   }
   else
   {
      /* Disable the lights */
      menuItemToggleLight->setCaption("Enable light");
      DNT::Game::getSceneManager()->setAmbientLight(
            Ogre::ColourValue(0.8f, 0.8f, 0.8f),
            Ogre::ColourValue(0.8f, 0.8f, 0.8f),
            Ogre::Vector3(0.0f, -1.0f, 0.0f));
   }
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
 *                          openNewMapWindow                           *
 ***********************************************************************/
void MainGui::openNewMapWindow()
{
   /* Close it, if opened */
   if(newMapWindow)
   {
      newMapWindow->close();
   }
   
   /* Create it */
   newMapWindow = new Farso::Window(158, 100, "New Map");
   new Farso::Label(0, 1, 44, 21, "SizeX:", newMapWindow);
   spinNewMapSizeX = new Farso::Spin(Farso::Spin::VALUE_TYPE_INTEGER, 45, 1,
         newMapWindow);
   spinNewMapSizeX->setValue(6);
   spinNewMapSizeX->setRange(1, 20);
   new Farso::Label(0, 23, 44, 21, "SizeZ:", newMapWindow);
   spinNewMapSizeZ = new Farso::Spin(Farso::Spin::VALUE_TYPE_INTEGER, 45, 23,
         newMapWindow);
   spinNewMapSizeZ->setValue(6);
   spinNewMapSizeZ->setRange(1, 20);
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_BOTTOM_CENTERED, newMapWindow);
   buttonNewMapCreate = new Farso::Button(0, 2, 100, 21, "Create", cont);
   newMapWindow->setExternPointer(&newMapWindow);
   newMapWindow->open();
   newMapWindow->setPosition(0, 100);
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
         if(fileMenu->getCurrentItem() == menuItemNewIndoor)
         {
            openNewMapWindow();
         }
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
            setLight();
         }
      }
      else if(event.getWidget() == mapMenu)
      {
         if(mapMenu->getCurrentItem() == menuItemMetadata)
         {
            metadataGui.open();
         }
         else if(mapMenu->getCurrentItem() == menuItemNodes)
         {
            nodesWindow.open();
         }
      }
   }
   else if(event.getType() == Farso::EVENT_BUTTON_RELEASE)
   {
      /* Check creation of a new map */
      if(event.getWidget() == buttonNewMapCreate)
      {
         /* Force deletion of any already loaded map */
         DNT::Game::setCurrentMap(NULL);
         /* Create a new map and set it to be the active one */
         DNT::Map* map = new DNT::Map();
         map->create(spinNewMapSizeX->getIntValue(), 
               spinNewMapSizeZ->getIntValue());
         DNT::Game::setCurrentMap(map);
         setCameraOnMap();
         toggleMenuStatus();
         newMapWindow->close();
      }
   }
   else if(event.getWidget() == loadSaveSelector)
   {
      if(event.getType() == Farso::EVENT_FILESELECTOR_ACCEPT)
      {
         if(loadSaveSelector->isLoadType())
         {
            /* Must load the map */
            if(DNT::Game::loadMap(loadSaveSelector->getFilename(), true))
            {
               setCameraOnMap();
            }
            else
            {
               /* TODO: Show error! */
            }
            toggleMenuStatus();
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
   
   if(metadataGui.checkEvents())
   {
   }

   /* Should not quit. */
   return false;
}

/***********************************************************************
 *                           setCameraOnMap                            *
 ***********************************************************************/
void MainGui::setCameraOnMap()
{
   int sizeX = DNT::Game::getCurrentMap()->getSizeX() *  MAP_SQUARE_SIZE;
   int sizeZ = DNT::Game::getCurrentMap()->getSizeZ() *  MAP_SQUARE_SIZE;
   /* Set camera area to the whole map */
   Goblin::Camera::limitCameraArea(Ogre::Vector3(0.0f,0.0f,0.0f),
         Ogre::Vector3(sizeX, 0.0f, sizeZ));

   /* Put it at map's center */
   Goblin::Camera::setPosition(Ogre::Vector3(sizeX / 2.0f, 0.0f, sizeZ / 2.0f));
}

/***********************************************************************
 *                           isLightEnabled                            *
 ***********************************************************************/
bool MainGui::isLightEnabled()
{
   return lightEnabled;
}

