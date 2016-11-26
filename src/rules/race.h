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

#ifndef _dnt_race_h
#define _dnt_race_h

#include "ruledef.h"
#include "modifier.h"
#include "feats.h"
#include "skills.h"

namespace DNT
{

/*! Single Race Definition */
class Race: public Kobold::ListElement, public RuleDefinition
{
   public:
      Race();
      ~Race();

      /*! Load race definition file.
       * \return success flag */
      bool load(Kobold::String filename);

      /*! Apply the Skill Costs, based on the race (in other words,
       * put the cost = 1 to all race skills)
       * \param sk -> character skill list */
      void applySkillCosts(Skills* sk);

      /*! Get the attributes modifiers based on the race
       * \param mods -> the modifiers
       * \param sum -> if is to sum to the mods or just set the mods as
       *               the modifiers. (the sum = true is util when calling
       *               this function after a call to the similar one at
       *               the classes)
       * \param sk -> character skill list */
      void getAttModifiers(int mods[6], bool sum, Skills* sk);

   private:
      Kobold::String citation;        /**< Race Citation */

      int totalModifiers;       /**< Total Modifiers on Class */
      Modifier* raceModifiers;  /**< Race Modifiers */

      int totalFeats;             /**< Total Race Feats */
      Kobold::String* raceFeats;  /**< ID String of Race Feats */

      int totalSkills;                   /**< Total Race Skills */
      Kobold::String* raceSkills;        /**< Race Skills */
      Kobold::String* raceSkillsJustify; /**< Justify of the Skill */
};

/**! The List of Races Class */
class Races
{
   public:
      /*! Static Constructor */
      static void init();
      /*! Static Destructor */
      static void finish();

      /*! Get an Race from list
       * \param id -> integer ID of the race
       * \return race found, if found one. */
      static Race* getRaceByInteger(int id);
      /*! Get an Race from list 
       * \param id -> string ID of the race
       * \return race found, if found one. */
      static Race* getRaceByString(Kobold::String id);

      /*! Get total races avaible
       * \return number of races avaible */
      static int getTotalRaces();

   private:
      static Kobold::List* list;        /**< List of races */

      /*! Insert an race on List
       * \param fileName -> file name of the race to insert 
       * \param imgFile -> image file of the race
       * \param idString -> String Identifier of the race
       * \param idInt -> Integer Identifier of the race */
      static void insertRace(Kobold::String fileName, Kobold::String imgFile, 
                             Kobold::String idString,int idInt);
};

}

#endif
