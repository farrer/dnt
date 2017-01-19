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

#include "positioneditor.h"
using namespace DNTMapEditor;

/***********************************************************************
 *                           PositionEditor                            *
 ***********************************************************************/
PositionEditor::PositionEditor(Ogre::SceneManager* sceneManager)
        :xAxis("xAxys", "mapeditor/vector.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC),
         yAxis("yAxys", "mapeditor/vector.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC), 
         zAxis("zAxys", "mapeditor/vector.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC) 
{
   xAxis.setScale(10.0f, 10.0f, 10.0f);
   yAxis.setScale(10.0f, 10.0f, 10.0f);
   zAxis.setScale(10.0f, 10.0f, 10.0f);

   yAxis.setMaterial("greenVector");
   zAxis.setMaterial("blueVector");

   xAxis.setOrientation(0.0f, 0.0f, 270.0f);
   zAxis.setOrientation(0.0f, 90.0f, 90.0f);

   xAxis.setPosition(10.0f, 0.0f, 0.0f);
   yAxis.setPosition(0.0f, 10.0f, 0.0f);
   zAxis.setPosition(0.0f, 0.0f, 10.0f);

   reference = -1;

   hide();
}

/***********************************************************************
 *                           PositionEditor                            *
 ***********************************************************************/
PositionEditor::~PositionEditor()
{
}

/***********************************************************************
 *                           selectThing                               *
 ***********************************************************************/
void PositionEditor::selectThing(DNT::Thing* thing)
{
   selectedThing = thing;
   if(thing)
   {
      setPosition(thing->getModel()->getPosition());
      show();
   }
   else
   {
      hide();
   }
}

/***********************************************************************
 *                             selectAxis                              *
 ***********************************************************************/
bool PositionEditor::selectAxis(Ogre::SceneNode* sceneNode)
{
   if(selectedAxis)
   {
     /* Axis is currently selected. Must ignore new selections until
      * mouse button is released */
     return false;
   }

   if(xAxis.ownSceneNode(sceneNode))
   {
      selectedAxis = &xAxis;
      return true;
   }
   else if(yAxis.ownSceneNode(sceneNode))
   {
      selectedAxis = &yAxis;
      return true;
   }
   else if(zAxis.ownSceneNode(sceneNode))
   {
      selectedAxis = &zAxis;
      return true;
   }

   return false;
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
bool PositionEditor::update(bool leftButtonPressed, int cursorX, int cursorY)
{
   if((selectedAxis) && (!leftButtonPressed))
   {
      /* Not pressing, should release the axis selection */
      selectedAxis = NULL;
      return true;
   }

   if((selectedAxis) && (leftButtonPressed) && (selectedThing))
   {
      int curValue = (selectedAxis == &yAxis) ? cursorY : cursorX;
      if(reference == -1)
      {
         reference = curValue;
      }
      else if(reference != curValue)
      {
         /* Must move */
         int totalToMove = curValue - reference;

         Ogre::Vector3 curPos = selectedThing->getModel()->getPosition();

         if(selectedAxis == &xAxis)
         {
            /* Move on X */
            curPos.x += totalToMove;
         }
         else if(selectedAxis == &yAxis)
         {
            /* Move on Y */
            curPos.y += totalToMove;
         }
         else if(selectedAxis == &zAxis)
         {
            /* Move on Z */
            curPos.z += totalToMove;
         }

         /* Define new thing position */
         selectedThing->getModel()->setPosition(curPos);

         /* Redefine axis position, based on Thing's. */
         setPosition(curPos);

         reference = curValue;
      }
      return true;
   }

   reference = -1;
   return false;
}

/***********************************************************************
 *                           setPosition                               *
 ***********************************************************************/ 
void PositionEditor::setPosition(Ogre::Vector3 pos)
{
   xAxis.setPosition(pos.x + 10.0f, pos.y, pos.z);
   yAxis.setPosition(pos.x, pos.y + 10.0f, pos.z);
   zAxis.setPosition(pos.x, pos.y, pos.z + 10.0f);
}

/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void PositionEditor::hide()
{
   xAxis.hide();
   yAxis.hide();
   zAxis.hide();
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void PositionEditor::show()
{
   xAxis.show();
   yAxis.show();
   zAxis.show();
}


