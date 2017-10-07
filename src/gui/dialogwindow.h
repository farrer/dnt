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

#ifndef _dnt_dialog_window_h
#define _dnt_dialog_window_h

#include "dntconfig.h"
#include <farso/controller.h>
#include <kobold/mutex.h>

namespace DNT
{
   /*! A window to show an interactive Conversation of a Thing. */
   class DialogWindow
   {
      public:

         /*! Open the dialog window for a Thing conversation.
          * \param owner Thing's to open its conversation.
          * \param pc pointer to current active character */
         static void open(Thing* owner, PlayableCharacter* pc);

         /*! Close the current opened DialogWindow */
         static void close();

         /*! Check Farso events happening on the DialogWindow.
          * \return true if treated some event, false otherwise */
         static bool checkEvents();

         /*! \return if dialog window is actually opened */
         static bool isOpened();

         /*! \return if dialog window is actually opened for an specific
          * Thing */
         static bool isOpened(Thing* owner);

         /*! Clear the dialog window elements */
         static void clear();
         /*! Set Owner text on dialog */
         static void setOwnerText(const Kobold::String& text);
         /*! Add an dialog option to the PC selections
          * \param text option's text
          * \param index option index [0,n) */
         static void addOption(const Kobold::String& text, int index);

      private:

         /*! Open the dialog window for a Thing conversation.
          * \param owner Thing's to open its conversation.
          * \param pc pointer to current active character */
         static void openNow(Thing* owner, PlayableCharacter* pc);

         /*! Close the current opened DialogWindow */
         static void closeNow();


         static Thing* owner; /**< Current owner of the dialog window */

         static Farso::Window* window; /**< The window itself */
         static Farso::Picture* picture; /**< Owner's portrait */
         static Farso::ScrollText* ownerText; /**< Owner's current text */
         static Farso::TextSelector* pcOptions; /**< Current PC text options */

         static int lastPosX; /**< Last window X position */
         static int lastPosY; /**< Last window Y position */

         static int keyPressed; /**< Current pressed option key */

         static Kobold::Mutex mutex; /**< Mutex for access control */
         static Thing* toOpenOwner;  /**< Owner of the dialog to open */
         static PlayableCharacter* toOpenPC; /**< Pc to use dialog when open */
         static bool shouldClose; /**< Mark to close window on next cycle */
   };

}

#endif

