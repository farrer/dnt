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

#include "farsofunctions.h"
#include <kobold/log.h>
using namespace DNT;

/************************************************************************
 *                          registerFunctions                           *
 ************************************************************************/
void FarsoFunctions::registerFunctions(asIScriptEngine* asEngine)
{
   int r;

   /* Common Widget functions */
   r = asEngine->RegisterGlobalFunction(
         "void widgetSetMouseHint(string, string)",
         asFUNCTION(FarsoFunctions::widgetSetMouseHint), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "void widgetSetCaption(string, string)",
         asFUNCTION(FarsoFunctions::widgetSetCaption), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void widgetSetEnable(string, bool)",
         asFUNCTION(FarsoFunctions::widgetSetEnable), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("bool widgetIsEnabled(string)",
         asFUNCTION(FarsoFunctions::widgetIsEnabled), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void widgetSetVisible(string, bool)",
         asFUNCTION(FarsoFunctions::widgetSetVisible), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("bool widgetIsVisible(string)",
         asFUNCTION(FarsoFunctions::widgetIsVisible), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void rootWidgetRemove(string)",
         asFUNCTION(FarsoFunctions::rootWidgetRemove), asCALL_CDECL); 
   assert(r >= 0);

   /* CheckBox functions */
   r = asEngine->RegisterGlobalFunction("void checkBoxSet(string, bool)",
         asFUNCTION(FarsoFunctions::checkBoxSet), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("bool checkBoxIsChecked(string)",
         asFUNCTION(FarsoFunctions::checkBoxIsChecked), asCALL_CDECL); 
   assert(r >= 0);

   /* Picture functions */
   r = asEngine->RegisterGlobalFunction("void pictureSet(string, string)",
         asFUNCTION(FarsoFunctions::pictureSet), asCALL_CDECL); 
   assert(r >= 0);

   /* ProgressBar functions */
   r = asEngine->RegisterGlobalFunction("void progressBarSet(string, int, int)",
         asFUNCTION(FarsoFunctions::progressBarSet), asCALL_CDECL); 
   assert(r >= 0);

   /* TextSelector functions */
   r = asEngine->RegisterGlobalFunction(
         "void textSelectorAddOption(string, string, int)",
         asFUNCTION(FarsoFunctions::textSelectorAddOption), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void textSelectorClear(string)",
         asFUNCTION(FarsoFunctions::textSelectorClear), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "int textSelectorGetSelected(string)",
         asFUNCTION(FarsoFunctions::textSelectorGetSelected), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction(
         "bool textSelectorHaveOption(string, int)",
         asFUNCTION(FarsoFunctions::textSelectorHaveOption), asCALL_CDECL); 
   assert(r >= 0);

   /* ScrollText functions */
   r = asEngine->RegisterGlobalFunction("void scrollTextSet(string, string)",
         asFUNCTION(FarsoFunctions::scrollTextSet), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("void scrollTextClear(string)",
         asFUNCTION(FarsoFunctions::scrollTextClear), asCALL_CDECL); 
   assert(r >= 0);

   /* Spin functions */
   r = asEngine->RegisterGlobalFunction("void spinSet(string, float)",
         asFUNCTION(FarsoFunctions::spinSet), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("int spinGetInt(string)",
         asFUNCTION(FarsoFunctions::spinGetInt), asCALL_CDECL); 
   assert(r >= 0);
   r = asEngine->RegisterGlobalFunction("float spinGetFloat(string)",
         asFUNCTION(FarsoFunctions::spinGetFloat), asCALL_CDECL); 
   assert(r >= 0);

   /* Window functions */
   r = asEngine->RegisterGlobalFunction("bool windowIsOpened(string)",
         asFUNCTION(FarsoFunctions::windowIsOpened), asCALL_CDECL);
   assert(r >= 0);


   /* Add events constants */
   r = asEngine->RegisterEnum("FarsoEvent");
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_NONE", 
         Farso::EVENT_NONE);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_USER_DEFINED", 
         Farso::EVENT_USER_DEFINED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_WINDOW_MOVE_INIT",
         Farso::EVENT_WINDOW_MOVE_INIT);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_WINDOW_MOVING", 
         Farso::EVENT_WINDOW_MOVING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_WINDOW_MOVE_END", 
         Farso::EVENT_WINDOW_MOVE_END);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_WINDOW_WILL_ACTIVATE", Farso::EVENT_WINDOW_WILL_ACTIVATE);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_WINDOW_ACTIVATED",
         Farso::EVENT_WINDOW_ACTIVATED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_WINDOW_CLICKED", 
         Farso::EVENT_WINDOW_CLICKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_WINDOW_WILL_CLOSE", Farso::EVENT_WINDOW_WILL_CLOSE);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_BUTTON_PRESS_INIT", Farso::EVENT_BUTTON_PRESS_INIT);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_BUTTON_PRESSING", 
         Farso::EVENT_BUTTON_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_BUTTON_NOT_PRESSING", Farso::EVENT_BUTTON_NOT_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_BUTTON_PRESS_CANCELLED", 
         Farso::EVENT_BUTTON_PRESS_CANCELLED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_BUTTON_RELEASED", 
         Farso::EVENT_BUTTON_RELEASED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_LABEL_CLICKED", 
         Farso::EVENT_LABEL_CLICKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_CHECKBOX_PRESSING", Farso::EVENT_CHECKBOX_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_CHECKBOX_CHECKED",
         Farso::EVENT_CHECKBOX_CHECKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_CHECKBOX_UNCHECKED", Farso::EVENT_CHECKBOX_UNCHECKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TEXTSELECTOR_OPTION_SELECTED", 
         Farso::EVENT_TEXTSELECTOR_OPTION_SELECTED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_SCROLLBAR_CHANGED", Farso::EVENT_SCROLLBAR_CHANGED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_SCROLLBAR_UNCHANGED", 
         Farso::EVENT_SCROLLBAR_UNCHANGED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_STACKTAB_CHANGED",
         Farso::EVENT_STACKTAB_CHANGED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_STACKTAB_UNCHANGED", Farso::EVENT_STACKTAB_UNCHANGED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TEXTENTRY_EDITION_START", 
         Farso::EVENT_TEXTENTRY_EDITION_START);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TEXTENTRY_EDITING", Farso::EVENT_TEXTENTRY_EDITING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TEXTENTRY_EDITION_DONE", 
         Farso::EVENT_TEXTENTRY_EDITION_DONE);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_GRID_CLICKING_ELEMENT", 
         Farso::EVENT_GRID_CLICKING_ELEMENT);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_GRID_SELECTED_ELEMENT", 
         Farso::EVENT_GRID_SELECTED_ELEMENT);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_FILESELECTOR_ACCEPT", 
         Farso::EVENT_FILESELECTOR_ACCEPT);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_FILESELECTOR_CANCEL", 
         Farso::EVENT_FILESELECTOR_CANCEL);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_FILESELECTOR_SELECTED_FILE", 
         Farso::EVENT_FILESELECTOR_SELECTED_FILE);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_FILESELECTOR_SELECTED_DIRECTORY", 
         Farso::EVENT_FILESELECTOR_SELECTED_DIRECTORY);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_MENU_PRESSING", 
         Farso::EVENT_MENU_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_MENU_SELECTED", 
         Farso::EVENT_MENU_SELECTED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_MENU_CLOSED", 
         Farso::EVENT_MENU_CLOSED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_MENU_OPENED", 
         Farso::EVENT_MENU_OPENED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_LABELLEDPICTURE_PRESSING", 
         Farso::EVENT_LABELLEDPICTURE_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_LABELLEDPICTURE_CLICKED", 
         Farso::EVENT_LABELLEDPICTURE_CLICKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_CLICKABLEPICTURE_PRESSING", 
         Farso::EVENT_CLICKABLEPICTURE_PRESSING);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_CLICKABLEPICTURE_CLICKED", 
         Farso::EVENT_CLICKABLEPICTURE_CLICKED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", "FARSO_EVENT_SPIN_CHANGED", 
         Farso::EVENT_SPIN_CHANGED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TREEVIEW_EXPANDED", 
         Farso::EVENT_TREEVIEW_EXPANDED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TREEVIEW_COLLAPSED", 
         Farso::EVENT_TREEVIEW_COLLAPSED);
   assert(r >= 0);
   r = asEngine->RegisterEnumValue("FarsoEvent", 
         "FARSO_EVENT_TREEVIEW_SELECTED", 
         Farso::EVENT_TREEVIEW_SELECTED);
   assert(r >= 0);
}

/************************************************************************
 *                           widgetSetMouseHint                         *
 ************************************************************************/
void FarsoFunctions::widgetSetMouseHint(const Kobold::String& id,
      const Kobold::String& text)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      widget->setMouseHint(text);
   }
}

/************************************************************************
 *                            widgetSetCaption                          *
 ************************************************************************/
void FarsoFunctions::widgetSetCaption(const Kobold::String& id,
      const Kobold::String& text)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      widget->setCaption(text);
   }
}

/************************************************************************
 *                            widgetSetEnable                           *
 ************************************************************************/
void FarsoFunctions::widgetSetEnable(const Kobold::String& id,
      bool value)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      value ? widget->enable() : widget->disable();
   }
}

/************************************************************************
 *                            widgetIsEnabled                           *
 ************************************************************************/
bool FarsoFunctions::widgetIsEnabled(const Kobold::String& id)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      return widget->isEnabled();
   }

   return false;
}

/************************************************************************
 *                           widgetSetVisible                           *
 ************************************************************************/
void FarsoFunctions::widgetSetVisible(const Kobold::String& id,
      bool value)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      value ? widget->show() : widget->hide();
   }
}

/************************************************************************
 *                            widgetIsVisible                           *
 ************************************************************************/
bool FarsoFunctions::widgetIsVisible(const Kobold::String& id)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      return widget->isVisible();
   }

   return false;
}

/************************************************************************
 *                           rootWidgetRemove                           *
 ************************************************************************/
void FarsoFunctions::rootWidgetRemove(const Kobold::String& id)
{
   Farso::Widget* widget = getWidget(id);
   if(widget)
   {
      if(widget->getParent() == NULL)
      {
         Farso::Controller::markToRemoveWidget(widget);
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
            "Error: rootWidgetRemove function is only valid for root widgets."
            " '%s' isn't a root one.", id.c_str());
      }
   }
}

/************************************************************************
 *                             checkBoxSet                              *
 ************************************************************************/
void FarsoFunctions::checkBoxSet(const Kobold::String& id, bool value)
{
   Farso::CheckBox* checkBox = static_cast<Farso::CheckBox*>(getWidget(id,
            Farso::Widget::WIDGET_TYPE_CHECK_BOX));
   if(checkBox)
   {
      value ? checkBox->check() : checkBox->uncheck();
   }
}

/************************************************************************
 *                           checkBoxIsChecked                          *
 ************************************************************************/
bool FarsoFunctions::checkBoxIsChecked(const Kobold::String& id)
{
   Farso::CheckBox* checkBox = static_cast<Farso::CheckBox*>(getWidget(id,
            Farso::Widget::WIDGET_TYPE_CHECK_BOX));
   if(checkBox)
   {
      return checkBox->isChecked();
   }

   return false;
}

/************************************************************************
 *                               pictureSet                             *
 ************************************************************************/
void FarsoFunctions::pictureSet(const Kobold::String& id, 
      const Kobold::String& filename)
{
   Farso::Picture* pic = static_cast<Farso::Picture*>(getWidget(id,
            Farso::Widget::WIDGET_TYPE_PICTURE));
   if(pic)
   {
      pic->setImage(filename);
   }
}

/************************************************************************
 *                             progressBarSet                           *
 ************************************************************************/
void FarsoFunctions::progressBarSet(const Kobold::String& id, 
      int curValue, int maxValue)
{
   Farso::ProgressBar* bar = static_cast<Farso::ProgressBar*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_PROGRESS_BAR));
   if(bar)
   {
      bar->setMaxValue(maxValue);
      bar->setValue(curValue);
   }
}

/************************************************************************
 *                         textSelectorAddOption                        *
 ************************************************************************/
void FarsoFunctions::textSelectorAddOption(const Kobold::String& id, 
      const Kobold::String& text, int index)
{
   Farso::TextSelector* tsel = static_cast<Farso::TextSelector*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_TEXT_SELECTOR));
   if(tsel)
   {
      tsel->addOption(text, index);
   }
}

/************************************************************************
 *                           textSelectorClear                          *
 ************************************************************************/
void FarsoFunctions::textSelectorClear(const Kobold::String& id)
{
   Farso::TextSelector* tsel = static_cast<Farso::TextSelector*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_TEXT_SELECTOR));
   if(tsel)
   {
      tsel->clearOptions();
   }
}

/************************************************************************
 *                        textSelectorGetSelected                       *
 ************************************************************************/
int FarsoFunctions::textSelectorGetSelected(const Kobold::String& id)
{
   int res = -1;
   Farso::TextSelector* tsel = static_cast<Farso::TextSelector*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_TEXT_SELECTOR));
   if(tsel)
   {
      res = tsel->getSelectedOption();
   }

   return res;
}

/************************************************************************
 *                        textSelectorHaveOption                        *
 ************************************************************************/
bool FarsoFunctions::textSelectorHaveOption(const Kobold::String& id,
      int index)
{
   bool res = false;
   Farso::TextSelector* tsel = static_cast<Farso::TextSelector*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_TEXT_SELECTOR));
   if(tsel)
   {
      res = tsel->haveOption(index);
   }

   return res;
}

/************************************************************************
 *                             scrollTxtSet                             *
 ************************************************************************/
void FarsoFunctions::scrollTextSet(const Kobold::String& id, 
      const Kobold::String& text)
{
   Farso::ScrollText* scroll = static_cast<Farso::ScrollText*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_SCROLL_TEXT));
   if(scroll)
   {
      scroll->setText(text);
   }
}

/************************************************************************
 *                            scrollTxtClear                            *
 ************************************************************************/
void FarsoFunctions::scrollTextClear(const Kobold::String& id)
{
   Farso::ScrollText* scroll = static_cast<Farso::ScrollText*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_SCROLL_TEXT));
   if(scroll)
   {
      scroll->clear();
   }
}

/************************************************************************
 *                                spinSet                               *
 ************************************************************************/
void FarsoFunctions::spinSet(const Kobold::String& id, float value)
{
   Farso::Spin* spin = static_cast<Farso::Spin*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_SPIN));
   if(spin)
   {
      spin->setValue(value);
   }
}

/************************************************************************
 *                             spinGetInt                               *
 ************************************************************************/
int FarsoFunctions::spinGetInt(const Kobold::String& id)
{
   Farso::Spin* spin = static_cast<Farso::Spin*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_SPIN));
   if(spin)
   {
      return spin->getIntValue();
   }

   return 0;
}

/************************************************************************
 *                            spinGetFloat                              *
 ************************************************************************/
float FarsoFunctions::spinGetFloat(const Kobold::String& id)
{
   Farso::Spin* spin = static_cast<Farso::Spin*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_SPIN));
   if(spin)
   {
      return spin->getFloatValue();
   }

   return 0.0f;
}

/************************************************************************
 *                           windowIsOpened                             *
 ************************************************************************/
bool FarsoFunctions::windowIsOpened(const Kobold::String& id)
{
   Farso::Window* window = static_cast<Farso::Window*>(
         getWidget(id, Farso::Widget::WIDGET_TYPE_WINDOW));

   return window != NULL;
}

/************************************************************************
 *                                getWidget                             *
 ************************************************************************/
Farso::Widget* FarsoFunctions::getWidget(const Kobold::String& id, 
      const Farso::Widget::WidgetType& type)
{
   Farso::Widget* res = getWidget(id);

   if((res != NULL) && (res->getType() != type))
   {
      /* Wrong type, should not return it */
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
            "Warning: Farso::Widget '%s' isn't of the expected type (%d != %d)",
            id.c_str(), type, res->getType());
      res = NULL;
   }

   return res;
}

/************************************************************************
 *                                getWidget                             *
 ************************************************************************/
Farso::Widget* FarsoFunctions::getWidget(const Kobold::String& id)
{
   Farso::Widget* res = Farso::Controller::getWidgetById(id);

   if(res == NULL)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
            "Warning: couldn't find Farso::Widget with id '%s'", id.c_str());
   }

   return res;
}

