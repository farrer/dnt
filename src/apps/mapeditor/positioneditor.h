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

         /*! \return if is moving the selection */
         bool isMoving();

         /*! Clear all selctions and hide the editor. Usually called
          * when created a new map or loaded another one */
         void clear();

         /*! Update the editor with input */
         bool update(bool leftButtonPressed, const Ogre::Vector3& floorMouse, 
               const int mouseX, const int mouseY);

         /*! Update axis position to current selected position */
         void updateAxisPosition();

         /*! Define axis visibility, based on current selection */
         void defineAxis();
         
         /*! Check if any position or rotational axis is visible */
         bool isAxisVisible();

      private:

         /*! Hide the translation axis */
         void hideTranslationAxis();
         /*! Show the translation axis */
         void showTranslationAxis();

         /*! Hide the rotation axis */
         void hideRotationAxis();
         /*! Show the rotation axis */
         void showRotationAxis();

         /*! Set axis positions to be relative to */
         void setPosition(Ogre::Vector3 pos);

         /*! Restore selected axis to its original material */
         void restoreAxisMaterial();
         
         /*! Add a value, limiting it */
         Ogre::Real addOnLimit(Ogre::Real value, Ogre::Real addVal);

         Goblin::Model3d xAxis; /**< Model for x axis */
         Goblin::Model3d yAxis; /**< Model for y axis */
         Goblin::Model3d zAxis; /**< Model for z axis */
         
         Goblin::Model3d lightDirAxis; /**< Model for light direction axis */

         Goblin::Model3d xRot; /**< Model for x rotation */
         Goblin::Model3d yRot; /**< Model for y rotation */
         Goblin::Model3d zRot; /**< Model for z rotation */

         Goblin::Model3d* selectedAxis; /**< Model for selected axis */
         Goblin::Model3d* coloredAxis;  /**< Model for colored axis */

         void* lastSelected; /**< Last Thing or Light selected, whose our axis
                                position is relative for */

         float reference; /**< Internal reference value */

   };

}

#endif

