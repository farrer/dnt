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

#ifndef _dnt_character_h
#define _dnt_character_h

#include "../rules/thing.h"
#include "../rules/modeffect.h"

namespace DNT
{

   /*! A Character is a living Thing. Coulb be playable by human 
    * (PlayableCharacter, or PC) or by the AI (NonPlayableCharacter, aka NPC). */
   class Character : public Thing
   {
      public:
         /*! Constructor */
         Character();
         /*! Destructor */
         virtual ~Character();

         /*! Add a modEffect to the Character
          * \param mod -> modifier value
          * \param time -> time to expire (0 == forever)
          * \param periodicTime -> time to apply the modEffect again 
          *                        (0 to not re-apply)
          * \param factorId -> id of the target factor
          * \param factorType -> type of the target factor */
         void addModEffect(int mod, int time, int periodicTime,
               Kobold::String factorId, Kobold::String factorType);

         /*! Remove all modEffects from the Character */
         void removeAllModEffects();

         /*! Get the modEffectList pointer
          * \return pointer to the modEffectList */
         ModEffectList* getEffects();

         /*! Update all effects affecting the Character */
         void updateEffects();

         /*! Verify if the Character alignment is of type represented by al
          * \param al -> string with align type
          * \return -> true if the align string identifier have the string al
          *            in it, false otherwise. 
          * \note -> a common use is, for example: isAlignOf("FREE_SOFTWARE") */
         bool isAlignOf(Kobold::String al);

   protected:

      /*! Parse key/value pairs specific to the character thing's 
       * specialization */
      bool doSpecificParse(Ogre::String key, Ogre::String value);

      /*! Parse specifc key/value pair readed from definition's file that
       * doesn't belong to the generic character specification (but to its
       * specialization). */
      virtual bool doCharacterSpecializationParse(Ogre::String key, 
            Ogre::String value) = 0;

   private:

      bool dead; /**< If the character is actually dead (just a corpse). */
      ModEffectList* effects;     /**< Current Character effects */
};

}

#endif

