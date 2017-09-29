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
   /* Nullify Load/Save window things */
   loadSaveWindow = NULL;
   loadSaveSelector = NULL;
   newMapWindow = NULL;
   spinNewMapSizeX = NULL;
   spinNewMapSizeZ = NULL;
   buttonNewMapCreate = NULL;
   module = NULL;
   loadingModule = false;

   /* Create the progress bar */
   progressBar = new Farso::ProgressBar(412, 374, 200, 20, NULL);

   cont = new Farso::Container(Farso::Container::TYPE_TOP_LEFT, 
         0, 0, 1024, 23, NULL);
   cont->setFilled();
   
   /* Create File button and its menu */
   int pX = 1;
   fileButton = new Farso::Button(pX, 1, 80, 21, "File", cont);
   fileMenu = new Farso::Menu(100);
   fileMenu->beginCreate();
   menuItemOpenModule = fileMenu->insertItem("Open Module...");
   fileMenu->insertSeparator();
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
   pX += 80;

   /* Create edit button and menu */
   editButton = new Farso::Button(pX, 1, 80, 21, "Edit", cont);
   editMenu = new Farso::Menu(120);
   editMenu->beginCreate();
   menuItemUnselect = editMenu->insertItem("Unselect", "CTRL+U");
   menuItemDuplicate = editMenu->insertItem("Duplicate", "CTRL+D");
   editMenu->insertSeparator();
   menuItemRemove = editMenu->insertItem("Remove", "CTRL+BACKSPACE");
   editMenu->endCreate();
   editButton->setMenu(editMenu);
   pX += 80;
   
   /* Create view button and menu */
   viewButton = new Farso::Button(pX, 1, 80, 21, "View", cont);
   viewMenu = new Farso::Menu();
   viewMenu->beginCreate();
   menuItemToggleLight = viewMenu->insertItem("Disable light");
   lightEnabled = true;
   viewMenu->insertSeparator();
   menuItemShowConnections = viewMenu->insertItem("Show connections");
   viewMenu->endCreate();
   viewButton->setMenu(viewMenu);
   pX += 80;

   /* Create dialogs button and menu */
   dialogsButton = new Farso::Button(pX, 1, 80, 21, "Dialogs", cont);
   dialogsMenu = new Farso::Menu(120);
   dialogsMenu->beginCreate();
   menuItemMetadata = dialogsMenu->insertItem("Metadata", "ALT+M");
   dialogsMenu->insertSeparator();
   menuItemNodes = dialogsMenu->insertItem("Nodes", "ALT+N");
   menuItemTransform = dialogsMenu->insertItem("Transform", "ALT+T");
   dialogsMenu->insertSeparator();
   menuItemLight = dialogsMenu->insertItem("Light", "ALT+L");
   dialogsMenu->endCreate();
   dialogsButton->setMenu(dialogsMenu);

   toggleMenuStatus();
}

/***********************************************************************
 *                              ~MainGui                               *
 ***********************************************************************/
MainGui::~MainGui()
{
   closeMapRelatedWindows(NULL);
   if(loadSaveWindow)
   {
      loadSaveWindow->close();
   }
   if(newMapWindow)
   {
      newMapWindow->close();
   }
   if(module)
   {
      delete module;
   }
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
   if((module != NULL) && (!loadingModule))
   {
      menuItemOpenModule->disable();
      menuItemNewIndoor->enable();
      menuItemNewOutdoor->enable();
      menuItemLoad->enable();
   }
   else
   {
      if(!loadingModule)
      {
         menuItemOpenModule->enable();
      }
      else
      {
         menuItemOpenModule->disable();
      }
      menuItemNewIndoor->disable();
      menuItemNewOutdoor->disable();
      menuItemLoad->disable();
   }
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
      menuItemTransform->enable();
      menuItemLight->enable();

      menuItemToggleLight->enable();
      menuItemShowConnections->enable();

      menuItemMetadata->enable();
      menuItemNodes->enable();
   }
   else
   {
      menuItemSave->disable();
      menuItemSaveAs->disable();
      menuItemTransform->disable();
      menuItemLight->disable();
      menuItemMetadata->disable();
      menuItemNodes->disable();
      disableEditItems();
   }
}

/***********************************************************************
 *                            enableEditItems                          *
 ***********************************************************************/
void MainGui::enableEditItems()
{
   menuItemUnselect->enable();
   menuItemDuplicate->enable();
   menuItemRemove->enable();
}

/***********************************************************************
 *                           disableEditItems                          *
 ***********************************************************************/
void MainGui::disableEditItems()
{
   menuItemUnselect->disable();
   menuItemDuplicate->disable();
   menuItemRemove->disable();
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
            Ogre::ColourValue(0.2f, 0.2f, 0.2f),
            Ogre::ColourValue(0.2f, 0.2f, 0.2f),
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
 *                       closeMapRelatedWindows                        *
 ***********************************************************************/
void MainGui::closeMapRelatedWindows(PositionEditor* positionEditor)
{
   metadataGui.close();
   nodesWindow.close();
   transformWindow.close();
   lightWindow.close();
   if(positionEditor)
   {
      positionEditor->clear();
   }
}

/***********************************************************************
 *                          toggleNodeWindow                           *
 ***********************************************************************/
void MainGui::toggleNodeWindow()
{
   if(!DNT::Game::getCurrentMap())
   {
      return;
   }

   if(nodesWindow.isOpened())
   {
      nodesWindow.close();
   }
   else
   {
      nodesWindow.open();
   }
}

/***********************************************************************
 *                       toggleMetadataWindow                          *
 ***********************************************************************/
void MainGui::toggleMetadataWindow()
{
   if(!DNT::Game::getCurrentMap())
   {
      return;
   }

   if(metadataGui.isOpened())
   {
      metadataGui.close();
   }
   else
   {
      metadataGui.open();
   }
}

/***********************************************************************
 *                       toggleTransformWindow                         *
 ***********************************************************************/
void MainGui::toggleTransformWindow()
{
   if(!DNT::Game::getCurrentMap())
   {
      return;
   }

   if(transformWindow.isOpened())
   {
      transformWindow.close();
   }
   else
   {
      transformWindow.open();
   }
}

/***********************************************************************
 *                         toggleLightWindow                           *
 ***********************************************************************/
void MainGui::toggleLightWindow()
{
   if(!DNT::Game::getCurrentMap())
   {
      return;
   }

   if(lightWindow.isOpened())
   {
      lightWindow.close();
   }
   else
   {
      lightWindow.open();
   }
}

/***********************************************************************
 *                           openLoadOrSave                            *
 ***********************************************************************/
void MainGui::openLoadOrSaveWindow(bool loading, bool map)
{
   /* Remove if already created */
   if(loadSaveWindow)
   {
      loadSaveWindow->close();
   }

   /* Define directory */
   Kobold::String dir, filter;
   if(map)
   {
      dir = module->getPath() + "/maps/";
      filter = ".map";
   }
   else
   {
      dir = "../data/modules/";
      filter = ".as";
   }


   /* Create the window */
   loadSaveWindow = new Farso::Window(300, 250, ((loading) ? "Load" : "Save"));
   loadSaveSelector = new Farso::FileSelector(loading, dir, true, 
         loadSaveWindow);
   loadSaveSelector->setFilter(filter);
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
 *                               update                                *
 ***********************************************************************/
void MainGui::update(PositionEditor* positionEditor)
{
   transformWindow.update(positionEditor);
   lightWindow.update(positionEditor);

   /* Check if we are on a cycle of loading module. */
   if(loadingModule)
   {
      progressBar->show();
      loadingModule = !module->doCycleInit(false, module->getPath(), 
            progressBar);
      if(!loadingModule)
      {
         /* Finished loading */
         toggleMenuStatus();
         progressBar->hide();
      }
   }
}

/***********************************************************************
 *                        duplicateSelection                           *
 ***********************************************************************/
void MainGui::duplicateSelection(PositionEditor* positionEditor)
{
   if(positionEditor->getSelectedThing())
   {
      DNT::Thing* thing = positionEditor->getSelectedThing();
      Ogre::Vector3 pos = thing->getModel()->getPosition();
      pos.x += 0.5f;
      //TODO: check if new position is inner the map!
      DNT::Thing* newThing = DNT::Game::getCurrentMap()->insertThing(
            thing->getFilename(), true, pos, 
            Ogre::Vector3(thing->getModel()->getPitch(),
               thing->getModel()->getYaw(),
               thing->getModel()->getRoll()));
      /* Add to node's tree, if opened */
      if(nodesWindow.isOpened())
      {
         nodesWindow.addThing(newThing);
         nodesWindow.setSelectedNodeByData(newThing);
      }
      positionEditor->selectThing(newThing);
   }
   else if(positionEditor->getSelectedLight())
   {
      DNT::LightInfo* lightInfo = positionEditor->getSelectedLight();
      Ogre::Vector3 pos = lightInfo->getPosition();
      pos.x += 0.5f;
      //TODO: check if new position is inner the map!
      DNT::LightInfo* newLight = DNT::Game::getCurrentMap()->
         getLights()->createLightInfo(lightInfo->getType());
      newLight->set(lightInfo);
      newLight->setPosition(pos);

      /* Add to node's tree, if opened */
      if(nodesWindow.isOpened())
      {
         nodesWindow.addLight(newLight);
         nodesWindow.setSelectedNodeByData(newLight);
      }
      positionEditor->selectLight(newLight);
      newLight->flush();
   }
}

/***********************************************************************
 *                          removeSelection                            *
 ***********************************************************************/
void MainGui::removeSelection(PositionEditor* positionEditor)
{
   if(positionEditor->getSelectedThing())
   {
      nodesWindow.removeNodeByData(static_cast<void*>(
               positionEditor->getSelectedThing()));
      DNT::Game::getCurrentMap()->removeThing(
            positionEditor->getSelectedThing());
   }
   else if(positionEditor->getSelectedLight())
   {
      nodesWindow.removeNodeByData(static_cast<void*>(
               positionEditor->getSelectedLight()));
      DNT::Game::getCurrentMap()->getLights()->removeLight(
            positionEditor->getSelectedLight());
   }
   /* And unselect it */
   positionEditor->selectThing(NULL);
}

/***********************************************************************
 *                              unselect                               *
 ***********************************************************************/
void MainGui::unselect(PositionEditor* positionEditor)
{
   positionEditor->selectThing(NULL);
   nodesWindow.unselect();
}

/***********************************************************************
 *                            checkEvents                              *
 ***********************************************************************/
bool MainGui::checkEvents(PositionEditor* positionEditor)
{
   if(loadingModule)
   {
      return false;
   }

   /* Check if need to change EditItems available status */
   if((menuItemUnselect->isEnabled()) && (!positionEditor->hasSelection()))
   {
      disableEditItems();
   }
   else if((!menuItemUnselect->isEnabled()) && 
           (positionEditor->hasSelection()))
   {
      enableEditItems();
   }

   /* Let's check any event */
   Farso::Event event = Farso::Controller::getLastEvent();
   if(event.getType() == Farso::EVENT_MENU_SELECTED)
   {
      if(event.getWidget() == fileMenu)
      {
         if(fileMenu->getCurrentItem() == menuItemOpenModule)
         {
            openLoadOrSaveWindow(true, false);
         }
         else if(fileMenu->getCurrentItem() == menuItemNewIndoor)
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
            openLoadOrSaveWindow(true, true);
         }
         else if(fileMenu->getCurrentItem() == menuItemSave)
         {
            DNT::Map* curMap = DNT::Game::getCurrentMap();
            if(curMap->save(curMap->getFilename()))
            {
               //TODO: add a success message.
            }
            else
            {
               //TODO: show error message.
            }
         }
         else if(fileMenu->getCurrentItem() == menuItemSaveAs)
         {
            openLoadOrSaveWindow(false, true);
         }
      }
      /* Edit Menu */
      else if(event.getWidget() == editMenu)
      {
         if(editMenu->getCurrentItem() == menuItemUnselect)
         {
            /* Unselect current selection */
            unselect(positionEditor);
         } 
         else if(editMenu->getCurrentItem() == menuItemDuplicate)
         {
            /* Duplicate selection */
            duplicateSelection(positionEditor);
         }
         else if(editMenu->getCurrentItem() == menuItemRemove)
         {
            /* Remove current selection (both from nodeWindow and map) */
            removeSelection(positionEditor);
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
      else if(event.getWidget() == dialogsMenu)
      {
         if(dialogsMenu->getCurrentItem() == menuItemMetadata)
         {
            metadataGui.open();
         }
         else if(dialogsMenu->getCurrentItem() == menuItemNodes)
         {
            nodesWindow.open();
         }
         else if(dialogsMenu->getCurrentItem() == menuItemTransform)
         {
            transformWindow.open();
         }
         else if(dialogsMenu->getCurrentItem() == menuItemLight)
         {
            lightWindow.open();
         }
      }
   }
   else if(event.getType() == Farso::EVENT_BUTTON_RELEASE)
   {
      /* Check creation of a new map */
      if(event.getWidget() == buttonNewMapCreate)
      {
         closeMapRelatedWindows(positionEditor);
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
            closeMapRelatedWindows(positionEditor);

            if(module != NULL)
            {
               /* Must load the map */
               if(DNT::Game::loadMap(loadSaveSelector->getFilename(), true, 
                        true, true))
               {
                  setCameraOnMap();
               }
               else
               {
                  /* TODO: Show error! */
               }
            }
            else
            {
               /* Create and load the module */
               module = new DNT::Module(loadSaveSelector->getFilename());
               loadingModule = true;
            }
            toggleMenuStatus();
         }
         else
         {
            /* Must save the map */
            if(DNT::Game::getCurrentMap()->save(
                     loadSaveSelector->getFilename()))
            {
               /* TODO: Show success message */
            }
            else
            {
               /* TODO: Show error message */
            }
         }
         loadSaveWindow->close();
      }
      else if(event.getType() == Farso::EVENT_FILESELECTOR_CANCEL)
      {
         /* Close the window */
         loadSaveWindow->close();
      }
   }
   
   //FIXME: not call when already got the event.
   if(metadataGui.checkEvents())
   {
   }
   else if(nodesWindow.checkEvents(positionEditor))
   {
   }
   else if(transformWindow.checkEvents(positionEditor))
   {
   }
   else if(lightWindow.checkEvents(positionEditor))
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

