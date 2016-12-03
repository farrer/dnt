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


#ifndef _dnt_core_h_
#define _dnt_core_h

#include "dntconfig.h"
#include <goblin/baseapp.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

namespace DNT
{

#define APP_NAME                "dnt"
#define DEFAULT_WINDOW_WIDTH     1024
#define DEFAULT_WINDOW_HEIGHT     768
#define WIDTH_TO_DOUBLE_SIZE     2000
#define BASE_DATA_DIR           "../data"

/*! The core (main entry) implementation of DNT. */
class Core : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      Core();
      /*! Destructor */
      ~Core();
   protected:

      Ogre::String getApplicationUnixName() { return APP_NAME; };
      int getDefaultWindowWidth() { return DEFAULT_WINDOW_WIDTH; };
      int getDefaultWindowHeight() { return DEFAULT_WINDOW_HEIGHT; };
      Orientation getDefaultOrientation() { return LANDSCAPE; };
      int getMinWidthToUseDoubleSizedGui() { return WIDTH_TO_DOUBLE_SIZE; };
      Ogre::String getBaseDataDir() {return BASE_DATA_DIR;};

      bool doInit();
      void getDataDirectories(Ogre::String** dataDirectories,
            Ogre::String** dataGroups, int& total);

      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();
      void doCycle();
      bool shouldQuit();

   private:

      void initBlendMaps(Ogre::Terrain* terrain);

      Ogre::TerrainGlobalOptions* terrainGlobalOptions;
      Ogre::TerrainGroup* terrainGroup;
};

}

#endif


