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


#ifndef _dnt_mapeditor_h_
#define _dnt_mapeditor_h

#include "dntconfig.h"
#include <goblin/baseapp.h>

#include "maingui.h"

namespace DNTMapEditor
{

#define APP_NAME                "dntMapEditor"
#define DEFAULT_WINDOW_WIDTH     1024
#define DEFAULT_WINDOW_HEIGHT     768
#define WIDTH_TO_DOUBLE_SIZE     2000
#define BASE_DATA_DIR           "../data"

/*! The BaseApp implementation for DNTMapDitor */
class MapEditor : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      MapEditor();
      /*! Destructor */
      ~MapEditor();
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

#if OGRE_VERSION_MAJOR == 1
      /*! Override our BaseApp shadow technique to  what we defined. */
      Ogre::ShadowTechnique getShadowTechnique();
#endif

      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();

      void doCycle();
      bool shouldQuit();

   private:

      Ogre::Vector3 floorMouse; /**< Coordinates of mouse on floor */ 
      int lastMouseX; /**< Last mouse X coordinate when updated floorMouse */
      int lastMouseY; /**< Last mouse Y coordinate when updated floorMouse */

      MainGui* mainGui; /**< Main editor's gui */

      bool shouldExit; /**< exit flag */

};

}

#endif


