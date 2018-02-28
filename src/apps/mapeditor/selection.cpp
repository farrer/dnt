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

#include "selection.h"
using namespace DNTMapEditor;

/***********************************************************************
 *                               clear                                 *
 ***********************************************************************/
void Selection::clear()
{
   selectedLight = NULL;
   selectedThing = NULL;
}

/***********************************************************************
 *                           hasSelection                              *
 ***********************************************************************/
bool Selection::hasSelection()
{
   return selectedThing != NULL || selectedLight != NULL;
}


/***********************************************************************
 *                           selectThing                               *
 ***********************************************************************/
void Selection::selectThing(DNT::Thing* thing)
{
   selectedLight = NULL;
   selectedThing = thing;
}

/***********************************************************************
 *                           selectLight                               *
 ***********************************************************************/
void Selection::selectLight(DNT::LightInfo* light)
{
   selectedThing = NULL;
   selectedLight = light;
}


/***********************************************************************
 *                            attributes                               *
 ***********************************************************************/
DNT::Thing* Selection::selectedThing = NULL;
DNT::LightInfo* Selection::selectedLight = NULL;


