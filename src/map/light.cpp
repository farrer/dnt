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

#include "light.h"

#include "../core/game.h"

#include <OGRE/OgreMath.h>

using namespace DNT;

/**************************************************************************
 *                                Constructor                             *
 **************************************************************************/
LightInfo::LightInfo(Ogre::Light::LightTypes type, MapLights* lights)
{
   this->type = type;
   this->mapLights = lights;
   this->direction = Ogre::Vector3(0.0f, -1.0f, 0.0f);
   this->outerAngle = Ogre::Degree(120.0f);
   this->range = 325;
   this->constant = 1.0;
   this->linear = 0.014; 
   this->quadric = 0.0007;

   ogreLight = Game::getSceneManager()->createLight();
   lightSceneNode = 
      Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
   lightSceneNode->attachObject(ogreLight);
   flush();
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
LightInfo::~LightInfo()
{
   if(lightSceneNode)
   {
      lightSceneNode->detachObject(ogreLight);
      Game::getSceneManager()->destroySceneNode(lightSceneNode);
   }
   if(ogreLight)
   {
      Game::getSceneManager()->destroyLight(ogreLight);
   }

}

/**************************************************************************
 *                                   set                                  *
 **************************************************************************/
void LightInfo::set(LightInfo* lightInfo)
{
   assert(type == lightInfo->type);
   if(type == lightInfo->type)
   {
      /* Clone parameters */
      this->position = lightInfo->position;
      this->direction = lightInfo->direction;
      this->diffuse = lightInfo->diffuse;
      this->specular = lightInfo->specular;
      this->outerAngle = lightInfo->outerAngle;
      this->range = lightInfo->range;
      this->constant = lightInfo->constant;
      this->linear = lightInfo->linear;
      this->quadric = lightInfo->quadric;
   }
}

/**************************************************************************
 *                                 setType                                *
 **************************************************************************/
void LightInfo::setType(Ogre::Light::LightTypes type)
{
   this->type = type;
}

/**************************************************************************
 *                               setPosition                              *
 **************************************************************************/
void LightInfo::setPosition(Ogre::Vector3 pos)
{
   this->position = pos;
}

/**************************************************************************
 *                               setDirection                             *
 **************************************************************************/
void LightInfo::setDirection(Ogre::Vector3 dir)
{
   this->direction = dir;
   this->direction.normalise();
}

/**************************************************************************
 *                               setDiffuse                               *
 **************************************************************************/
void LightInfo::setDiffuse(float r, float g, float b)
{
   this->diffuse = Ogre::ColourValue(r, g, b);
}

/**************************************************************************
 *                               setSpecular                              *
 **************************************************************************/
void LightInfo::setSpecular(float r, float g, float b)
{
   this->specular = Ogre::ColourValue(r, g, b);
}

/**************************************************************************
 *                           setSpolightRange                             *
 **************************************************************************/
void LightInfo::setSpotlightRange(Ogre::Degree outerAngle)
{
   this->outerAngle = outerAngle;
}

/**************************************************************************
 *                            setAttenuation                              *
 **************************************************************************/
void LightInfo::setAttenuation(Ogre::Real range, Ogre::Real constant,
      Ogre::Real linear, Ogre::Real quadric)
{
   this->range = range;
   this->constant = constant;
   this->linear = linear;
   this->quadric = quadric;
}

/**************************************************************************
 *                         flushToSceneNode                               *
 **************************************************************************/
void LightInfo::flushToSceneNode()
{
   lightSceneNode->resetOrientation();
   if(type != Ogre::Light::LT_DIRECTIONAL)
   {
      lightSceneNode->setPosition(position);
   }
   if(type != Ogre::Light::LT_POINT)
   {
      /* Use direction */
      lightSceneNode->setDirection(direction);
   }
}

/**************************************************************************
 *                                flush                                   *
 **************************************************************************/
void LightInfo::flush()
{
   //FIXME: PI used to work before update to current 
   //       2.1 commit on 2 july 2017.
   //light->setPowerScale(Ogre::Math::PI); 
   ogreLight->setPowerScale(50); 

   /*light->setShadowNearClipDistance(2.2f);
   light->setShadowFarClipDistance(2200.0f);
   light->setShadowFarDistance(2000.0f);*/
   /* Define common light elements */
   ogreLight->setType(type);
   ogreLight->setDiffuseColour(diffuse);
   ogreLight->setSpecularColour(specular);

   if(type != Ogre::Light::LT_DIRECTIONAL)
   {
      /* Define attenuation */
      ogreLight->setAttenuation(range, constant, linear, quadric);
   }

   if(type == Ogre::Light::LT_SPOTLIGHT)
   {
      /* Set spotlight outer angle */
      ogreLight->setSpotlightRange(Ogre::Degree(0), outerAngle);
   }

   flushToSceneNode();
}

/**************************************************************************
 *                              MapLights                                 *
 **************************************************************************/
MapLights::MapLights()
{
   lastX = -1;
   lastZ = -1;
   curLight = NULL;
}

/**************************************************************************
 *                             ~MapLights                                 *
 **************************************************************************/
MapLights::~MapLights()
{
}

/**************************************************************************
 *                         createLightInfo                                *
 **************************************************************************/
LightInfo* MapLights::createLightInfo(Ogre::Light::LightTypes type)
{
   LightInfo* info = new LightInfo(type, this);
   insertAtEnd(info);

   return info;
}

/**************************************************************************
 *                               removeLight                              *
 **************************************************************************/
void MapLights::removeLight(LightInfo* light)
{
   remove(light);
}

