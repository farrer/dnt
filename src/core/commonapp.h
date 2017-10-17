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


#ifndef _dnt_common_app_h_
#define _dnt_common_app_h

#include "dntconfig.h"
#include <goblin/baseapp.h>
#include <farso/controller.h>

namespace DNT
{

#define DEFAULT_WINDOW_WIDTH     1024
#define DEFAULT_WINDOW_HEIGHT     700
#define WIDTH_TO_DOUBLE_SIZE     2000
#define BASE_DATA_DIR           "../data"

/*! An abstract class with basic common DNT application related things.
 * Mostly related to Application initialization. */ 
class CommonApp : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      CommonApp();
      /*! Destructor */
      virtual ~CommonApp();
   protected:
      /*! \return skin file to use for Farso */
      virtual const Ogre::String getSkin() const = 0;
      /*! Do anithing needed after select a node with mouse click */
      virtual bool specialSelect(Ogre::SceneNode* sceneNode) = 0;

      /* From Goblin::BaseApp */
      /*! \return false, as DNT won't use overlay */
      const bool getUseOverlay() const { return false; };
      /*! \return current width to use */
      const int getDefaultWindowWidth() const { return DEFAULT_WINDOW_WIDTH; };
      /*! \return current height to use */
      const int getDefaultWindowHeight() const { return DEFAULT_WINDOW_HEIGHT;};
      /*! \return current orientation to the window to use */
      const Orientation getDefaultOrientation() const { return LANDSCAPE; };
      /*! \return minimun size to double the gui widgets */
      const int getMinWidthToUseDoubleSizedGui() const
      { 
         return WIDTH_TO_DOUBLE_SIZE; 
      };
      /*! \return base data directory to use */
      const Ogre::String getBaseDataDir() const {return BASE_DATA_DIR;};
      /*! \return if should use Kobold::I18n for internationalization */
      const bool shouldUseKoboldI18n() const { return false; };
      /*! Get the data directories for immediate load */
      void getDataDirectories(Ogre::String** dataDirectories,
            Ogre::String** dataGroups, int& total);
      /*! If should do camera input at this step */
      const bool shouldDoCameraInput();
      /* Done with Goblin::BaseApp */

      /*! Init, cycled, the engine */
      bool doCommonCycleInit(int callCounter, bool& shouldAbort);
      /*! Update the mouse world position 
       * \param updateThingUnderMouse true to update the thing pointer of
       * a thing that is pointed by the mouse (if any). */
      void updateMouseWorld(bool updateThingUnderMouse);

      Ogre::Vector3 floorMouse; /**< Coordinates of mouse on floor */ 
      int lastMouseX; /**< Last mouse X coordinate when updated floorMouse */
      int lastMouseY; /**< Last mouse Y coordinate when updated floorMouse */

      Ogre::RaySceneQuery* ogreRaySceneQuery;/**< To ray cast */
      DNT::Thing* thingUnderCursor; /**< Current Thing under mouse cursor */
      Farso::ProgressBar* progressBar; /**< A multiple-use progress bar */
};

}

#endif

