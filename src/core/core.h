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

      const Ogre::String getApplicationUnixName() const { return APP_NAME; };
      const int getDefaultWindowWidth() const { return DEFAULT_WINDOW_WIDTH; };
      const int getDefaultWindowHeight() const {return DEFAULT_WINDOW_HEIGHT;};
      const Orientation getDefaultOrientation() const { return LANDSCAPE; };
      const int getMinWidthToUseDoubleSizedGui() const 
      { 
         return WIDTH_TO_DOUBLE_SIZE; 
      };
      const Ogre::String getBaseDataDir() const {return BASE_DATA_DIR;};
      const bool shouldUseKoboldI18n() const { return false; };
      const bool shouldCreateBasicWorkspace() const { return false; };


      bool doCycleInit(int callCounter, bool& shouldAbort);
      void getDataDirectories(Ogre::String** dataDirectories,
            Ogre::String** dataGroups, int& total);

      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();

      void doCycle();
      bool shouldQuit();

   private:

      Ogre::Vector3 floorMouse; /**< Coordinates of mouse on floor */ 
      int lastMouseX; /**< Last mouse X coordinate when updated floorMouse */
      int lastMouseY; /**< Last mouse Y coordinate when updated floorMouse */

};

}

#endif


