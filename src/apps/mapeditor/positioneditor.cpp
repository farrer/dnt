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
#include "../../core/game.h"
#include "../../map/map.h"
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

   xAxis.getItem()->setCastShadows(false);
   yAxis.getItem()->setCastShadows(false);
   zAxis.getItem()->setCastShadows(false);

   reference = -1;
   selectedThing = NULL;
   selectedLight = NULL;
   selectedAxis = NULL;

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
   selectedLight = NULL;
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
 *                           selectLight                               *
 ***********************************************************************/
void PositionEditor::selectLight(DNT::LightInfo* light)
{
   selectedThing = NULL;
   selectedLight = light;
   if(light)
   {
      setPosition(light->getPosition());
      show();
      DNT::Game::getCurrentMap()->getLights()->setActiveLight(
            selectedLight);
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
 *                                clear                                *
 ***********************************************************************/
void PositionEditor::clear()
{
   selectedAxis = NULL;
   selectedLight = NULL;
   selectedThing = NULL;
   reference = -1;
   hide();
}

/***********************************************************************
 *                           hasSelection                              *
 ***********************************************************************/
bool PositionEditor::hasSelection()
{
   return selectedThing != NULL || selectedLight != NULL;
}

/***********************************************************************
 *                             isMoving                                *
 ***********************************************************************/
bool PositionEditor::isMoving()
{
   return reference != -1;
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
bool PositionEditor::update(bool leftButtonPressed, 
      const Ogre::Vector3& floorMouse, const int mouseY)
{
   if((selectedAxis) && (!leftButtonPressed))
   {
      /* Not pressing, should release the axis selection */
      selectedAxis = NULL;
      reference = -1;
      return true;
   }

   if((selectedAxis) && (leftButtonPressed) && (hasSelection()))
   {
      /* define our current value */
      int curValue;
      if(selectedAxis == &xAxis)
      {
         curValue = floorMouse.x;
      }
      else if(selectedAxis == &yAxis)
      {
         curValue = -mouseY;
      }
      else 
      {
         curValue = floorMouse.z;
      }
      
      /* Check if reference is defined */
      if(reference == -1)
      {
         reference = curValue;
      }
      else if(reference != curValue)
      {
         /* Must move */
         int totalToMove = curValue - reference;

         Ogre::Vector3 curPos;
         if(selectedThing)
         {
            curPos = selectedThing->getModel()->getPosition();
         }
         else if(selectedLight)
         {
            curPos = selectedLight->getPosition();
         }

         if(selectedAxis == &xAxis)
         {
            /* Move on X */
            curPos.x += totalToMove;
         }
         else if(selectedAxis == &yAxis)
         {
            /* Move on Y (with div factor, as not using real world 
             * coordinates to the cursor position) */
            curPos.y += (totalToMove / 2.0f);
         }
         else if(selectedAxis == &zAxis)
         {
            /* Move on Z */
            curPos.z += totalToMove;
         }

         /* Define new position */
         if(selectedThing)
         {
            selectedThing->getModel()->setPosition(curPos);
         }
         else if(selectedLight)
         {
            selectedLight->setPosition(curPos);
         }

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
 *                       updateAxisPosition                            *
 ***********************************************************************/
void PositionEditor::updateAxisPosition()
{
   Ogre::Vector3 curPos;
   if(selectedThing)
   {
      curPos = selectedThing->getModel()->getPosition();
   }
   else if(selectedLight)
   {
      curPos = selectedLight->getPosition();
   }
   setPosition(curPos);
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


