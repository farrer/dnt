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
#include <stdarg.h>

using namespace DNT;

#define DNT_BRIEFING_RATE 500

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
void Briefing::init()
{
   lastAddTimer.reset();

   briefTxt = new Farso::ScrollText(0, 0, 320, 160, 
         "LiberationSans-Regular.ttf", 10, Farso::Colors::colorText, NULL);
   briefTxt->hide();
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
void Briefing::finish()
{
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool Briefing::isDisplayed()
{
   return briefTxt->isVisible();
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void Briefing::show()
{
   briefTxt->show();
}

/***********************************************************************
 *                                 hide                                *
 ***********************************************************************/
void Briefing::hide()
{
   briefTxt->hide();
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addText(Kobold::String text)
{
   return addText(Farso::Colors::colorText.red, 
         Farso::Colors::colorText.green, Farso::Colors::colorText.blue, text);
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
bool Briefing::addText(int R, int G, int B, Kobold::String text, bool forceRep)
{
   if((text != lastText) || 
      (lastAddTimer.getMilliseconds() > DNT_BRIEFING_RATE))
   {
      lastText = text;
      lastAddTimer.reset();
      briefTxt->addText(text, Farso::Color(R, G, B, 255));
      briefTxt->addLineBreak();

      return true;
   }

   return false;
}

/***********************************************************************
 *                             addWarning                              *
 ***********************************************************************/
bool Briefing::addWarning(Kobold::String text)
{
   if(addText(220, 20, 20, text))
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
 *                           addCheckText                              *
 ***********************************************************************/
bool Briefing::addCheckText(Kobold::String testName, int value, int difficulty)
{
   char buffer[512];
   bool res = (value >= difficulty);

   /* Buffer it */
   sprintf(&buffer[0], "%s: %d x %d: %s.",
         testName.c_str(), value, difficulty,
         res ? gettext("Success") : gettext("Failure"));

   if(res)
   {
      /* With blue color */
      return addText(27, 169, 245, buffer);
   }
   else
   {
      /* With red color */
      return addText(233, 0, 5, buffer);
   }
}

/***********************************************************************
 *                           Static Members                            *
 ***********************************************************************/
Farso::ScrollText* Briefing::briefTxt = NULL;
Kobold::Timer Briefing::lastAddTimer;
Kobold::String Briefing::lastText;

