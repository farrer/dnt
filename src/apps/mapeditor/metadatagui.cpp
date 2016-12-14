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

#include "metadatagui.h"

#include "../../core/game.h"
#include "../../map/map.h"

using namespace DNTMapEditor;

#define DNT_MUSIC_PREFIX "../data/music/"
 
/***********************************************************************
 *                           Consstructor                              *
 ***********************************************************************/ 
MetadataGui::MetadataGui()
{
   metadataWindow = NULL;
   mapNameTextEntry = NULL;
   mapMusicTextEntry = NULL;
   buttonSelectMusic = NULL;
   buttonApplyMetadata = NULL;

   selectWindow = NULL;
   musicSelector = NULL;
}

/***********************************************************************
 *                            Destructor                               *
 ***********************************************************************/
MetadataGui::~MetadataGui()
{
}

/***********************************************************************
 *                                open                                 *
 ***********************************************************************/
void MetadataGui::open()
{
   Ogre::StringStream ss;

   close();
   metadataWindow = new Farso::Window(300, 125, "Metadata");

   ss << "Size: ";
   ss << DNT::Game::getCurrentMap()->getSizeX();
   ss << " x ";
   ss << DNT::Game::getCurrentMap()->getSizeZ();
   new Farso::Label(0, 1, 200, 21, ss.str(), metadataWindow);
   new Farso::Label(0, 23, 44, 21, "Name:", metadataWindow);
   mapNameTextEntry = new Farso::TextEntry(45, 23, 155, 21, metadataWindow);
   mapNameTextEntry->setCaption(DNT::Game::getCurrentMap()->getName());
   new Farso::Label(0, 45, 44, 21, "Music:", metadataWindow);
   mapMusicTextEntry = new Farso::TextEntry(45, 45, 155, 21, metadataWindow); 
   mapMusicTextEntry->setCaption(
         DNT::Game::getCurrentMap()->getMusicFilename());
   mapMusicTextEntry->disable();
   buttonSelectMusic = new Farso::Button(205, 45, 80, 21, "Select", 
         metadataWindow);
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_BOTTOM_CENTERED, metadataWindow);
   buttonApplyMetadata = new Farso::Button(0, 2, 80, 21, "Apply", cont);

   metadataWindow->setExternPointer(&metadataWindow);
   metadataWindow->open();
   metadataWindow->setPosition(Kobold::Mouse::getX(), Kobold::Mouse::getY());
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void MetadataGui::close()
{
   if(metadataWindow)
   {
      metadataWindow->close();
   }
   if(selectWindow)
   {
      selectWindow->close();
   }
}

/***********************************************************************
 *                        openSelectMusicWindow                        *
 ***********************************************************************/
void MetadataGui::openSelectMusicWindow()
{
   /* Remove if already created */
   if(selectWindow)
   {
      selectWindow->close();
   }
   /* Create the window */
   selectWindow = new Farso::Window(300, 250, "Select Music");
   musicSelector = new Farso::FileSelector(true, DNT_MUSIC_PREFIX, false, 
         selectWindow);
   musicSelector->setFilter(".ogg");
   selectWindow->setExternPointer(&selectWindow);
   selectWindow->open();
   selectWindow->setPosition(Kobold::Mouse::getX(), Kobold::Mouse::getY());
}

/***********************************************************************
 *                            checkEvents                              *
 ***********************************************************************/
bool MetadataGui::checkEvents()
{
   if((selectWindow) && (!metadataWindow))
   {
      /* Closed the metadata window (but not the select), so close it. */
      selectWindow->close();
   }

   Farso::Event event = Farso::Controller::getLastEvent();
   if(event.getType() == Farso::EVENT_BUTTON_RELEASE)
   {
      if(event.getWidget() == buttonSelectMusic)
      {
         /* Open the music selector */
         openSelectMusicWindow();
         return true;
      }
      else if(event.getWidget() == buttonApplyMetadata)
      {
         /* Apply the data to the map and close */
         DNT::Game::getCurrentMap()->setName(mapNameTextEntry->getCaption());
         DNT::Game::getCurrentMap()->setMusicFilename(
               mapMusicTextEntry->getCaption());
         close();
         return true;
      }
   }
   else if(event.getType() == Farso::EVENT_FILESELECTOR_ACCEPT)
   {
      if(event.getWidget() == musicSelector)
      {
         /* Define the new filename (without prefix) and close the window */
         mapMusicTextEntry->setCaption(
               Ogre::StringUtil::replaceAll(musicSelector->getFilename(),
                                            DNT_MUSIC_PREFIX, ""));
         selectWindow->close();
         if(metadataWindow)
         {
            metadataWindow->activate();
         }
         return true;
      }
   }
   else if(event.getType() == Farso::EVENT_FILESELECTOR_CANCEL)
   {
      if(event.getWidget() == musicSelector)
      {
         /* Just close the dialog */
         selectWindow->close();
         if(metadataWindow)
         {
            metadataWindow->activate();
         }
         return true;
      }
   }
   return false;
}

