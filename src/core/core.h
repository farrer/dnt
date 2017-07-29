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
#include "commonapp.h"

namespace DNT
{

#define APP_NAME "dnt"
#define APP_SKIN "skins/moderna.skin"

/*! The core (main entry) implementation of DNT. */
class Core : public CommonApp
{
   public:
      /*! Constructor */
      Core();
      /*! Destructor */
      ~Core();
   protected:

      /*! \return application name to display as window title */
      const Ogre::String getApplicationUnixName() const { return APP_NAME; };
      const Ogre::String getSkin() const {return APP_SKIN;};

      /*! Do a single cycle of the init procedure */
      bool doCycleInit(int callCounter, bool& shouldAbort);

      /*! Called when the memory is low. Only for portable devices */
      void doLowMemoryClean();
      /*! Called when the application was sent to background */
      void doSendToBackground();
      /*! Called when the application was sent back to foreground */
      void doSendToForeground();

      /*! Called just before the render */
      void doBeforeRender();
      /*! Called juset after the render */
      void doAfterRender();
      /*! \return true to quit the application */
      bool shouldQuit();
      bool specialSelect(Ogre::SceneNode* sceneNode);

   private:

      Thing* testedThingUnderCursor; /**< Thing under cursor of the last 
                                          atSight test */
      bool testedAtSightResult;  /**< Result of the atSight for last test */
      bool treatedGui; /**< If treated some GUI event */

};

}

#endif


