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
               Goblin::Model3d::MODEL_DYNAMIC),
         lightDirAxis("lightDirAxys", "mapeditor/vector.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC),
         xRot("xRot", "mapeditor/rotation_circle.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC),
         yRot("yRot", "mapeditor/rotation_circle.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC),
         zRot("zRot", "mapeditor/rotation_circle.mesh", sceneManager, 
               Goblin::Model3d::MODEL_DYNAMIC)
{
   xAxis.setScaleNow(5.0f, 5.0f, 5.0f);
   yAxis.setScaleNow(5.0f, 5.0f, 5.0f);
   zAxis.setScaleNow(5.0f, 5.0f, 5.0f);

   lightDirAxis.setScaleNow(5.0f, 5.0f, 5.0f);

   xRot.setScaleNow(4.0f, 4.0f, 4.0f);
   yRot.setScaleNow(4.0f, 4.0f, 4.0f);
   zRot.setScaleNow(4.0f, 4.0f, 4.0f);

   yAxis.setMaterial("greenVector");
   yRot.setMaterial("greenVector");
   zAxis.setMaterial("blueVector");
   zRot.setMaterial("blueVector");
   lightDirAxis.setMaterial("yellowVector");

   xAxis.setOrientationNow(0.0f, 0.0f, 270.0f);
   xRot.setOrientationNow(0.0f, 0.0f, 270.0f);
   zAxis.setOrientationNow(0.0f, 90.0f, 90.0f);
   zRot.setOrientationNow(0.0f, 90.0f, 90.0f);

   xAxis.setPositionNow(10.0f, 0.0f, 0.0f);
   xRot.setPositionNow(12.0f, 0.0f, 0.0f);
   yAxis.setPositionNow(0.0f, 10.0f, 0.0f);
   yRot.setPositionNow(0.0f, 12.0f, 0.0f);
   zAxis.setPositionNow(0.0f, 0.0f, 10.0f);
   zRot.setPositionNow(0.0f, 0.0f, 12.0f);
   lightDirAxis.setPositionNow(0.0f, 0.0f, 0.0f);

   xAxis.getItem()->setCastShadows(false);
   xRot.getItem()->setCastShadows(false);
   yAxis.getItem()->setCastShadows(false);
   yRot.getItem()->setCastShadows(false);
   zAxis.getItem()->setCastShadows(false);
   zRot.getItem()->setCastShadows(false);
   lightDirAxis.getItem()->setCastShadows(false);

   reference = -1;
   selectedThing = NULL;
   selectedLight = NULL;
   selectedAxis = NULL;
   coloredAxis = NULL;

   hideTranslationAxis();
   hideRotationAxis();
   lightDirAxis.hide();
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
      showTranslationAxis();
      showRotationAxis();
      lightDirAxis.hide();
   }
   else
   {
      hideTranslationAxis();
      hideRotationAxis();
      lightDirAxis.hide();
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
      showTranslationAxis();

      if(light->getType() != Ogre::Light::LT_POINT)
      {
         lightDirAxis.show();
         lightDirAxis.clearOrientation();
         lightDirAxis.getSceneNode()->setDirection(
               light->getDirection(), Ogre::Node::TS_LOCAL,
               Ogre::Vector3::UNIT_Y);
         showRotationAxis();
      }
      else
      {
         lightDirAxis.hide();
         hideRotationAxis();
      }
   }
   else
   {
      hideRotationAxis();
      hideTranslationAxis();
      lightDirAxis.hide();
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
   }
   else if(yAxis.ownSceneNode(sceneNode))
   {
      selectedAxis = &yAxis;
   }
   else if(zAxis.ownSceneNode(sceneNode))
   {
      selectedAxis = &zAxis;
   }
   else if(xRot.ownSceneNode(sceneNode))
   {
      selectedAxis = &xRot;
   }
   else if(yRot.ownSceneNode(sceneNode))
   {
      selectedAxis = &yRot;
   }
   else if(zRot.ownSceneNode(sceneNode))
   {
      selectedAxis = &zRot;
   }

   if(selectedAxis)
   {
      /* Must set it as grey */
      if(coloredAxis)
      {
         restoreAxisMaterial();
      }
      coloredAxis = selectedAxis;
      selectedAxis->setMaterial("greyVector");
   }
   else if(coloredAxis)
   {
      /* No selection and no more under cursor, must unset the
       * grey color on previous axis. */
      restoreAxisMaterial();
   }

   return selectedAxis;
}

/***********************************************************************
 *                         restoreAxisMaterial                         *
 ***********************************************************************/
void PositionEditor::restoreAxisMaterial()
{
   if((coloredAxis == &xAxis) || (coloredAxis == &xRot))
   {
      coloredAxis->setMaterial("redVector");
   }
   else if((coloredAxis == &yAxis) || (coloredAxis == &yRot))
   {
      coloredAxis->setMaterial("greenVector");
   }
   else if((coloredAxis == &zAxis) || (coloredAxis == &zRot))
   {
      coloredAxis->setMaterial("blueVector");
   }
   coloredAxis = NULL;
}

/***********************************************************************
 *                                clear                                *
 ***********************************************************************/
void PositionEditor::clear()
{
   /* Restore material */
   restoreAxisMaterial();

   /* Clear selections */
   selectedAxis = NULL;
   selectedLight = NULL;
   selectedThing = NULL;
   reference = -1;
   hideRotationAxis();
   hideTranslationAxis();
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
 *                            addOnLimit                               *
 ***********************************************************************/
Ogre::Real PositionEditor::addOnLimit(Ogre::Real value, Ogre::Real addVal)
{
   Ogre::Real res = value;

   if(selectedThing)
   {
      res += addVal;
      if(res >= 360.0f)
      {
         res -= 360.0f;
      }
      else if(res < 0.0f)
      {
         res += 360.0f;
      }
   }
   else if(selectedLight)
   {
      res += (addVal / 360.0f);
      if(res > 1.0f)
      {
         res = 1.0f;
      }
      else if(res < -1.0f)
      {
         res = -1.0f;
      }
   }

   return res;
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
bool PositionEditor::update(bool leftButtonPressed, 
      const Ogre::Vector3& floorMouse, const int mouseX, const int mouseY)
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
      else if(selectedAxis == &zAxis)
      {
         curValue = floorMouse.z;
      }
      else if(selectedAxis == &xRot)
      {
         curValue = -mouseY;
      }
      else if(selectedAxis == &yRot)
      {
         curValue = mouseX;
      }
      else if(selectedAxis == &zRot)
      {
         curValue = -mouseY;
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
         Ogre::Vector3 ori;
         if(selectedThing)
         {
            curPos = selectedThing->getModel()->getPosition();
            ori.x = selectedThing->getModel()->getPitch();
            ori.y = selectedThing->getModel()->getYaw();
            ori.z = selectedThing->getModel()->getRoll();
         }
         else if(selectedLight)
         {
            curPos = selectedLight->getPosition();
            ori = selectedLight->getDirection();
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
         else if(selectedAxis == &xRot)
         {
            ori.x = addOnLimit(ori.x, totalToMove);
         }
         else if(selectedAxis == &yRot)
         {
            ori.y = addOnLimit(ori.y, totalToMove);
         }
         else if(selectedAxis == &zRot)
         {
            ori.z = addOnLimit(ori.z, totalToMove);
         }

         /* Define new position */
         if(selectedThing)
         {
            selectedThing->getModel()->setPositionNow(curPos);
            selectedThing->getModel()->clearOrientation();
            selectedThing->getModel()->setOrientationNow(ori.x, ori.y, ori.z);
         }
         else if(selectedLight)
         {
            selectedLight->setPosition(curPos);
            lightDirAxis.clearOrientation();
            lightDirAxis.getSceneNode()->setDirection(
               ori.normalisedCopy(), Ogre::Node::TS_LOCAL,
               Ogre::Vector3::UNIT_Y);
            selectedLight->setDirection(ori.normalisedCopy());
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
   xAxis.setPositionNow(pos.x + 10.0f, pos.y, pos.z);
   xRot.setPositionNow(pos.x + 12.0f, pos.y, pos.z);
   yAxis.setPositionNow(pos.x, pos.y + 10.0f, pos.z);
   yRot.setPositionNow(pos.x, pos.y + 12.0f, pos.z);
   zAxis.setPositionNow(pos.x, pos.y, pos.z + 10.0f);
   zRot.setPositionNow(pos.x, pos.y, pos.z + 12.0f);

   lightDirAxis.setPositionNow(pos);
}

/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void PositionEditor::hideRotationAxis()
{
   xRot.hide();
   yRot.hide();
   zRot.hide();
}

/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void PositionEditor::hideTranslationAxis()
{
   xAxis.hide();
   yAxis.hide();
   zAxis.hide();
}


/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void PositionEditor::showRotationAxis()
{
   xRot.show();
   yRot.show();
   zRot.show();
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void PositionEditor::showTranslationAxis()
{
   xAxis.show();
   yAxis.show();
   zAxis.show();
}


