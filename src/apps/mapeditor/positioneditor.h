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
#include "../../map/light.h"

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

         /*! \return current selected thing pointer, if any */
         DNT::Thing* getSelectedThing() const { return selectedThing; };

         /*! Select a Light for position edition
          * \note must not be a directional (ie: without position) light. */
         void selectLight(DNT::LightInfo* light);

         /*! \return current selected light */
         DNT::LightInfo* getSelectedLight() const { return selectedLight; };

         /*! \return if a Thing, Wall or Light is selected */
         bool hasSelection();

         /*! \return if is moving the selection */
         bool isMoving();

         /*! Clear all selctions and hide the editor. Usually called
          * when created a new map or loaded another one */
         void clear();

         bool update(bool leftButtonPressed, const Ogre::Vector3& floorMouse, 
               const int mouseX, const int mouseY);

         /*! Update axis position to current selected position */
         void updateAxisPosition();

      private:

         void hide();
         void show();

         /*! Set axis positions to be relative to */
         void setPosition(Ogre::Vector3 pos);

         Goblin::Model3d xAxis;
         Goblin::Model3d yAxis;
         Goblin::Model3d zAxis;

         Goblin::Model3d xRot;
         Goblin::Model3d yRot;
         Goblin::Model3d zRot;

         Goblin::Model3d* selectedAxis;
         DNT::Thing* selectedThing;
         DNT::LightInfo* selectedLight;

         int reference;

   };

}

#endif

