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

#include "lightwindow.h"
using namespace DNTMapEditor;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
LightWindow::LightWindow()
{
   window = NULL;
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
LightWindow::~LightWindow()
{
}

/************************************************************************
 *                                 open                                 *
 ************************************************************************/
void LightWindow::open()
{
   if(!window)
   {
      window = new Farso::Window(320, 210, "Light");
      window->setExternPointer(&window);

      /* Create light color texts */
      diffuse = new Vector3TextEntry(0, 0, Vector3TextEntry::TYPE_RGB,
            "Diffuse", window);
      specular = new Vector3TextEntry(0, 22, Vector3TextEntry::TYPE_RGB,
            "Specular", window);

      /* Create the light type stack tab */
      Farso::Container* tabCont = new Farso::Container(
            Farso::Container::TYPE_TOP_LEFT, 
            Farso::Rect(0, 44, 0, 1), window);
      tab = new Farso::StackTab(tabCont);
      
      /* Point light */
      pointCont = tab->insertTab("Point");
      usePoint = new Farso::CheckBox(0, 0, 100, "Enabled", false, pointCont);
      pointPosition = new Vector3TextEntry(0, 22, Vector3TextEntry::TYPE_AXIS,
            "Position", pointCont);
      pointAttenuation = new AttenuationTextEntry(0, 44, pointCont);

      /* Spot light */
      spotCont = tab->insertTab("Spot");
      useSpot = new Farso::CheckBox(0, 0, 100, "Enabled", false, spotCont);
      new Farso::Label(169, 0, 75, 21, "Outer angle:", spotCont);
      spotOuterAngle = new Farso::TextEntry(244, 0, 60, 21, spotCont);
      spotPosition = new Vector3TextEntry(0, 22, Vector3TextEntry::TYPE_AXIS,
            "Position", spotCont);
      spotDirection = new Vector3TextEntry(0, 44, Vector3TextEntry::TYPE_AXIS,
            "Direction", spotCont);
      spotAttenuation = new AttenuationTextEntry(0, 66, spotCont);

      /* Directional light */
      direcCont = tab->insertTab("Directional");
      useDirec = new Farso::CheckBox(0, 0, 100, "Enabled", false, direcCont);
      direcDirection = new Vector3TextEntry(0, 22, Vector3TextEntry::TYPE_AXIS,
            "Direction", direcCont);
      
      /* Open and set its position */
      window->open();
      window->setPosition(Farso::Controller::getWidth() - 320, 315);
   }
}

/************************************************************************
 *                                close                                 *
 ************************************************************************/
void LightWindow::close()
{
   if(window)
   {
      window->close();
   }
}

/************************************************************************
 *                             setEnabled                               *
 ************************************************************************/
void LightWindow::setEnabled(bool enable)
{
   if((enable) && (!diffuse->isAvailable()))
   {
      /* Should enable to edit */
      specular->enable();
      diffuse->enable();
      usePoint->enable();
      enablePoint();
      useSpot->enable();
      enableSpot();
      useDirec->enable();
      enableDirectional();
   }
   else if((!enable) && (diffuse->isAvailable()))
   {
      /* Should disable the edition */
      specular->disable();
      diffuse->disable();
      usePoint->disable();
      disablePoint();
      useSpot->disable();
      disableSpot();
      useDirec->disable();
      disableDirectional();
   }
}

/************************************************************************
 *                           disabledPoint                              *
 ************************************************************************/
void LightWindow::disablePoint()
{
   usePoint->uncheck();
   pointPosition->disable();
   pointAttenuation->disable();
}

/************************************************************************
 *                            enabledPoint                              *
 ************************************************************************/
void LightWindow::enablePoint()
{
   pointPosition->enable();
   pointAttenuation->enable();
}

/************************************************************************
 *                            disabledSpot                              *
 ************************************************************************/
void LightWindow::disableSpot()
{
   useSpot->uncheck();
   spotPosition->disable();
   spotDirection->disable();
   spotOuterAngle->disable();
   spotAttenuation->disable();
}
 
/************************************************************************
 *                            enabledSpot                               *
 ************************************************************************/
void LightWindow::enableSpot()
{
   spotPosition->enable();
   spotDirection->enable();
   spotOuterAngle->enable();
   spotAttenuation->enable();
}

/************************************************************************
 *                        disabledDirectional                           *
 ************************************************************************/
void LightWindow::disableDirectional()
{
   useDirec->uncheck();
   direcDirection->disable();
}

/************************************************************************
 *                         enabledDirectional                           *
 ************************************************************************/
void LightWindow::enableDirectional()
{
   direcDirection->enable();
}

/************************************************************************
 *                             updateTexts                              *
 ************************************************************************/
void LightWindow::updateTexts(DNT::LightInfo* light)
{
   diffuse->setValue(light->getDiffuse());
   specular->setValue(light->getSpecular());
   if(light->getType() == Ogre::Light::LT_POINT)
   {
      disableSpot();
      disableDirectional();
      enablePoint();
      usePoint->check();
      pointPosition->setValue(light->getPosition());
      pointAttenuation->setValue(light->getAttenuationRange(), 
            light->getAttenuationConstant(), light->getAttenuationLinear(),
            light->getAttenuationQuadric());
   }
   else if(light->getType() == Ogre::Light::LT_SPOTLIGHT)
   {
      disablePoint();
      disableDirectional();
      enableSpot();
      useSpot->check();
      spotPosition->setValue(light->getPosition());
      spotDirection->setValue(light->getDirection());
      spotAttenuation->setValue(light->getAttenuationRange(), 
            light->getAttenuationConstant(), light->getAttenuationLinear(),
            light->getAttenuationQuadric());
      spotOuterAngle->setCaption(Ogre::StringConverter::toString(
               light->getSpotlightRange()));
   }
   else if(light->getType() == Ogre::Light::LT_DIRECTIONAL)
   {
      disablePoint();
      disableSpot();
      enableDirectional();
      useDirec->check();
      direcDirection->setValue(light->getDirection());
   }
}

/************************************************************************
 *                               update                                 *
 ************************************************************************/
void LightWindow::update(PositionEditor* positionEditor)
{
   if(window)
   {
      /* Check if enabled or disabled */
      setEnabled(positionEditor->getSelectedLight() != NULL);
      if(positionEditor->getSelectedLight())
      {
         /* Let's update current transforms */
         updateTexts(positionEditor->getSelectedLight());
      }
   }
}

/************************************************************************
 *                             checkEvents                              *
 ************************************************************************/
bool LightWindow::checkEvents(PositionEditor* positionEditor)
{
   if((window) && (window->isActive()) && (positionEditor->getSelectedLight()))
   {
      DNT::LightInfo* light = positionEditor->getSelectedLight();
      Farso::Event event = Farso::Controller::getLastEvent();
      if(event.getType() == Farso::EVENT_TEXTENTRY_EDITION_DONE)
      {
         if(event.getWidget() == spotOuterAngle)
         {
            /* parse the outer angle and set it. */
            light->setSpotlightRange(Ogre::Degree(
                     Ogre::StringConverter::parseReal(
                        spotOuterAngle->getCaption(), 
                        light->getSpotlightRange().valueDegrees())));
            light->flush();
         }
      }
      else if(event.getType() == Farso::EVENT_CHECKBOX_CHECKED)
      {
         if(event.getWidget() == usePoint)
         {
            light->setType(Ogre::Light::LT_POINT);
            updateTexts(light);
            light->flush();
         }
         else if(event.getWidget() == useSpot)
         {
            light->setType(Ogre::Light::LT_SPOTLIGHT);
            updateTexts(light);
            light->flush();
         }
         else if(event.getWidget() == useDirec)
         {
            light->setType(Ogre::Light::LT_DIRECTIONAL);
            updateTexts(light);
            light->flush();
         }
      }
      else if(event.getType() == Farso::EVENT_USER_DEFINED)
      {
         if(event.getWidget() == diffuse)
         {
            Ogre::Vector3 dif = diffuse->getValue();
            light->setDiffuse(dif[0], dif[1], dif[2]);
            light->flush();
         }
         else if(event.getWidget() == specular)
         {
            Ogre::Vector3 sp = specular->getValue();
            light->setSpecular(sp[0], sp[1], sp[2]);
            light->flush();
         }
         else if(event.getWidget() == pointPosition)
         {
            light->setPosition(pointPosition->getValue());
            light->flushToSceneNode();
         }
         else if(event.getWidget() == pointAttenuation)
         {
            light->setAttenuation(pointAttenuation->getRange(), 
                  pointAttenuation->getConstant(), 
                  pointAttenuation->getLinear(), 
                  pointAttenuation->getQuadric());
            light->flush();
         }
         else if(event.getWidget() == spotPosition)
         {
            light->setPosition(spotPosition->getValue());
            light->flushToSceneNode();
         }
         else if(event.getWidget() == spotDirection)
         {
            light->setDirection(spotDirection->getValue());
            light->flushToSceneNode();
         }
         else if(event.getWidget() == spotAttenuation)
         {
            light->setAttenuation(spotAttenuation->getRange(), 
                  spotAttenuation->getConstant(), 
                  spotAttenuation->getLinear(), 
                  spotAttenuation->getQuadric());
            light->flush();
         }
         else if(event.getWidget() == direcDirection)
         {
            light->setDirection(direcDirection->getValue());
            light->flush();
         }
      }
   }

   return false;
}

