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

#ifndef _dnt_script_object_dice_h
#define _dnt_script_object_dice_h

#include "dntconfig.h"
#include "scriptobject.h"
#include "../../rules/dices.h"
#include <angelscript.h>

namespace DNT
{

   /*! Dice representation for script usage */
   class ScriptObjectDice : public ScriptObject
   {
      public:
         /*! Constructor
          * \param faces number of dice faces */
         ScriptObjectDice(Dice::DiceType faces);
         ~ScriptObjectDice();

         /*! Roll the single dice
          * \return value from 1 to number of faces. */
         int roll();

         /*! Makes no sense here. assert(false) */
         void setPointer(void* newPtr);

         /*!\ return always true (our pointer is controlled by ourselves,
          * thus always valid). */
         const bool isValid() { return true; };
         
         /*! Register our functions and types */
         static void doScriptRegister(asIScriptEngine* asEngine);

         static ScriptObjectDice* factoryDice(int faces);

      private:
         Dice* dice; /**< The dice used */


   };

}

#endif


