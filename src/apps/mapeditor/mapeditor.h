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
#include "../../core/commonapp.h"

#include "maingui.h"
#include "positioneditor.h"

namespace DNTMapEditor
{

#define APP_NAME                "dntMapEditor"
#define APP_SKIN                "skins/clean.skin"

/*! The BaseApp implementation for DNTMapDitor */
class MapEditor : public DNT::CommonApp
{
   public:
      /*! Constructor */
      MapEditor();
      /*! Destructor */
      ~MapEditor();
   protected:

      /*! \return string to show as window title */
      const Ogre::String getApplicationUnixName() const { return APP_NAME; };
      /*! \return Farso skint to use */
      const Ogre::String getSkin() const {return APP_SKIN;};
      /*! Execute a single step of the init */
      bool doCycleInit(int callCounter, bool& shouldAbort);
 
      /*! Unused */
      void doLowMemoryClean();
      /*! Unused */
      void doSendToBackground();
      /*! Unused */
      void doSendToForeground();

      /*! Called before the render */
      void doBeforeRender();
      /*! Called after the render */
      void doAfterRender();
      /*! \return true to quit */
      bool shouldQuit();
      bool specialSelect(Ogre::SceneNode* sceneNode);

   private:
      /*! Check keyboard input for editor */
      bool checkKeyboardInput();

      MainGui* mainGui; /**< Main editor's gui */
      PositionEditor* positionEditor; /**< Position editor */

      bool shouldExit; /**< exit flag */
      bool treatedGui; /**< if got gui event on current frame */

      Kobold::KeyCodes lastKey; /**< Last action key checked */

};

}

#endif


