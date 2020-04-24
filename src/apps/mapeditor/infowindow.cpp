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

#include "infowindow.h"
#include "selection.h"
using namespace DNTMapEditor;

InfoWindow::InfoWindow()
{
   dialogFile = NULL;
   doorLocked = NULL;
   difficultValue = NULL;
   hardnessValue = NULL;
   psychoCombo = NULL;
   window = NULL;
   selected = NULL;
}

InfoWindow::~InfoWindow()
{
}

void InfoWindow::open()
{
}

void InfoWindow::close()
{
   if(window)
   {
      window->close();
   }
}

void InfoWindow::update()
{
   if(selected && !Selection::getSelectedThing())
   {
      /* Must close the window */
      close();
      return;
   }
   else if(selected != Selection::getSelectedThing())
   {
      selected = Selection::getSelectedThing();
   }
}

void InfoWindow::checkEvents()
{
}
