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

#ifndef _dnt_map_editor_position_editor_h
#define _dnt_map_editor_position_editor_h

#include <OGRE/OgreSceneManager.h>
#include <goblin/model3d.h>
#include "../../rules/thing.h"

namespace DNTMapEditor
{

   /*! Implementation of a simple position editor, based on axis */
   class PositionEditor
   {
      public:
         /*! Constructor 
          * \param sceneManager pointer to the used scene manager */
         PositionEditor(Ogre::SceneManager* sceneManager);
         /*! Destructor */
         ~PositionEditor();

         /*! Check if the scene node represents an axis. If true,
          * select this axis.
          * \param sceneNode pointer to the sceneNode to check
          * \return if sceneNode is referent to an axis. */
         bool selectAxis(Ogre::SceneNode* sceneNode);

         /*! Select a Thing for position edition */
         void selectThing(DNT::Thing* thing);

         bool update(bool leftButtonPressed, int cursorX, int cursorY);

      private:

         void hide();
         void show();

         /*! Set axis positions to be relative to */
         void setPosition(Ogre::Vector3 pos);

         Goblin::Model3d xAxis;
         Goblin::Model3d yAxis;
         Goblin::Model3d zAxis;

         Goblin::Model3d* selectedAxis;
         DNT::Thing* selectedThing;

         int reference;

   };

}

#endif

