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

#include "briefing.h"

#include "../lang/translate.h"

#include <kosound/sound.h>
#include <kobold/log.h>
#include <stdarg.h>
#include <farso/controller.h>

using namespace DNT;

#define DNT_BRIEFING_RATE 500

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
void Briefing::init()
{
   lastAddTimer.reset();

   scrollId = "";
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
void Briefing::finish()
{
}

/***********************************************************************
 *                         setScrollTextId                             *
 ***********************************************************************/
void Briefing::setScrollTextId(const Kobold::String& id)
{
   mutex.lock();

   scrollId = id;

   mutex.unlock();
}

/***********************************************************************
 *                          getScrollText                              *
 ***********************************************************************/
Farso::ScrollText* Briefing::getScrollText()
{
   Farso::ScrollText* scrollText = NULL;
   if(!scrollId.empty())
   {
      scrollText = static_cast<Farso::ScrollText*>(
            Farso::Controller::getWidgetById(scrollId));
   }

   return scrollText;
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool Briefing::isDisplayed()
{
   bool res = false;

   mutex.lock();
   Farso::ScrollText* briefTxt = getScrollText();
   if(briefTxt)
   {
      res = briefTxt->isVisible();
   }
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void Briefing::show()
{
   mutex.lock();

   Farso::ScrollText* briefTxt = getScrollText();
   if(briefTxt)
   {
      briefTxt->show();
   }

   mutex.unlock();
}

/***********************************************************************
 *                                 hide                                *
 ***********************************************************************/
void Briefing::hide()
{
   mutex.lock();

   Farso::ScrollText* briefTxt = getScrollText();
   if(briefTxt)
   {
      briefTxt->hide();
   }

   mutex.unlock();
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addText(const Kobold::String& text)
{
   return addText(Farso::Colors::colorText.red, 
         Farso::Colors::colorText.green, Farso::Colors::colorText.blue, 
         text, false);
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addTextWithBreak(const Kobold::String& text)
{
   return addText(Farso::Colors::colorText.red, 
         Farso::Colors::colorText.green, Farso::Colors::colorText.blue, 
         text, true);
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addText(int R, int G, int B, const Kobold::String& text, 
      bool breakAfter, bool forceRep)
{
   bool res = false;

   mutex.lock();
   if((text != lastText) || 
      (lastAddTimer.getMilliseconds() > DNT_BRIEFING_RATE))
   {
      lastText = text;
      lastAddTimer.reset();

      Farso::ScrollText* briefTxt = getScrollText();
      if(briefTxt)
      {
         briefTxt->addText(text, Farso::Color(R, G, B, 255));
         if(breakAfter)
         {
            briefTxt->addLineBreak();
         }
      }

      Kobold::Log::add(text);

      res = true;
   }
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                           addLineBreak                              *
 ***********************************************************************/
void Briefing::addLineBreak()
{
   mutex.lock();

   Farso::ScrollText* briefTxt = getScrollText();
   if(briefTxt)
   {
      briefTxt->addLineBreak();
   }

   mutex.unlock();
}

/***********************************************************************
 *                             addWarning                              *
 ***********************************************************************/
bool Briefing::addWarning(const Kobold::String& text)
{
   if(addText(220, 20, 20, text, true))
   {
      /* Play a sound on warnings. */
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, 
            "sndfx/gui/console.ogg");
      return true;
   }

   return false;
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addText(const char* format, ...)
{
   va_list arg;
   char buf[512];

   /* Parse the arguments  */
   va_start(arg, format);
   vsnprintf(&buf[0], 512, format, arg);
   va_end(arg);

   /* Add the formated text */
   return addText(buf);
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addTextWithBreak(const char* format, ...)
{
   va_list arg;
   char buf[512];

   /* Parse the arguments  */
   va_start(arg, format);
   vsnprintf(&buf[0], 512, format, arg);
   va_end(arg);

   /* Add the formated text */
   return addTextWithBreak(buf);
}

/***********************************************************************
 *                           Static Members                            *
 ***********************************************************************/
Kobold::String Briefing::scrollId = "";
Kobold::Timer Briefing::lastAddTimer;
Kobold::String Briefing::lastText = "";
Kobold::Mutex Briefing::mutex;

