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

#ifndef _dnt_map_light_h
#define _dnt_map_light_h

#include "dntconfig.h"

#include <OGRE/OgreLight.h>

#include <kobold/list.h>
#include <kobold/kstring.h>

#include <farso/rect.h>

namespace DNT
{
   
   /*! Keep light information in a map */
   class LightInfo : public Kobold::ListElement
   {
      public:
         /*! Constructor */
         LightInfo(Ogre::Light::LightTypes type);
         /*! Destructor */
         ~LightInfo();

         /*! Set light position
          * \param pos new light position */
         void setPosition(Ogre::Vector3 pos);

         /*! Set light direction
          * \param dir new light direction */
         void setDirection(Ogre::Vector3 dir);

         /*! Set diffuse light color */
         void setDiffuse(float r, float g, float b);

         /*! Set specular light color */
         void setSpecular(float r, float g, float b);

         /*! Set outer angle of the spotlight range */
         void setSpotlightRange(Ogre::Degree outerAngle);

         /*! Add an affected area */
         void addArea(int x1, int y1, int x2, int y2);

         /*! \return if point is inner affected area */
         bool isInner(int pX, int pZ);

         /*! Set an Ogre light to be as defined by this LightInfo */
         void setLight(Ogre::Light* light);

      private:
         class Area : public Kobold::ListElement
         {
            public:
               Farso::Rect rect;  /**< Area where the light will act */
         };
         Ogre::ColourValue diffuse;   /**< Diffuse color */
         Ogre::ColourValue specular;  /**< Specular color */

         Ogre::Vector3 position; /**< Light position */
         Ogre::Vector3 direction; /**< Light direction */

         Ogre::Degree outerAngle; /**< Spolight outerAngle */

         Ogre::Light::LightTypes type; /**< Type of the light used */

         Kobold::List areas; /**< Afftecting areas */
   };

   /*! Class that keeps information about all lights on a map, selecting
    * those active based on current character position */
   class MapLights : public Kobold::List
   {
      public:
         /*! Constructor */
         MapLights();
         /*! Destructor */
         ~MapLights();

         /*! Create a LightInfo for a Map light definition.
          * \param type light type
          * \return pointer to the created LightInfo */
         LightInfo* createLightInfo(Ogre::Light::LightTypes type);

         /*! Set active light for position on map.
          * \param pX X coordinate
          * \param pZ Z coordinate
          * \note if more than one light is defined for the position (ie: 
          *       overlapped areas), the first one is used */
         void setActiveLight(float pX, float pZ);

      private:
         Ogre::Light* light;  /**< Current active light (Ogre) */
         LightInfo* curLight; /**< Current active light */
         Kobold::List lights; /**< Map Lights */

         int lastX;  /**< Last X coordinate of the last check */
         int lastZ;  /**< Last Z coordinate of the last chack */
   };

}

#endif

