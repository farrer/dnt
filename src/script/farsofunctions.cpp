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
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
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
 *                                getWidget                             *
 ************************************************************************/
Farso::Widget* FarsoFunctions::getWidget(const Kobold::String& id, 
      const Farso::Widget::WidgetType& type)
{
   Farso::Widget* res = getWidget(id);

   if((res != NULL) && (res->getType() != type))
   {
      /* Wrong type, should not return it */
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
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
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Warning: couldn't find Farso::Widget with id '%s'", id.c_str());
   }

   return res;
}

