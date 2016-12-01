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

#ifndef _dnt_modeffect_h
#define _dnt_modeffect_h

#include "dntconfig.h"

#include <kobold/kstring.h>
#include <kobold/list.h>
#include <kobold/timer.h>


namespace DNT
{

/*! Status modifier effect - A temporally effect to some status that affects
 * characters. */
class ModEffect: public Kobold::ListElement
{
   public:

      /*! Constructor
       * \param mod -> modifier value
       * \param time -> time to expire (0 to not expire)
       * \param periodicTime -> time to apply the ModEffect again 
       *                        (0 to not apply)
       * \param factorId -> id of the target factor (see: factor)
       * \param factorType -> type of the target factor (see: factor) */
      ModEffect(int mod, int time, int periodicTime, 
                Kobold::String factorId, Kobold::String factorType);
      /*! Constructor 
       * \param obj -> ModEffect to use as base */
      ModEffect(ModEffect* obj);
      /*! Constructor
       * \param saveTxt -> saveText with ModEffect info to load from */
      ModEffect(Kobold::String saveTxt);
      /*! Destructor */
      ~ModEffect();

      /*! Apply the ModEffect to the Character
       * \param actor -> pointer to the Character to apply the ModEffect */
      void apply(Character* actor);

      /*! Unapply the ModEffect to the Character
       * \param actor -> pointer to the Character to deapply the ModEffect */
      void unApply(Character* actor);

      /*! Get a string describing the ModEffect
       * \param actor -> pointer to the Character "owner" of the effect
       * \return -> representative string of the ModEffect */
      Kobold::String toReadableText(Character* actor);

      /*! Get a string to save the ModEffect
       * \return -> string with ModEffect Info */
      Kobold::String toSaveText();

      /*! Load the ModEffect from the save string
       * \param txt -> saveString with ModEffect info */
      void fromSaveText(Kobold::String txt);

      /*! Get modification value */
      int getValue();

      /*! Reset current timer */
      void resetTimer();

      /*! Check if timer is elapsed */
      bool isElapsed();
      /*! Verify if the effect is temporally (time != 0) or if will last 
       * forever (timeless, time == 0). */
      bool isTemporally();
      /*! Verify if is a periodic ModEffect (that should be reapplied after
       * each timestamp */
      bool isPeriodic();
      /*! Check if periodic timer is ellapsed (and effect should be applied
       * again. */
      bool isPeriodicElapsed();

      /*! \return if was applied or not */
      bool wasApplied();

   protected:
      /*! Do the apply on actor, with predefined value
       * \param actor -> Character to apply the ModEffect to
       * \param value -> value to use as modifier (instead of current mod) */
      void doApply(Character* actor, int value);

      int mod;             /**< The modifier value */
      Factor* cause;       /**< Thing to modify */
      Kobold::Timer timer; /**< Timer controller */
      unsigned int time;   /**< Max Time the effect remains (0 for infinity) */
      unsigned int periodicTime; /**< Periodicity of the effect (with saves), 
                                0 for non-periodic ones. */
      Kobold::Timer applyTimer; /**< Timer from last application */
      bool applied;        /**< If applied or not */
};

/*! List of ModEffects */
class ModEffectList: public Kobold::List
{
   public:
      /*! Constructor */
      ModEffectList();

      /*! Destructor */
      ~ModEffectList();

      /*! Set the Character owner of the ModEffect list
       * \param actor -> pointer to the Character owner of the list */
      void setOwner(Character* actor);

      /*! Insert the ModEffect to the list
       * \param obj -> ModEffect to insert
       * \param applyEffect -> true to apply the effect to Character
       * \return true on success */
      bool insert(ModEffect* obj, bool applyEffect);

      /*! Insert a copy of the ModEffect on the list 
       * (applying it to the owner Character)
       * \param obj -> pointer to ModEffect to insert a copy
       * \param applyEffect -> true to apply the effect to Character
       * \return true on success */
      bool insertCopy(ModEffect* obj, bool applyEffect);

      /*! Remove all expired ModEffects from the list (unappling it to 
       * the owner Character) and apply all periodic ModEffects when needed */
      void doStep();

      /*! Do the rest to the ModEffects list (removing all timed effects) */
      void rest();

      /*! Unapply all effects (but do not remove them from list) */
      void unApply();

      /*! Apply all effects at the list */
      void apply();

   protected:

      Character* owner;    /**< Pointer to the Character owner of the list */
};

}

#endif

