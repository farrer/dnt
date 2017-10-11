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

#ifndef _dnt_dialog_widget_script_h
#define _dnt_dialog_widget_script_h

#include "dntconfig.h"
#include "widgetscript.h"

namespace DNT
{

   /*! Instance of a DialogWidgetScript */
   class DialogWidgetScriptInstance : public WidgetScriptInstance
   {
      public:
         /*! Constructor */
         DialogWidgetScriptInstance(asIScriptObject* obj, 
                  WidgetScript* script, ScriptManager* manager); 
         /*! Destructor */
         virtual ~DialogWidgetScriptInstance();

         /*! \return identifier of the root widget defined for the dialog.
          *       This will be the entry point for closing a dialog screen. */
         Kobold::String getRootWidgetId();

         /*! Clear all dialog options and owner text.
          * \note should keep the picture, if any, or any other owner related
          *       information. Those should only change at setOwner. */
         void clear();
         /*! Add a new dialog option text.
          * \param text String with option's text
          * \param index its selection index [0,n) */
         void addOption(const Kobold::String& text, int index);
         /*! Set the current owner displayed text 
          * \param text to set */
         void setOwnerText(const Kobold::String& text);

         /*! Set current dialog owner as a Object */
         void setOwner(Thing* owner);

         /*! Select an option.
          * \note This is called when tried to select an option by keyboard.
          *       Note that the option index could be invalid (ie: greater
          *       than the maximun available options 
          * \param index option index [0, 9] */
         void selectOption(int index);
         /*! \return the current selected option, if any (if not, return -1) */
         int getSelectedOption();
   };

   /*! Script for controlling a dialog (conversation) widget */
   class DialogWidgetScript : public WidgetScript
   {
      public:
         DialogWidgetScript(ScriptManager* manager);
         virtual ~DialogWidgetScript();

         /*! \return a new instance of the script */
         DialogWidgetScriptInstance* createInstance();
         
         /*! \return pointer to getRootWidgetId function */
         asIScriptFunction* getRootWidgetIdFunction();
         /*! \return pointer to clear function */
         asIScriptFunction* getClearFunction();
         /*! \return pointer to addOption */
         asIScriptFunction* getAddOptionFunction();
         /*! \return pointer to setOwnerText */
         asIScriptFunction* getSetOwnerTextFunction();
         /*! \return pointer to setOwner */
         asIScriptFunction* getSetOwnerFunction();
         asIScriptFunction* getSelectOptionFunction();
         asIScriptFunction* getGetSelectedOptionFunction();

         /*! \return the loaded json file representing the widget */
         const Kobold::String& getJson() { return jsonStr; };

      protected:
         virtual void setFunctionPointers();
         virtual const Kobold::String getMainTypeInterface() const 
         { 
            return "DialogWidgetController";
         };

      private:
         asIScriptFunction* rootWidgetIdFunction;
         asIScriptFunction* clearFunction;
         asIScriptFunction* addOptionFunction;
         asIScriptFunction* setOwnerTextFunction;
         asIScriptFunction* setOwnerFunction;
         asIScriptFunction* selectOptionFunction;
         asIScriptFunction* getSelectedOptionFunction;
         Kobold::String jsonStr;
   };

}

#endif

