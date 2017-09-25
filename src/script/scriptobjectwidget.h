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

#ifndef _dnt_script_object_widget_h
#define _dnt_script_object_widget_h

#include "dntconfig.h"
#include <angelscript.h>
#include "scriptobject.h"
#include "widgetscript.h"

namespace DNT
{

   /*! Widget object for script usage */
   class ScriptObjectWidget : public ScriptObject
   {
      public:
         /*! Constructor
          * \param filename name of the WidgetScript file which 
          *                 controls the widget. */
         ScriptObjectWidget(const Kobold::String& filename);
         /*! Destructor */
         ~ScriptObjectWidget();

         /*! Reset the WidgetScript pointer.
          * \note only called on WidgetScript destructor, to avoid 
          *       double free. */
         void setPointer(void* newPtr);

         /*!\ return if the widget is actually valid */ 
         const bool isValid();
         
         /*! Register the related class */
         static void registerClass(asIScriptEngine* asEngine);
         /*! Register the related functions */
         static void registerFunctions(asIScriptEngine* asEngine);

         /*! Factory function.
          * \param filename json file defining the widget.
          * \return a new widget for script usage. */
         static ScriptObjectWidget* factoryWidget(
               const Kobold::String& filename);

      private:
         WidgetScriptInstance* widgetScript; /**< The related widget 
                                                  (controller) */


   };

}


#endif

