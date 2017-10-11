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

#ifndef _dnt_widget_script_h
#define _dnt_widget_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"

#include <farso/widgeteventlistener.h>

namespace DNT
{
   /*! Instance of a WidgetScript */
   class WidgetScriptInstance : public ScriptInstance, 
                                public Farso::WidgetEventListener
   {
      public:
         /*! Constructor */
         WidgetScriptInstance(asIScriptObject* obj, 
               WidgetScript* script, ScriptManager* manager, 
               ScriptObjectWidget* widget);
         /*! Destructor */
         virtual ~WidgetScriptInstance();

         /*! \return the json definition filename representing 
          * the widget(s) to load */
         Kobold::String getWidgetFilename();

         /*! \return if shoud exit the script instance (ie: the widget is no 
          * longer necessary). */
         bool shouldQuit();

         /*! Call the function to close the widget. */
         void close();

         /*! Call to treat some Farso event on the widget.
          * \param eventType type of the event to treat.
          * \param widgetId widget's identifier. */
         void treatEvent(int eventType, const Kobold::String& widgetId);

         /*! From Farso::WidgetEventListener */
         void onEvent(const Farso::EventType& eventType, Farso::Widget* widget);

      private:
         ScriptObjectWidget* widget; /**< The related widget */
   };

   /*! A script for controlling Farso's widgets defined by a JSON file. */
   class WidgetScript : public ScriptController
   {
      public:
         WidgetScript(ScriptManager* manager,
               ScriptController::ScriptType scriptType = SCRIPT_TYPE_WIDGET);
         virtual ~WidgetScript();

         /*! \return a new instance of the script */
         WidgetScriptInstance* createInstance(ScriptObjectWidget* widget);
         
         /*! \return pointer to the constructor function */
         asIScriptFunction* getFactoryFunction();
         /*! \return pointer to the function called on every cycle */
         asIScriptFunction* getStepFunction();
         /*! \return pointer to the function that defines the json widget */ 
         asIScriptFunction* getWidgetFilenameFunction();
         /*! \return pointer to the function called to treat events */
         asIScriptFunction* getTreatEventFunction();
         /*! \return pointer to the shouldQuit function */
         asIScriptFunction* getShouldQuitFunction();
         /*! \return pointer to the close function */
         asIScriptFunction* getCloseFunction();

      protected:
         virtual void setFunctionPointers();
         virtual const Kobold::String getMainTypeInterface() const 
         { 
            return "WidgetController";
         };
         Kobold::String loadFile(const Kobold::String& filename);

      private:
         asIScriptFunction* factoryFunction;
         asIScriptFunction* stepFunction;
         asIScriptFunction* widgetFilenameFunction;
         asIScriptFunction* treatEventFunction;
         asIScriptFunction* shouldQuitFunction;
         asIScriptFunction* closeFunction;
   };

}

#endif

