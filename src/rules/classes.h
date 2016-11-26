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

#ifndef _dnt_classes_h
#define _dnt_classes_h

#include "ruledef.h"
#include "skills.h"
#include "feats.h"
#include "modifier.h"
#include "bonusandsaves.h"

namespace DNT
{

#define MAX_DEFINED_LEVEL 20

/*! Single Character Class Definition */
class Class : public Kobold::ListElement, public RuleDefinition
{
   public:
      /*! Constructor */
      Class();
      /*! Destructor */
      ~Class();

      /*! Load class definition */
      bool load(Kobold::String filename, Kobold::String imgFile, 
            Kobold::String idString, int idInt);

      /*! Apply the Skill Costs, based on the classe (in other words,
       * put the cost = 1 to all race skills)
       * \param sk -> character skill list */
      void applySkillCosts(Skills* sk);

      /*! Get the attributes modifiers based on the classe
       * \param mods -> the modifiers
       * \param sum -> if is to sum to the mods or just set the mods as
       *               the modifiers. (the sum = true is util when calling
       *               this function after a call to the similar one at
       *               the races)
       * \param sk -> character skill list */
      void getAttModifiers(int mods[6], bool sum, Skills* sk);

   private:

      Kobold::String citation;      /**< Class Citation */

      int totalModifiers;        /**< Total Modifiers on Class */
      int* modifierLevels;       /**< Level when each Modifier will be avaible*/
      Modifier* classModifiers;  /**< Class Modifiers */

      int totalFeats;            /**< Total Class Feats */
      Kobold::String* classFeats;   /**< ID String of Class Feats */
      int* reqLevel;             /**< Requerided Level for Feat */

      int totalSkills;           /**< Total Class Skills */
      Kobold::String* classSkills;  /**< Class Skills */

      int lifeDiceID;            /**< Life Dice ID */
      LevelPoints firstLevelSP;  /**< First Level Skill Points */
      LevelPoints otherLevelsSP; /**< Other Levels Skill Points */

      BonusAndSaves bonus[MAX_DEFINED_LEVEL]; /**< Bonus per Level */
};

/**! The Static List of Classes */
class Classes
{
   public:
      /*! Init the classes list to use */ 
      static void init();

      /*! Finish the classes list  */
      static void finish();

      /*! Get a Class from list
       * \param id -> integer ID of the class
       * \return class found, if found one. */
      static Class* getClassByInteger(int id);
      /*! Get a Class from list 
       * \param id -> string ID of the class
       * \return class found, if found one. */
      static Class* getClassByString(Kobold::String id);

      /*! Get total classes avaible */
      static int getTotalClasses();
      
   private:
      static Kobold::List* list;           /**< Class List */

      /*! Insert a class on List
       * \param fileName -> file name of the class to insert 
       * \param imgFile -> image file of the class
       * \param idString -> String Identifier of the class
       * \param idInt -> Integer Identifier of the class */
      static void insertClass(Kobold::String fileName, Kobold::String imgFile, 
                              Kobold::String idString, int idInt);
};

}

#endif

