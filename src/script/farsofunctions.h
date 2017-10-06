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

#ifndef _dnt_farso_functions_h
#define _dnt_farso_functions_h

#include <angelscript.h>
#include <farso/controller.h>
#include <kobold/kstring.h>

namespace DNT
{
   /*! Implementation of some Farso functions to be used on scripts. 
    * The related Farso::Widget is always got by its string identifier. */
   class FarsoFunctions
   {
      public:
         /*! Register all functions */
         static void registerFunctions(asIScriptEngine* asEngine);

         /*! Set text to be displayed when mouse is over a Farso::Widget */
         static void widgetSetMouseHint(const Kobold::String& id,
               const Kobold::String& text);
         /*! Set caption of a Farso::Widget */
         static void widgetSetCaption(const Kobold::String& id,
               const Kobold::String& text);
         /*! Set enable status of a Farso::Widget */
         static void widgetSetEnable(const Kobold::String& id, bool value);
         /*! \return if a Farso::Widget is enabled or not */
         static bool widgetIsEnabled(const Kobold::String& id);
         /*! Set visible status of a Farso::Widget */
         static void widgetSetVisible(const Kobold::String& id, bool value);
         /*! \return if a Farso::Widget is visible or not */
         static bool widgetIsVisible(const Kobold::String& id);
         /*! Remove (ie: close) a root widget from current widgets */
         static void rootWidgetRemove(const Kobold::String& id);

         /*! Set status of a Farso::CheckBox (checked or unchecked) */
         static void checkBoxSet(const Kobold::String& id, bool value);
         /*! \return if a Farso::CheckBox is checked or not */
         static bool checkBoxIsChecked(const Kobold::String& id);

         /*! Set image of a Farso::Picture to the one defined by filename */
         static void pictureSet(const Kobold::String& id, 
               const Kobold::String& filename); 

         /*! Set a Farso::ProgressBar current and max value */
         static void progressBarSet(const Kobold::String& id, 
               int curValue, int maxValue);

         /*! Set Farso::Spin value */
         static void spinSet(const Kobold::String& id, float value);
         /*! \return Farso::Spin integer value */
         static int spinGetInt(const Kobold::String& id);
         /*! \return Farso::Spin float value */
         static float spinGetFloat(const Kobold::String& id);

         /*! \return if a Farso::Window is opened */
         static bool windowIsOpened(const Kobold::String& id);

      private:
         /*! \return pointer to the Widget of type with defined id */
         static Farso::Widget* getWidget(const Kobold::String& id, 
               const Farso::Widget::WidgetType& type);

         /*! \return pointer to the Widget of defined id */
         static Farso::Widget* getWidget(const Kobold::String& id); 
   };

}

#endif


