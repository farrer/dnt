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


#include "mapeditor.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   #include "windows.h"
#endif
 

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
      LPSTR lpCmdLine, int nCmdShow) 
#else
int main (int argc, char *argv[])
#endif
{
   DNTMapEditor::MapEditor* editor = new DNTMapEditor::MapEditor();
   editor->run();
   delete editor;
}
#endif
