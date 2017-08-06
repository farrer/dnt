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
#include <OGRE/OgreSceneNode.h>

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
         LightInfo(Ogre::Light::LightTypes type, MapLights* lights);
         /*! Destructor */
         ~LightInfo();

         /*! Set light to be exactly like the light info defined.
          * \note both must be of the same type 
          * \param lightInfo pointer to LightInfo to clone its information. */
         void set(LightInfo* lightInfo);

         /*! Change the light type */
         void setType(Ogre::Light::LightTypes type);

         /*! \return Light type of this light */
         const Ogre::Light::LightTypes getType() const { return type; };

         /*! Set light position
          * \param pos new light position */
         void setPosition(Ogre::Vector3 pos);
         /*! \return current lifght position */
         const Ogre::Vector3& getPosition() const { return position; };

         /*! Set light direction
          * \param dir new light direction */
         void setDirection(Ogre::Vector3 dir);
         /*! \return light direction */
         const Ogre::Vector3& getDirection() const { return direction; };

         /*! Set diffuse light color */
         void setDiffuse(float r, float g, float b);
         /*! \return diffuse color */
         const Ogre::ColourValue& getDiffuse() const { return diffuse; };

         /*! Set specular light color */
         void setSpecular(float r, float g, float b);
         /*! \return specular color */
         const Ogre::ColourValue& getSpecular() const { return specular; };

         /*! Set outer angle of the spotlight range */
         void setSpotlightRange(Ogre::Degree outerAngle);
         const Ogre::Degree& getSpotlightRange() const { return outerAngle; };

         /*! Set attenuation factor (only used for point lights) */
         void setAttenuation(Ogre::Real range, Ogre::Real constant,
               Ogre::Real linear, Ogre::Real quadric);
         /*! \return attenuation range */
         const Ogre::Real& getAttenuationRange() const { return range; };
         /*! \return attenuation constant */
         const Ogre::Real& getAttenuationConstant() const { return constant; };
         /*! \return attenuation linear */
         const Ogre::Real& getAttenuationLinear() const { return linear; };
         /*! \return attenuation range */
         const Ogre::Real& getAttenuationQuadric() const { return quadric; };

         /*! Flush the defined parameters to the light */
         void flush();

         /*! Flush the scene node related parameters only */
         void flushToSceneNode();

      private:
         Ogre::Light* ogreLight;  /**< Current active light (Ogre) */
         Ogre::SceneNode* lightSceneNode; /**< Node associated to the light */
         Ogre::ColourValue diffuse;   /**< Diffuse color */
         Ogre::ColourValue specular;  /**< Specular color */

         Ogre::Vector3 position; /**< Light position */
         Ogre::Vector3 direction; /**< Light direction */

         Ogre::Degree outerAngle; /**< Spolight outerAngle */

         Ogre::Light::LightTypes type; /**< Type of the light used */

         MapLights* mapLights; /**< Current lights */

         /* Attenuation values */
         Ogre::Real range,    /**< range value for attenuation */
                    constant, /**< constant value for attenuation */
                    linear,   /**< linear value for attenuation */
                    quadric;  /**< quadric value for attenuation */
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

         /*! Remove a light from the list (and delete its pointer */
         void removeLight(LightInfo* light);

      private:

         LightInfo* curLight; /**< Current active light */

         int lastX;  /**< Last X coordinate of the last check */
         int lastZ;  /**< Last Z coordinate of the last chack */
   };

}

#endif

