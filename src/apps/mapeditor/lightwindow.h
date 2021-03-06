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

#ifndef _dnt_map_editor_light_window_h
#define _dnt_map_editor_light_window_h

#include <farso/controller.h>
#include <map>
#include "positioneditor.h"
#include "vector3textentry.h"
#include "attenuationtextentry.h"

#include "../../map/light.h"

namespace DNTMapEditor
{ 
   class MainGui;

   /*! A window to direct define light attributes. */
   class LightWindow
   {
      public:
         /*! Constructor */
         LightWindow(MainGui* mainGui);
         /*! Destructor */
         ~LightWindow();

         /*! Open window (closing it if already opened before 
          *  opening again). */
         void open();

         /*! Close the window, if opened */
         void close();

         /*! \return if window is opened */
         const bool isOpened() const { return window != NULL; };

         /*! Check current Farso::Events for the window 
          * \return true if treated some event. */
         bool checkEvents(PositionEditor* positionEditor);

         /*! Update window with the values from current selected */
         void update();

      private:
         /*! Set enable status.
          * \param enable if enabled or not */
         void setEnabled(bool enable);

         /*! Update current transform texts with ones defined by the light */
         void updateTexts(DNT::LightInfo* light);
         /*! Disable point light */
         void disablePoint();
         /*! Enable point light */
         void enablePoint();
         /*! Disable spot light */
         void disableSpot();
         /*! Enable spot light */
         void enableSpot();
         /*! Disable directional light */
         void disableDirectional();
         /*! Enable directional light */
         void enableDirectional();
         /*! Create a new light and select it for editing */
         void createLight();

         Farso::Window* window; /**< The window */
         Vector3TextEntry* diffuse; /**< Light diffuse color */
         Vector3TextEntry* specular; /**< Light specular color */
         Farso::TextEntry* powerScale; /**< Light power scale */
         Farso::CheckBox* hdr; /**< If light is in HDR or LDR */
         Farso::CheckBox* castShadows; /**< If light should cast shadows */
         Farso::Button* addNewLight; /**< Button for add a new light */
         Farso::Button* deleteLight; /**< Button to delete a light */

         Farso::StackTab* tab; /**< The light-type tab */

         /* Point light */
         Farso::Container* pointCont; /**< point light parameters container */
         Farso::CheckBox* usePoint; /**< if the light is a point one */
         Vector3TextEntry* pointPosition; /**< point light position */
         AttenuationTextEntry* pointAttenuation; /**< point light attenuation */

         /* Spot light */
         Farso::Container* spotCont; /**< spot light parameters container */
         Farso::CheckBox* useSpot; /**< if the light is a spot one */
         Vector3TextEntry* spotPosition; /**< spot light position */
         Vector3TextEntry* spotDirection; /**< spot light direction */
         AttenuationTextEntry* spotAttenuation; /**< spot light attenuation */
         Farso::TextEntry* spotOuterAngle; /**< spot light outer angle */

         /* Directional light */
         Farso::Container* direcCont; /**< directional light param container */
         Farso::CheckBox* useDirec; /**< if the light is a directional one */
         Vector3TextEntry* direcDirection; /**< directional light dir */

         MainGui* mainGui;
   };

}

#endif

