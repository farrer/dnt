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
LightInfo::LightInfo(Ogre::Light::LightTypes type)
{
   this->type = type;
   this->direction = Ogre::Vector3(0.0f, -1.0f, 0.0f);
   this->outerAngle = 120.0f;
   this->range = 325;
   this->constant = 1.0;
   this->linear = 0.014; 
   this->quadratic = 0.0007;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
LightInfo::~LightInfo()
{
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
      Ogre::Real linear, Ogre::Real quadratic)
{
   this->range = range;
   this->constant = constant;
   this->linear = linear;
   this->quadratic = quadratic;
}

/**************************************************************************
 *                                setArea                                 *
 **************************************************************************/
void LightInfo::addArea(int x1, int y1, int x2, int y2)
{
   LightInfo::Area* area = new LightInfo::Area();
   area->rect = Farso::Rect(x1, y1, x2, y2);
   areas.insert(area);
}

/**************************************************************************
 *                                isInner                                 *
 **************************************************************************/
bool LightInfo::isInner(int pX, int pZ)
{
   LightInfo::Area* area = static_cast<LightInfo::Area*>(areas.getFirst());
   for(int i = 0; i < areas.getTotal(); i++)
   {
      if(area->rect.isInner(pX, pZ))
      {
         /* Is inner some of our affecting areas */
         return true;
      }
      area = static_cast<LightInfo::Area*>(area->getNext());
   }
   return false;
}

/**************************************************************************
 *                               setLight                                 *
 **************************************************************************/
void LightInfo::setLight(Ogre::Light* light, Ogre::SceneNode* sceneNode)
{
   light->setPowerScale(Ogre::Math::PI); 

   /*light->setShadowNearClipDistance(2.2f);
   light->setShadowFarClipDistance(2200.0f);
   light->setShadowFarDistance(2000.0f);*/
   /* Define common light elements */
   light->setType(type);
   sceneNode->setPosition(position);
   light->setDiffuseColour(diffuse);
   light->setSpecularColour(specular);

   if(type != Ogre::Light::LT_POINT)
   {
      /* Use direction */
      sceneNode->setDirection(direction);
   }
   //else
   {
      /* Use attenuation */
      light->setAttenuation(range, constant, linear, quadratic);
   }

   if(type == Ogre::Light::LT_SPOTLIGHT)
   {
      /* Set spotlight outer angle */
      light->setSpotlightRange(Ogre::Degree(0), Ogre::Degree(outerAngle));
      //FIXME: define radius attenuation by parameter
      //light->setAttenuationBasedOnRadius(10.0f, 0.01f);
   }
}

/**************************************************************************
 *                              MapLights                                 *
 **************************************************************************/
MapLights::MapLights()
{
   light = Game::getSceneManager()->createLight();
   light->setType(Ogre::Light::LT_POINT);

   lightSceneNode = 
      Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
   lightSceneNode->attachObject(light);

   lastX = -1;
   lastZ = -1;
   curLight = NULL;
}

/**************************************************************************
 *                             ~MapLights                                 *
 **************************************************************************/
MapLights::~MapLights()
{
   if(lightSceneNode)
   {
      lightSceneNode->detachObject(light);
      Game::getSceneManager()->destroySceneNode(lightSceneNode);
   }
   if(light)
   {
      Game::getSceneManager()->destroyLight(light);
   }
}

/**************************************************************************
 *                         createLightInfo                                *
 **************************************************************************/
LightInfo* MapLights::createLightInfo(Ogre::Light::LightTypes type)
{
   LightInfo* info = new LightInfo(type);
   insertAtEnd(info);

   return info;
}

/**************************************************************************
 *                             setActiveLight                             *
 **************************************************************************/
void MapLights::setActiveLight(float pX, float pZ)
{
   int iX = Ogre::Math::Floor(pX);
   int iZ = Ogre::Math::Floor(pZ);

   if((iX != lastX) || (iZ != lastZ))
   {
      lastX = iX;
      lastZ = iZ;

      /* Note: assuming that full map is always covered by at least one area. */
      LightInfo* info = static_cast<LightInfo*>(getFirst());
      for(int i = 0; i < getTotal(); i++)
      {
         if(info->isInner(iX, iZ))
         {
            /* Found light, let's see if changed */
            if(curLight != info)
            {
               curLight = info;
               info->setLight(light, lightSceneNode);
            }
            return;
         }
         info = static_cast<LightInfo*>(info->getNext());
      }
      
      /* No light found... must disable current? 
       * For now just using the first one. */
      info = static_cast<LightInfo*>(getFirst());
      if(curLight != info)
      {
         curLight = info;
         info->setLight(light, lightSceneNode);
      }
   }
}


