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

      const Ogre::String getApplicationUnixName() const { return APP_NAME; };
      const Ogre::String getSkin() const {return APP_SKIN;};

      bool doCycleInit(int callCounter, bool& shouldAbort);

      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();

      void doCycle();
      bool shouldQuit();
      bool specialSelect(Ogre::SceneNode* sceneNode);

   private:


};

}

#endif


