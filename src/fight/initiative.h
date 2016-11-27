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

#ifndef _dnt_initiative_h
#define _dnt_initiative_h

#include <kobold/list.h>

#include "../core/character.h"

namespace DNT
{

/*! Initiative information on a fight: its value and character */
class InitiativeInfo : public Kobold::ListElement
{
   public:
      /*! Constructor */
      InitiativeInfo(Character* dude, int value);
      /*! Destructor */
      ~InitiativeInfo();

      /*! \return pointer to the character owner */
      Character* getCharacter();

      /*! \return rolled initiative value */
      int getValue();
 
    private:
      Character* dude;  /**< Character Owner */
      int value;        /**< Rolled Value */
};

/*! Initiatives on a fight: keeps a list of each character initiative on
 * a fight currently happening, allowing cycling turns of actors. */
class Initiative : public Kobold::List
{
   public:
     /*! Constructor */
     Initiative();
     /*! Destructor */
     ~Initiative();

     /*!
     *************************************************************** 
     * Insert Character on Initiative.  
     * \param pers -> Pointer to Character
     ***************************************************************/
     void insertCharacter(Character* pers);
     /*!
     *************************************************************** 
     * Update the pointers to a new Round.  
     ***************************************************************/
     void newRound();
     /*!
     *************************************************************** 
     * Gets the next Character to Play.  
     * \return pointer to next Character to play (NULL for end 
     *         of round).
     ***************************************************************/
     Character* getNextCharacter();
     /*!
     *************************************************************** 
     * Gets the current character to act.  
     * \return pointer to Character to act (NULL for none at 
     *         this round).
     ***************************************************************/
     Character* getCurrentCharacter();
     /*!
     *************************************************************** 
     * Clears The initiative List.  
     ***************************************************************/
     void clear();

   private:

     /*! Get the first initiative lesser than the value (remember:
      * greater initiative values will act first and so they are on the list)
      * \return pointer to the InitiativeInfo greater, or NULL if none */
     InitiativeInfo* getFirstLesser(int value);

     InitiativeInfo* current; /**< Current to act Character */
     InitiativeInfo* next;    /**< Next to act Character */
};

}

#endif

