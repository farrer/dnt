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

#ifndef _dnt_briefing_h
#define _dnt_briefing_h

#include <farso/controller.h>
#include <kobold/kstring.h>

namespace DNT
{

/*! The briefing class that displays informations to the user on multiple 
 * textual lines. */
class Briefing
{
   public:
      /*! Init the use of briefing 
       * \note should be called at DNT init. */
      static void init();
      /*! Done with briefing use.
       * \note should be called at DNT finish. */
      static void finish();

      /*! Display the hidden Briefing widget */
      static void show();
      /*! Hide the briefing widget */
      static void hide();

      /*! Verify if the briefing is displayed or hidden
       * \return -> true if displayed */
      static bool isDisplayed();

       /*! Add the string to the briefing (with default font and colors) 
       * \param text -> text to add */
      static bool addText(Kobold::String text);

      /*! Add formated text to the briefing (with default font and colors) */
      static bool addText(const char* format, ...);

       /*! Add the string to the briefing 
       * \param text -> text to add
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font 
       * \param forceRep -> if you force repetition or not */
      static bool addText(int R, int G, int B, Kobold::String text, 
            bool forceRep=false);

      /*! Add an warning to the briefing */
      static bool addWarning(Kobold::String text);

      /*! Add a check text to the briefieng
       * \param testName -> name of the state tested
       * \param value -> result tested value
       * \param difficulty -> difficulty tested against */
      static bool addCheckText(Kobold::String testName, int value, 
            int difficulty);

      /*! Clear the briefing text */
      static void clear();

   protected:
      static Farso::ScrollText* briefTxt; /**< Briefing Text */
      static Kobold::String lastText; /**< last text added */
      static Kobold::Timer lastAddTimer;  /**< Timer to avoid same text added */
};

}

#endif


