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
#include "selection.h"
#include "../../core/game.h"
#include "../../rules/thing.h"
#include "../../map/map.h"
#include "../../map/light.h"
using namespace DNTMapEditor;

/***********************************************************************
 *                           PositionEditor                            *
 ***********************************************************************/
PositionEditor::PositionEditor(Ogre::SceneManager* sceneManager)
        :xAxis("xAxys", "vector.mesh", "mapeditor_models", sceneManager,
              Goblin::Model3d::MODEL_DYNAMIC),
         yAxis("yAxys", "vector.mesh", "mapeditor_models", sceneManager,
               Goblin::Model3d::MODEL_DYNAMIC), 
         zAxis("zAxys", "vector.mesh", "mapeditor_models", sceneManager,
               Goblin::Model3d::MODEL_DYNAMIC),
         lightDirAxis("lightDirAxys", "vector.mesh", "mapeditor_models",
               sceneManager, Goblin::Model3d::MODEL_DYNAMIC),
         xRot("xRot", "rotation_circle.mesh", "mapeditor_models", sceneManager,
               Goblin::Model3d::MODEL_DYNAMIC),
         yRot("yRot", "rotation_circle.mesh", "mapeditor_models", sceneManager,
               Goblin::Model3d::MODEL_DYNAMIC),
         zRot("zRot", "rotation_circle.mesh", "mapeditor_models", sceneManager,
               Goblin::Model3d::MODEL_DYNAMIC)
{
   xAxis.setScaleNow(0.25f, 0.25f, 0.25f);
   yAxis.setScaleNow(0.25f, 0.25f, 0.25f);
   zAxis.setScaleNow(0.25f, 0.25f, 0.25f);

   lightDirAxis.setScaleNow(0.25f, 0.25f, 0.25f);

   xRot.setScaleNow(0.2f, 0.2f, 0.2f);
   yRot.setScaleNow(0.2f, 0.2f, 0.2f);
   zRot.setScaleNow(0.2f, 0.2f, 0.2f);

   yAxis.setMaterial("greenVector");
   yRot.setMaterial("greenVector");
   zAxis.setMaterial("blueVector");
   zRot.setMaterial("blueVector");
   lightDirAxis.setMaterial("yellowVector");

   xAxis.setOrientationNow(0.0f, 0.0f, 270.0f);
   xRot.setOrientationNow(0.0f, 0.0f, 270.0f);
   zAxis.setOrientationNow(0.0f, 90.0f, 90.0f);
   zRot.setOrientationNow(0.0f, 90.0f, 90.0f);

   setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));

   xAxis.getItem()->setCastShadows(false);
   xRot.getItem()->setCastShadows(false);
   yAxis.getItem()->setCastShadows(false);
   yRot.getItem()->setCastShadows(false);
   zAxis.getItem()->setCastShadows(false);
   zRot.getItem()->setCastShadows(false);
   lightDirAxis.getItem()->setCastShadows(false);

   reference = -1;
   selectedAxis = NULL;
   coloredAxis = NULL;
   lastSelected = NULL;

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
 *                          positionEditor                             *
 ***********************************************************************/
void PositionEditor::defineAxis()
{
   if(Selection::getSelectedThing())
   {
      DNT::Thing* thing = Selection::getSelectedThing();
      setPosition(thing->getPosition());
      showTranslationAxis();
      if((thing->getThingType() != DNT::Thing::THING_TYPE_CHARACTER) && 
         (thing->getThingType() != DNT::Thing::THING_TYPE_DOOR))
      {
         /* Use all axis */
         showRotationAxis();
      }
      else
      {
         /* Only use Y rot */
         xRot.hide();
         yRot.show();
         zRot.hide();
      }
      lightDirAxis.hide();
   }
   else if(Selection::getSelectedLight())
   {
      DNT::LightInfo* light = Selection::getSelectedLight();
      if(light->getType() != Ogre::Light::LT_DIRECTIONAL)
      {
         setPosition(light->getPosition());
         showTranslationAxis();
      }
      else
      {
         hideTranslationAxis();
      }

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
   if(isMoving())
   {
     /* Axis is currently selected and moving. Must ignore new selections until
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
   lastSelected = NULL;
   reference = -1;
   hideRotationAxis();
   hideTranslationAxis();
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

   if(Selection::getSelectedThing())
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
   else if(Selection::getSelectedLight())
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
   if(Selection::hasSelection())
   {
      if(!isAxisVisible())
      {
         /* A thing or a light is selected, but no axis is yet visible for it */
         defineAxis();
         if(Selection::getSelectedThing())
         {
            lastSelected = Selection::getSelectedThing();
         } 
         else if(Selection::getSelectedLight())
         {
            lastSelected = Selection::getSelectedLight();
         }
      }
      else if((Selection::getSelectedThing() != NULL) && 
              (Selection::getSelectedThing() != lastSelected))
      {
         /* Selection changed to a different Thing */
         defineAxis();
         lastSelected = Selection::getSelectedThing();
      }
      else if((Selection::getSelectedLight() != NULL) && 
              (Selection::getSelectedLight() != lastSelected))
      {
         /* Selection changed to a different Light */
         defineAxis();
         lastSelected = Selection::getSelectedLight();
      }
   }
   else if(isAxisVisible())
   {
      lastSelected = NULL;
      defineAxis();
   }


   if((selectedAxis) && (!leftButtonPressed))
   {
      /* Not pressing, should release the axis selection */
      selectedAxis = NULL;
      reference = -1;
      return true;
   }

   if((selectedAxis) && (leftButtonPressed) && (Selection::hasSelection()))
   {
      /* define our current value */
      float curValue;
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
         float totalToMove = curValue - reference;

         Ogre::Vector3 curPos;
         Ogre::Vector3 ori;
         if(Selection::getSelectedThing())
         {
            DNT::Thing* thing = Selection::getSelectedThing();
            curPos = thing->getPosition();
            ori.x = thing->getPitch();
            ori.y = thing->getYaw();
            ori.z = thing->getRoll();
         }
         else if(Selection::getSelectedLight())
         {
            DNT::LightInfo* light = Selection::getSelectedLight();
            curPos = light->getPosition();
            ori = light->getDirection();
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
            curPos.y += (totalToMove * 0.025f);
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
         if(Selection::getSelectedThing())
         {
            DNT::Thing* thing = Selection::getSelectedThing();
            thing->setPositionNow(curPos);
            thing->clearOrientation();
            thing->setOrientationNow(
                  Ogre::Vector3(ori.x, ori.y, ori.z));
         }
         else if(Selection::getSelectedLight())
         {
            DNT::LightInfo* light = Selection::getSelectedLight();
            light->setPosition(curPos);
            lightDirAxis.clearOrientation();
            lightDirAxis.getSceneNode()->setDirection(
               ori.normalisedCopy(), Ogre::Node::TS_LOCAL,
               Ogre::Vector3::UNIT_Y);
            light->setDirection(ori.normalisedCopy());
            light->flushToSceneNode();
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
   if(Selection::getSelectedThing())
   {
      curPos = Selection::getSelectedThing()->getPosition();
   }
   else if(Selection::getSelectedLight())
   {
      curPos = Selection::getSelectedLight()->getPosition();
   }
   setPosition(curPos);
}

/***********************************************************************
 *                           setPosition                               *
 ***********************************************************************/
void PositionEditor::setPosition(Ogre::Vector3 pos)
{
   xAxis.setPositionNow(pos.x + 0.5f, pos.y, pos.z);
   xRot.setPositionNow(pos.x + 0.6f, pos.y, pos.z);
   yAxis.setPositionNow(pos.x, pos.y + 0.5f, pos.z);
   yRot.setPositionNow(pos.x, pos.y + 0.6f, pos.z);
   zAxis.setPositionNow(pos.x, pos.y, pos.z + 0.5f);
   zRot.setPositionNow(pos.x, pos.y, pos.z + 0.6f);

   lightDirAxis.setPositionNow(pos);
}

/***********************************************************************
 *                          isAxisVisible                              *
 ***********************************************************************/
bool PositionEditor::isAxisVisible()
{
   return xRot.isVisible() || yRot.isVisible() || zRot.isVisible() ||
      xAxis.isVisible() || yAxis.isVisible() || zAxis.isVisible() ||
      lightDirAxis.isVisible();
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


